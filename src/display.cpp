#include "Display.h"
#include <Arduino.h>

// Inicialización de miembros estáticos
TFT_eSPI Display::tft = TFT_eSPI();
XPT2046_Touchscreen Display::touch = XPT2046_Touchscreen(TOUCH_CS, TOUCH_IRQ);
uint16_t Display::screenWidth = 0;
uint16_t Display::screenHeight = 0;
bool Display::touchEnabled = false;
MenuType Display::currentMenu = MenuType::DASHBOARD;
VehicleData Display::vehicleData = {0};
TouchGesture Display::lastGesture = {GestureType::NONE, 0, 0, 0, 0, 0};
uint32_t Display::lastUpdateTime = 0;
bool Display::needsRedraw = true;
bool Display::touching = false;
int16_t Display::touchStartX = 0;
int16_t Display::touchStartY = 0;
uint32_t Display::touchStartTime = 0;
uint32_t Display::lastTouchTime = 0;

/**
 * @brief Inicializa el display y controlador táctil
 */
void Display::init() {
    Serial.println("Inicializando Display...");
    
    // Inicializar TFT
    tft.init();
    tft.setRotation(1);  // Modo horizontal (480x320)
    
    // Obtener dimensiones
    screenWidth = tft.width();
    screenHeight = tft.height();
    
    Serial.printf("Resolución: %dx%d\n", screenWidth, screenHeight);
    
    // Inicializar touch
    touchEnabled = touch.begin();
    if (touchEnabled) {
        touch.setRotation(1);  // Misma rotación que display
        Serial.println("Touch inicializado correctamente");
    } else {
        Serial.println("ADVERTENCIA: Touch no inicializado");
    }
    
    // Limpiar pantalla
    clear(COLOR_BG);
    
    // Animación de arranque
    playBootAnimation();
    
    // Inicializar datos del vehículo
    vehicleData.speed = 0.0f;
    vehicleData.rpm = 0.0f;
    vehicleData.odoTotal = 0.0f;
    vehicleData.odoTrip = 0.0f;
    vehicleData.batteryVoltage = 12.0f;
    vehicleData.batteryPercent = 100.0f;
    for (int i = 0; i < 4; i++) {
        vehicleData.motorTemp[i] = 25.0f;
        vehicleData.motorCurrent[i] = 0.0f;
    }
    vehicleData.gear = GearPosition::PARK;
    vehicleData.status = {false, false, true, false, false, false};
    
    Serial.println("Display inicializado completamente");
}

/**
 * @brief Actualiza el display - llamar en loop()
 */
void Display::update() {
    uint32_t currentTime = millis();
    
    // Actualizar detección de gestos
    updateGestureDetection();
    
    // Actualizar menú actual cada 50ms (20 FPS mínimo)
    if (currentTime - lastUpdateTime >= 50 || needsRedraw) {
        lastUpdateTime = currentTime;
        
        switch (currentMenu) {
            case MenuType::DASHBOARD:
                showDashboard();
                break;
            case MenuType::SETTINGS:
                // TODO: Implementar menú de ajustes
                break;
            case MenuType::CALIBRATION:
                showCalibrationMenu();
                break;
            case MenuType::HARDWARE_TEST:
                showHardwareTest();
                break;
            case MenuType::WIFI_CONFIG:
                showWiFiConfig();
                break;
            case MenuType::INA226_MONITOR:
                showINA226Monitor();
                break;
            case MenuType::STATISTICS:
                showStatistics();
                break;
            case MenuType::QUICK_MENU:
                showQuickMenu();
                break;
            default:
                break;
        }
        
        needsRedraw = false;
    }
    
    // Procesar gestos
    if (lastGesture.type != GestureType::NONE) {
        // Swipe desde borde derecho abre menú rápido
        if (lastGesture.type == GestureType::SWIPE_LEFT && lastGesture.startX > screenWidth - 30) {
            currentMenu = MenuType::QUICK_MENU;
            needsRedraw = true;
        }
        // Long press en esquina superior izquierda abre menú oculto
        else if (lastGesture.type == GestureType::LONG_PRESS && 
                 lastGesture.startX < 50 && lastGesture.startY < 50) {
            showHiddenMenu();
            needsRedraw = true;
        }
        
        lastGesture.type = GestureType::NONE;
    }
}

/**
 * @brief Limpia la pantalla con un color
 */
void Display::clear(uint16_t color) {
    tft.fillScreen(color);
}

/**
 * @brief Dibuja texto en coordenadas específicas
 */
void Display::drawText(const char* text, int16_t x, int16_t y, 
                      uint16_t color, uint8_t size) {
    tft.setTextColor(color);
    tft.setTextSize(size);
    tft.setCursor(x, y);
    tft.print(text);
}

/**
 * @brief Dibuja texto centrado horizontalmente
 */
