#include "AdvancedHUD.h"
#include "Display.h"
#include <Arduino.h>

// Inicialización de miembros estáticos
TFT_eSPI* AdvancedHUD::tft = nullptr;
HUDData AdvancedHUD::hudData = {0};
HUDData AdvancedHUD::prevHudData = {0};
unsigned long AdvancedHUD::lastUpdateTime = 0;
bool AdvancedHUD::needsFullRedraw = true;

/**
 * @brief Inicializa el sistema HUD
 */
void AdvancedHUD::init() {
    Serial.println("Inicializando Advanced HUD...");
    
    // Obtener referencia al TFT desde Display
    tft = &Display::getTFT();
    
    // Limpiar pantalla
    tft->fillScreen(COLOR_HUD_BG);
    
    // Inicializar datos
    memset(&hudData, 0, sizeof(HUDData));
    memset(&prevHudData, 0, sizeof(HUDData));
    
    // Valores por defecto
    hudData.batteryVoltage = 24.0f;
    hudData.batteryPercent = 100.0f;
    hudData.ambientTemp = 20.0f;
    hudData.gear = GearPosition::PARK;
    
    needsFullRedraw = true;
    
    Serial.println("Advanced HUD inicializado");
}

/**
 * @brief Actualiza la visualización del HUD
 */
void AdvancedHUD::update() {
    unsigned long currentTime = millis();
    
    // Actualizar a 30 FPS para máxima fluidez sin cortes
    if (currentTime - lastUpdateTime < 33 && !needsFullRedraw) {
        return;
    }
    
    lastUpdateTime = currentTime;
    
    if (needsFullRedraw) {
        // Dibujar todo
        tft->fillScreen(COLOR_HUD_BG);
        drawAmbientTemp(hudData.ambientTemp);
        drawBatteryHUD(hudData.batteryVoltage, hudData.batteryPercent);
        drawTopDownCar();
        drawSpeedometerHUD(hudData.speed);
        drawTachometerHUD(hudData.rpm);
        drawShifter(hudData.gear);
        drawPedalBar(hudData.pedalPosition);
        needsFullRedraw = false;
    } else {
        // Actualización selectiva
        
        // Temperatura ambiente (cambio >0.5°C)
        if (hasChanged(hudData.ambientTemp, prevHudData.ambientTemp, 0.5f)) {
            drawAmbientTemp(hudData.ambientTemp);
        }
        
        // Batería (cambio >0.1V o >1%)
        if (hasChanged(hudData.batteryVoltage, prevHudData.batteryVoltage, 0.1f) ||
            hasChanged(hudData.batteryPercent, prevHudData.batteryPercent, 1.0f)) {
            drawBatteryHUD(hudData.batteryVoltage, hudData.batteryPercent);
        }
        
        // Velocímetro (cambio >0.5 km/h)
        if (hasChanged(hudData.speed, prevHudData.speed, 0.5f)) {
            drawSpeedometerHUD(hudData.speed);
        }
        
        // Tacómetro (cambio >5 RPM)
        if (hasChanged(hudData.rpm, prevHudData.rpm, 5.0f)) {
            drawTachometerHUD(hudData.rpm);
        }
        
        // Shifter (cambio de marcha)
        if (hudData.gear != prevHudData.gear) {
            drawShifter(hudData.gear);
        }
        
        // Pedal (actualizar siempre - muy dinámico)
        drawPedalBar(hudData.pedalPosition);
        
        // Vista coche (ruedas, volante) - actualizar si hay cambios
        if (hasChanged(hudData.encoderAngle, prevHudData.encoderAngle, 2.0f) ||
            hasChanged(hudData.wheelFL.effort, prevHudData.wheelFL.effort, 2.0f) ||
            hasChanged(hudData.wheelFR.effort, prevHudData.wheelFR.effort, 2.0f) ||
            hasChanged(hudData.wheelRL.effort, prevHudData.wheelRL.effort, 2.0f) ||
            hasChanged(hudData.wheelRR.effort, prevHudData.wheelRR.effort, 2.0f)) {
            drawTopDownCar();
        }
    }
    
    // Guardar estado previo
    prevHudData = hudData;
}

/**
 * @brief Actualiza los datos del HUD
 */
void AdvancedHUD::updateHUDData(const HUDData& data) {
    hudData = data;
}