void Display::drawCenteredText(const char* text, int16_t y, 
                               uint16_t color, uint8_t size) {
    int16_t textWidth = strlen(text) * 6 * size;
    int16_t x = (screenWidth - textWidth) / 2;
    drawText(text, x, y, color, size);
}

/**
 * @brief Actualiza datos del vehículo
 */
void Display::updateVehicleData(const VehicleData& data) {
    vehicleData = data;
    needsRedraw = true;
}

/**
 * @brief Verifica si hay toque activo
 */
bool Display::isTouched() {
    if (!touchEnabled) return false;
    return touch.touched();
}

/**
 * @brief Obtiene coordenadas del toque actual
 */
bool Display::getTouchCoordinates(int16_t* x, int16_t* y) {
    if (!touchEnabled || !touch.touched()) {
        return false;
    }
    
    TS_Point p = touch.getPoint();
    
    // Mapear coordenadas touch a coordenadas de pantalla
    *x = map(p.x, 200, 3800, 0, screenWidth);
    *y = map(p.y, 200, 3800, 0, screenHeight);
    
    // Limitar a bordes de pantalla
    *x = constrain(*x, 0, screenWidth - 1);
    *y = constrain(*y, 0, screenHeight - 1);
    
    return true;
}

/**
 * @brief Verifica si un punto está dentro de un rectángulo
 */
bool Display::isTouchInRect(int16_t x, int16_t y, int16_t rx, int16_t ry, 
                           int16_t rw, int16_t rh) {
    return (x >= rx && x <= rx + rw && y >= ry && y <= ry + rh);
}

/**
 * @brief Actualiza detección de gestos táctiles
 */
void Display::updateGestureDetection() {
    int16_t x, y;
    bool touched = getTouchCoordinates(&x, &y);
    uint32_t currentTime = millis();
    
    if (touched && !touching) {
        // Inicio de toque
        touching = true;
        touchStartX = x;
        touchStartY = y;
        touchStartTime = currentTime;
        lastTouchTime = currentTime;
    }
    else if (!touched && touching) {
        // Fin de toque
        touching = false;
        
        uint32_t duration = currentTime - touchStartTime;
        int16_t dx = x - touchStartX;
        int16_t dy = y - touchStartY;
        int16_t distance = sqrt(dx*dx + dy*dy);
        
        // Detectar tipo de gesto
        if (duration > 3000) {
            // Long press
            lastGesture.type = GestureType::LONG_PRESS;
        }
        else if (distance < 20) {
            // Tap simple
            lastGesture.type = GestureType::TAP;
        }
        else if (abs(dx) > abs(dy) && abs(dx) > 50) {
            // Swipe horizontal
            lastGesture.type = (dx > 0) ? GestureType::SWIPE_RIGHT : GestureType::SWIPE_LEFT;
        }
        else if (abs(dy) > abs(dx) && abs(dy) > 50) {
            // Swipe vertical
            lastGesture.type = (dy > 0) ? GestureType::SWIPE_DOWN : GestureType::SWIPE_UP;
        }
        
        lastGesture.startX = touchStartX;
        lastGesture.startY = touchStartY;
        lastGesture.endX = x;
        lastGesture.endY = y;
        lastGesture.duration = duration;
    }
}

/**
 * @brief Obtiene color según temperatura
 */
uint16_t Display::getTemperatureColor(float temp) {
    if (temp < 40.0f) return COLOR_OK;       // Verde
    if (temp < 60.0f) return COLOR_WARNING;  // Amarillo
    return COLOR_ERROR;                       // Rojo
}

/**
 * @brief Animación de arranque del sistema
 */
void Display::playBootAnimation() {
    clear(COLOR_BG);
    
    // Logo o título
    drawCenteredText("SMART CAR", screenHeight / 2 - 40, COLOR_ACCENT, 4);
    drawCenteredText("Sistema de Control", screenHeight / 2 + 10, COLOR_TEXT, 2);
    
    // Barra de progreso
    int16_t barWidth = 200;
    int16_t barHeight = 20;
    int16_t barX = (screenWidth - barWidth) / 2;
    int16_t barY = screenHeight / 2 + 60;
    
    for (int i = 0; i <= 100; i += 10) {
        int16_t fillWidth = (barWidth * i) / 100;
        tft.fillRect(barX, barY, fillWidth, barHeight, COLOR_ACCENT);
        tft.drawRect(barX, barY, barWidth, barHeight, COLOR_TEXT);
        delay(100);
    }
    
    delay(500);
    clear(COLOR_BG);
}

/**
 * @brief Animación de transición entre menús
 */
void Display::playTransition(const char* type) {
    // Fade simple
    if (strcmp(type, "fade") == 0) {
        for (int i = 0; i < 5; i++) {
            tft.fillRect(0, 0, screenWidth, screenHeight, COLOR_BG_DARK);
            delay(20);
        }
    }
}

/**
 * @brief Cambia a un menú específico
 */
void Display::showMenu(MenuType type) {
    if (type != currentMenu) {
        currentMenu = type;
        needsRedraw = true;
    }
}