/**
 * @brief Calcula esfuerzo desde corriente
 */
float AdvancedHUD::calculateEffort(float current) {
    // Corriente máxima por motor = 10A
    return constrain((current / 10.0f) * 100.0f, 0.0f, 100.0f);
}

/**
 * @brief Obtiene color según temperatura
 */
uint16_t AdvancedHUD::getTemperatureColor(float temp) {
    if (temp < 35.0f) return 0x07E0;      // Verde
    if (temp < 45.0f) return 0xFFE0;      // Amarillo
    if (temp < 55.0f) return 0xFD20;      // Naranja
    return 0xF800;                         // Rojo
}

/**
 * @brief Obtiene color según esfuerzo
 */
uint16_t AdvancedHUD::getEffortColor(float effort) {
    if (effort < 25.0f) return 0x07E0;    // Verde
    if (effort < 50.0f) return 0xFFE0;    // Amarillo
    if (effort < 75.0f) return 0xFD20;    // Naranja
    return 0xF800;                         // Rojo
}

/**
 * @brief Dibuja la vista cenital completa del coche
 */
void AdvancedHUD::drawTopDownCar() {
    // Área de la vista centrada: X=170, Y=40, W=240, H=200
    // Coche centrado en pantalla (480px width), con espacio para gauges a los lados
    int16_t baseX = 170;
    int16_t baseY = 40;
    
    // Limpiar área
    tft->fillRect(baseX, baseY, 240, 200, COLOR_HUD_BG);
    
    // Marco del coche (silueta simplificada)
    tft->drawRect(baseX + 80, baseY + 20, 80, 160, COLOR_MERCEDES_SILVER);
    tft->drawRect(baseX + 81, baseY + 21, 78, 158, COLOR_MERCEDES_SILVER);
    
    // Logo Mercedes en el techo
    drawMercedesLogo(baseX + 120, baseY + 100, 15);
    
    // Volante central con ángulo
    drawSteeringWheel(baseX + 120, baseY + 140, hudData.encoderAngle);
    
    // Conexiones de dirección
    drawSteeringLinks();
    
    // Ruedas con datos individuales
    // FL (Front Left) - Superior izquierda
    drawWheel(baseX + 40, baseY + 20, hudData.wheelFL, "FL", true);
    
    // FR (Front Right) - Superior derecha
    drawWheel(baseX + 170, baseY + 20, hudData.wheelFR, "FR", true);
    
    // RL (Rear Left) - Inferior izquierda
    drawWheel(baseX + 40, baseY + 140, hudData.wheelRL, "RL", false);
    
    // RR (Rear Right) - Inferior derecha
    drawWheel(baseX + 170, baseY + 140, hudData.wheelRR, "RR", false);
}

/**
 * @brief Dibuja una rueda individual con sus datos
 */
void AdvancedHUD::drawWheel(int16_t x, int16_t y, const WheelData& wheel, 
                            const char* label, bool isFront) {
    // Tamaño: 50x40 px
    int16_t w = 50;
    int16_t h = 40;
    
    // Fondo de la rueda
    uint16_t borderColor = wheel.online ? COLOR_HUD_ACCENT : COLOR_HUD_PANEL;
    tft->fillRoundRect(x, y, w, h, 4, COLOR_HUD_PANEL);
    tft->drawRoundRect(x, y, w, h, 4, borderColor);
    
    // Etiqueta
    tft->setTextColor(COLOR_HUD_ACCENT);
    tft->setTextSize(1);
    tft->setCursor(x + 2, y + 2);
    tft->print(label);
    
    // Barra de esfuerzo (horizontal)
    int16_t barY = y + 14;
    int16_t barW = 46;
    int16_t barH = 6;
    int16_t fillW = (barW * wheel.effort) / 100;
    uint16_t effortColor = getEffortColor(wheel.effort);
    
    tft->fillRect(x + 2, barY, barW, barH, COLOR_HUD_BG);
    tft->fillRect(x + 2, barY, fillW, barH, effortColor);
    tft->drawRect(x + 2, barY, barW, barH, COLOR_HUD_ACCENT);
    
    // Porcentaje de esfuerzo
    char effortText[8];
    sprintf(effortText, "%d%%", (int)wheel.effort);
    tft->setTextColor(effortColor);
    tft->setCursor(x + 2, y + 22);
    tft->print(effortText);
    
    // Temperatura
    char tempText[8];
    sprintf(tempText, "%.0fC", wheel.temperature);
    uint16_t tempColor = getTemperatureColor(wheel.temperature);
    tft->setTextColor(tempColor);
    tft->setCursor(x + 2, y + 31);
    tft->print(tempText);
    
    // Indicador de ángulo (solo ruedas delanteras)
    if (isFront && wheel.angle != 0) {
        // Línea indicando dirección
        int16_t centerX = x + w / 2;
        int16_t centerY = y + h - 5;
        float rad = (wheel.angle) * DEG_TO_RAD;
        int16_t endX = centerX + sin(rad) * 15;
        int16_t endY = centerY - cos(rad) * 15;
        tft->drawLine(centerX, centerY, endX, endY, COLOR_GAUGE_GREEN);
    }
}

/**
 * @brief Dibuja el logo Mercedes (estrella de 3 puntas)
 */
void AdvancedHUD::drawMercedesLogo(int16_t x, int16_t y, int16_t size) {
    // Círculo exterior
    tft->drawCircle(x, y, size, COLOR_MERCEDES_SILVER);
    tft->drawCircle(x, y, size - 1, COLOR_MERCEDES_SILVER);
    
    // Estrella de 3 puntas (120° entre puntas)
    for (int i = 0; i < 3; i++) {
        float angle = (i * 120.0f - 90.0f) * DEG_TO_RAD;
        int16_t x1 = x;
        int16_t y1 = y;
        int16_t x2 = x + cos(angle) * (size - 2);
        int16_t y2 = y + sin(angle) * (size - 2);
        
        tft->drawLine(x1, y1, x2, y2, COLOR_STAR_GOLD);
        tft->drawLine(x1, y1, x2, y2, COLOR_STAR_GOLD);  // Más grueso
    }
    
    // Centro de la estrella
    tft->fillCircle(x, y, 3, COLOR_STAR_GOLD);
}

/**
 * @brief Dibuja el volante con indicación de ángulo
 * El volante puede girar ±350 grados (700° total)
 */
void AdvancedHUD::drawSteeringWheel(int16_t x, int16_t y, int16_t angle) {
    int16_t radius = 20;
    
    // Círculo del volante
    tft->drawCircle(x, y, radius, COLOR_HUD_ACCENT);
    tft->drawCircle(x, y, radius - 1, COLOR_HUD_ACCENT);
    
    // Línea indicadora de ángulo (normalizar a 0-360° para visualización)
    // El ángulo puede ser hasta ±350°, mapeamos a radianes directamente
    float rad = angle * DEG_TO_RAD;
    int16_t x1 = x;
    int16_t y1 = y;
    int16_t x2 = x + sin(rad) * (radius - 3);
    int16_t y2 = y - cos(rad) * (radius - 3);
    
    tft->drawLine(x1, y1, x2, y2, COLOR_GAUGE_RED);
    tft->drawLine(x1 + 1, y1, x2 + 1, y2, COLOR_GAUGE_RED);  // Más grueso
    
    // Texto con ángulo
    char angleText[8];
    sprintf(angleText, "%d", angle);
    tft->setTextColor(COLOR_TEXT);
    tft->setTextSize(1);
    int16_t textWidth = strlen(angleText) * 6;
    tft->setCursor(x - textWidth / 2, y + radius + 5);
    tft->print(angleText);
    tft->print((char)247);  // Símbolo de grado
}

/**
 * @brief Dibuja los enlaces de dirección
 * Los ángulos de las ruedas FL y FR vienen directamente del firmware
 * con la geometría Ackermann ya calculada
 */
void AdvancedHUD::drawSteeringLinks() {
    // Desde volante a ruedas delanteras
    int16_t wheelCenterX = 20 + 120;  // Centro del volante
    int16_t wheelCenterY = 40 + 140;
    
    // Línea a rueda FL
    int16_t flX = 20 + 40 + 25;
    int16_t flY = 40 + 20 + 20;
    tft->drawLine(wheelCenterX, wheelCenterY, flX, flY, COLOR_HUD_PANEL);
    
    // Línea a rueda FR
    int16_t frX = 20 + 170 + 25;
    int16_t frY = 40 + 20 + 20;
    tft->drawLine(wheelCenterX, wheelCenterY, frX, frY, COLOR_HUD_PANEL);
}

/**
 * @brief Dibuja velocímetro estilo HUD
 */
void AdvancedHUD::drawSpeedometerHUD(float speed) {
    // Velocímetro a la IZQUIERDA del coche (X=320 para estar a la izquierda)
    drawCircularGauge(80, 160, 60, speed, 30.0f, "km/h", COLOR_GAUGE_RED);
}

/**
 * @brief Dibuja tacómetro estilo HUD
 */
void AdvancedHUD::drawTachometerHUD(float rpm) {
    // Tacómetro a la DERECHA del coche (X=400 para estar a la derecha)
    drawCircularGauge(400, 160, 60, rpm, 220.0f, "RPM", COLOR_GAUGE_GREEN);
}

/**
 * @brief Dibuja gauge circular genérico
 */
void AdvancedHUD::drawCircularGauge(int16_t x, int16_t y, int16_t radius,
                                    float value, float maxValue,
                                    const char* label, uint16_t needleColor) {
    // Limpiar área
    tft->fillCircle(x, y, radius + 2, COLOR_HUD_BG);
    
    // Círculo exterior
    tft->drawCircle(x, y, radius, COLOR_HUD_ACCENT);
    
    // Marcas del gauge (270° de recorrido, -135° a +135°)
    int numMarks = 7;  // 6 intervalos
    for (int i = 0; i <= numMarks; i++) {
        float angle = -135.0f + (270.0f * i / numMarks);
        float rad = angle * DEG_TO_RAD;
        int16_t x1 = x + cos(rad) * (radius - 8);
        int16_t y1 = y + sin(rad) * (radius - 8);
        int16_t x2 = x + cos(rad) * radius;
        int16_t y2 = y + sin(rad) * radius;
        
        tft->drawLine(x1, y1, x2, y2, COLOR_TEXT_DIM);
    }
    
    // Aguja
    float percent = constrain(value / maxValue, 0.0f, 1.0f);
    float needleAngle = -135.0f + (270.0f * percent);
    float needleRad = needleAngle * DEG_TO_RAD;
    int16_t needleX = x + cos(needleRad) * (radius - 10);
    int16_t needleY = y + sin(needleRad) * (radius - 10);
    
    // Sombra de la aguja
    tft->drawLine(x + 1, y + 1, needleX + 1, needleY + 1, COLOR_HUD_PANEL);
    // Aguja principal
    tft->drawLine(x, y, needleX, needleY, needleColor);
    tft->drawLine(x, y, needleX, needleY, needleColor);  // Más gruesa
    
    // Centro
    tft->fillCircle(x, y, 5, needleColor);
    tft->fillCircle(x, y, 3, COLOR_HUD_BG);
    
    // Valor digital
    char valueText[10];
    sprintf(valueText, "%.0f", value);
    tft->setTextColor(COLOR_TEXT);
    tft->setTextSize(3);
    int16_t textWidth = strlen(valueText) * 18;
    tft->setCursor(x - textWidth / 2, y + 8);
    tft->print(valueText);
    
    // Etiqueta
    tft->setTextColor(COLOR_TEXT_DIM);
    tft->setTextSize(1);
    int16_t labelWidth = strlen(label) * 6;
    tft->setCursor(x - labelWidth / 2, y + 32);
    tft->print(label);
}

/**
 * @brief Dibuja indicador de batería
 */
void AdvancedHUD::drawBatteryHUD(float voltage, float percent) {
    int16_t x = 360;
    int16_t y = 5;
    
    // Limpiar área
    tft->fillRect(x, y, 110, 15, COLOR_HUD_BG);
    
    // Texto
    char text[20];
    sprintf(text, "%.1fV %d%%", voltage, (int)percent);
    
    // Color según porcentaje
    uint16_t color;
    if (percent > 60) color = COLOR_OK;
    else if (percent > 30) color = COLOR_WARNING;
    else color = COLOR_ERROR;
    
    tft->setTextColor(color);
    tft->setTextSize(1);
    tft->setCursor(x, y);
    tft->print(text);
}

/**
 * @brief Dibuja temperatura ambiente
 */
void AdvancedHUD::drawAmbientTemp(float temp) {
    int16_t x = 10;
    int16_t y = 5;
    
    // Limpiar área
    tft->fillRect(x, y, 100, 15, COLOR_HUD_BG);
    
    // Texto
    char text[20];
    sprintf(text, "Temp: %.0fC", temp);
    
    tft->setTextColor(COLOR_TEXT);
    tft->setTextSize(1);
    tft->setCursor(x, y);
    tft->print(text);
}

/**
 * @brief Dibuja selector de marcha iluminado
 */
void AdvancedHUD::drawShifter(GearPosition gear) {
    int16_t x = 100;
    int16_t y = 290;
    int16_t w = 160;
    int16_t h = 25;
    
    // Limpiar área
    tft->fillRect(x, y, w, h, COLOR_HUD_BG);
    
    // Fondo
    tft->fillRoundRect(x, y, w, h, 4, COLOR_HUD_PANEL);
    tft->drawRoundRect(x, y, w, h, 4, COLOR_HUD_ACCENT);
    
    // Posiciones: P | D2 | D1 | N | R
    const char* labels[] = {"P", "D2", "D1", "N", "R"};
    int numGears = 5;
    int gearWidth = w / numGears;
    
    for (int i = 0; i < numGears; i++) {
        int16_t gx = x + i * gearWidth;
        
        // Verificar si es la marcha actual
        bool isCurrent = false;
        if (i == 0 && gear == GearPosition::PARK) isCurrent = true;
        else if (i == 1 && gear == GearPosition::DRIVE2) isCurrent = true;
        else if (i == 2 && gear == GearPosition::DRIVE1) isCurrent = true;
        else if (i == 3 && gear == GearPosition::NEUTRAL) isCurrent = true;
        else if (i == 4 && gear == GearPosition::REVERSE) isCurrent = true;
        
        // Color de texto
        uint16_t textColor = isCurrent ? COLOR_HUD_ACCENT : COLOR_TEXT_DIM;
        
        // Fondo iluminado si es actual
        if (isCurrent) {
            tft->fillRect(gx + 2, y + 2, gearWidth - 4, h - 4, COLOR_MERCEDES_BLUE);
        }
        
        // Texto
        tft->setTextColor(textColor);
        tft->setTextSize(2);
        int16_t labelWidth = strlen(labels[i]) * 12;
        tft->setCursor(gx + (gearWidth - labelWidth) / 2, y + 6);
        tft->print(labels[i]);
        
        // Separador vertical
        if (i < numGears - 1) {
            tft->drawLine(gx + gearWidth, y + 4, gx + gearWidth, y + h - 4, COLOR_HUD_BG);
        }
    }
}

/**
 * @brief Dibuja barra de posición del pedal
 */
void AdvancedHUD::drawPedalBar(int8_t position) {
    int16_t x = 280;
    int16_t y = 290;
    int16_t w = 180;
    int16_t h = 25;
    
    // Limpiar área
    tft->fillRect(x, y, w, h, COLOR_HUD_BG);
    
    // Label
    tft->setTextColor(COLOR_TEXT);
    tft->setTextSize(1);
    tft->setCursor(x, y + 2);
    tft->print("Pedal:");
    
    // Barra
    int16_t barX = x + 40;
    int16_t barY = y + 5;
    int16_t barW = 100;
    int16_t barH = 15;
    int16_t fillW = (barW * position) / 100;
    
    // Color según posición
    uint16_t barColor;
    if (position < 33) barColor = COLOR_MERCEDES_BLUE;  // Azul (suave)
    else if (position < 66) barColor = COLOR_GAUGE_GREEN;  // Verde (medio)
    else barColor = COLOR_GAUGE_RED;  // Rojo (máximo)
    
    // Fondo de la barra
    tft->fillRect(barX, barY, barW, barH, COLOR_HUD_PANEL);
    // Relleno
    tft->fillRect(barX, barY, fillW, barH, barColor);
    // Borde
    tft->drawRect(barX, barY, barW, barH, COLOR_HUD_ACCENT);
    
    // Porcentaje
    char percentText[8];
    sprintf(percentText, "%d%%", position);
    tft->setTextColor(COLOR_TEXT);
    tft->setCursor(barX + barW + 5, barY + 3);
    tft->print(percentText);
}

/**
 * @brief Verifica si un valor cambió significativamente
 */
bool AdvancedHUD::hasChanged(float current, float previous, float threshold) {
    return abs(current - previous) >= threshold;
}

/**
 * @brief Fuerza redibujado completo
 */
void AdvancedHUD::requestRedraw() {
    needsFullRedraw = true;
}
