#include "Display.h"
#include <Arduino.h>

/**
 * @brief Muestra el dashboard principal con todos los indicadores
 */
void Display::showDashboard() {
    // Fondo oscuro
    if (needsRedraw) {
        clear(COLOR_BG);
    }
    
    // PANEL IZQUIERDO - Velocímetro
    drawSpeedometer(vehicleData.speed);
    
    // PANEL DERECHO - Tacómetro
    drawTachometer(vehicleData.rpm);
    
    // PARTE INFERIOR - Odómetro
    drawOdometer(vehicleData.odoTotal, vehicleData.odoTrip);
    
    // ESQUINA SUPERIOR DERECHA - Batería
    drawBatteryStatus(vehicleData.batteryVoltage, vehicleData.batteryPercent);
    
    // PARTE SUPERIOR - Temperaturas
    drawTemperatures(vehicleData.motorTemp[0], vehicleData.motorTemp[1],
                    vehicleData.motorTemp[2], vehicleData.motorTemp[3]);
    
    // CENTRO SUPERIOR - Indicador de marcha
    drawGearIndicator(vehicleData.gear);
    
    // ICONOS DE ESTADO
    drawStatusIcons(vehicleData.status);
}

/**
 * @brief Dibuja velocímetro analógico estilo aguja
 */
void Display::drawSpeedometer(float speed) {
    int16_t centerX = 120;
    int16_t centerY = 160;
    int16_t radius = 100;
    
    // Fondo del velocímetro
    tft.fillCircle(centerX, centerY, radius + 5, COLOR_BG_DARK);
    tft.drawCircle(centerX, centerY, radius + 5, COLOR_ACCENT);
    
    // Marcas de velocidad (0, 5, 10, 15, 20, 25 km/h)
    for (int i = 0; i <= 25; i += 5) {
        float angle = map(i, 0, 25, 135, 45);  // 135° a 45° (270° de recorrido)
        float rad = angle * DEG_TO_RAD;
        
        int16_t x1 = centerX + cos(rad) * (radius - 10);
        int16_t y1 = centerY + sin(rad) * (radius - 10);
        int16_t x2 = centerX + cos(rad) * radius;
        int16_t y2 = centerY + sin(rad) * radius;
        
        tft.drawLine(x1, y1, x2, y2, COLOR_TEXT_DIM);
        
        // Números
        char buf[4];
        sprintf(buf, "%d", i);
        int16_t tx = centerX + cos(rad) * (radius - 25);
        int16_t ty = centerY + sin(rad) * (radius - 25);
        tft.setTextColor(COLOR_TEXT_DIM);
        tft.setTextSize(1);
        tft.setCursor(tx - 6, ty - 4);
        tft.print(buf);
    }
    
    // Aguja del velocímetro
    float angle = map(constrain(speed, 0, 25), 0, 25, 135, 45);
    float rad = angle * DEG_TO_RAD;
    int16_t needleX = centerX + cos(rad) * (radius - 15);
    int16_t needleY = centerY + sin(rad) * (radius - 15);
    
    // Sombra de la aguja
    tft.drawLine(centerX + 1, centerY + 1, needleX + 1, needleY + 1, COLOR_BG_DARK);
    // Aguja principal
    tft.drawLine(centerX, centerY, needleX, needleY, COLOR_ACCENT);
    tft.drawLine(centerX, centerY, needleX, needleY, COLOR_ACCENT);  // Más gruesa
    
    // Centro de la aguja
    tft.fillCircle(centerX, centerY, 8, COLOR_ACCENT);
    tft.fillCircle(centerX, centerY, 4, COLOR_BG);
    
    // Valor digital de velocidad
    char speedText[10];
    sprintf(speedText, "%.1f", speed);
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(3);
    int16_t textWidth = strlen(speedText) * 18;
    tft.setCursor(centerX - textWidth / 2, centerY + 30);
    tft.print(speedText);
    
    tft.setTextColor(COLOR_TEXT_DIM);
    tft.setTextSize(1);
    tft.setCursor(centerX - 15, centerY + 55);
    tft.print("km/h");
}

/**
 * @brief Dibuja tacómetro/RPM circular
 */
void Display::drawTachometer(float rpm) {
    int16_t centerX = 360;
    int16_t centerY = 160;
    int16_t radius = 100;
    
    // Fondo del tacómetro
    tft.fillCircle(centerX, centerY, radius + 5, COLOR_BG_DARK);
    tft.drawCircle(centerX, centerY, radius + 5, COLOR_ACCENT);
    
    // Arco de RPM (0-100%)
    float maxRPM = 5000.0f;  // RPM máximo para visualización
    float rpmPercent = constrain(rpm / maxRPM, 0.0f, 1.0f);
    int16_t arcAngle = 270 * rpmPercent;  // 270° máximo
    
    // Dibujar arco de progreso
    for (int16_t i = 0; i < arcAngle; i += 2) {
        float angle = 135 + i;  // Empieza en 135°
        float rad = angle * DEG_TO_RAD;
        int16_t x = centerX + cos(rad) * radius;
        int16_t y = centerY + sin(rad) * radius;
        
        uint16_t color = (i < 180) ? COLOR_OK : (i < 225) ? COLOR_WARNING : COLOR_ERROR;
        tft.fillCircle(x, y, 3, color);
    }
    
    // Marcas principales
    for (int i = 0; i <= 4; i++) {
        float angle = 135 + (i * 67.5);  // Divide 270° en 4 secciones
        float rad = angle * DEG_TO_RAD;
        
        int16_t x1 = centerX + cos(rad) * (radius - 10);
        int16_t y1 = centerY + sin(rad) * (radius - 10);
        int16_t x2 = centerX + cos(rad) * radius;
        int16_t y2 = centerY + sin(rad) * radius;
        
        tft.drawLine(x1, y1, x2, y2, COLOR_TEXT_DIM);
    }
    
    // Valor digital de RPM
    char rpmText[10];
    sprintf(rpmText, "%.0f", rpm);
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(3);
    int16_t textWidth = strlen(rpmText) * 18;
    tft.setCursor(centerX - textWidth / 2, centerY + 20);
    tft.print(rpmText);
    
    tft.setTextColor(COLOR_TEXT_DIM);
    tft.setTextSize(1);
    tft.setCursor(centerX - 12, centerY + 50);
    tft.print("RPM");
}

/**
 * @brief Dibuja odómetro digital (total + parcial)
 */
void Display::drawOdometer(float total, float trip) {
    int16_t y = screenHeight - 50;
    
    // Fondo
    tft.fillRect(140, y - 5, 200, 45, COLOR_BG_DARK);
    
    // Odómetro total
    char totalText[20];
    sprintf(totalText, "ODO: %.1f km", total);
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(2);
    tft.setCursor(150, y);
    tft.print(totalText);
    
    // Odómetro parcial (trip)
    char tripText[20];
    sprintf(tripText, "TRIP: %.1f km", trip);
    tft.setTextColor(COLOR_TEXT_DIM);
    tft.setTextSize(1);
    tft.setCursor(150, y + 25);
    tft.print(tripText);
}

/**
 * @brief Dibuja barra de batería animada con porcentaje y voltaje
 */
void Display::drawBatteryStatus(float voltage, float percent) {
    int16_t x = screenWidth - 120;
    int16_t y = 10;
    int16_t width = 100;
    int16_t height = 40;
    
    // Marco de batería
    tft.drawRect(x, y, width, height, COLOR_TEXT);
    tft.fillRect(x + width, y + 10, 5, 20, COLOR_TEXT);  // Terminal +
    
    // Rellenar según porcentaje
    uint16_t fillColor;
    if (percent > 60) fillColor = COLOR_OK;
    else if (percent > 30) fillColor = COLOR_WARNING;
    else fillColor = COLOR_ERROR;
    
    int16_t fillWidth = (width - 4) * percent / 100;
    tft.fillRect(x + 2, y + 2, fillWidth, height - 4, fillColor);
    
    // Texto de porcentaje
    char percentText[8];
    sprintf(percentText, "%.0f%%", percent);
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(2);
    tft.setCursor(x + 15, y + 12);
    tft.print(percentText);
    
    // Voltaje debajo
    char voltText[10];
    sprintf(voltText, "%.1fV", voltage);
    tft.setTextColor(COLOR_TEXT_DIM);
    tft.setTextSize(1);
    tft.setCursor(x + 25, y + height + 5);
    tft.print(voltText);
}

/**
 * @brief Dibuja 4 indicadores de temperatura de motores
 */
void Display::drawTemperatures(float t1, float t2, float t3, float t4) {
    float temps[4] = {t1, t2, t3, t4};
    const char* labels[4] = {"M1", "M2", "M3", "M4"};
    
    int16_t startX = 160;
    int16_t y = 10;
    int16_t spacing = 60;
    
    for (int i = 0; i < 4; i++) {
        int16_t x = startX + i * spacing;
        uint16_t color = getTemperatureColor(temps[i]);
        
        // Círculo indicador
        tft.fillCircle(x, y + 10, 12, color);
        tft.drawCircle(x, y + 10, 12, COLOR_TEXT);
        
        // Etiqueta del motor
        tft.setTextColor(COLOR_TEXT_DIM);
        tft.setTextSize(1);
        tft.setCursor(x - 9, y + 6);
        tft.print(labels[i]);
        
        // Temperatura
        char tempText[8];
        sprintf(tempText, "%.0fC", temps[i]);
        tft.setTextColor(color);
        tft.setCursor(x - 12, y + 28);
        tft.print(tempText);
    }
}

/**
 * @brief Dibuja indicador de marcha actual
 */
void Display::drawGearIndicator(GearPosition gear) {
    int16_t x = screenWidth / 2 - 40;
    int16_t y = 60;
    int16_t width = 80;
    int16_t height = 60;
    
    // Fondo
    tft.fillRoundRect(x, y, width, height, 10, COLOR_BG_DARK);
    tft.drawRoundRect(x, y, width, height, 10, COLOR_ACCENT);
    
    // Letra de marcha
    const char* gearText;
    switch (gear) {
        case GearPosition::PARK:    gearText = "P"; break;
        case GearPosition::NEUTRAL: gearText = "N"; break;
        case GearPosition::REVERSE: gearText = "R"; break;
        case GearPosition::DRIVE1:  gearText = "D1"; break;
        case GearPosition::DRIVE2:  gearText = "D2"; break;
        default: gearText = "-"; break;
    }
    
    tft.setTextColor(COLOR_ACCENT);
    tft.setTextSize(4);
    int16_t textWidth = strlen(gearText) * 24;
    tft.setCursor(x + (width - textWidth) / 2, y + 15);
    tft.print(gearText);
}

/**
 * @brief Dibuja iconos de estado del sistema
 */
void Display::drawStatusIcons(const SystemStatus& status) {
    int16_t y = screenHeight - 30;
    int16_t spacing = 40;
    int16_t startX = 10;
    
    // Luces
    if (status.lights) {
        tft.fillCircle(startX, y, 10, COLOR_WARNING);
        drawIcon(startX, y, "light", COLOR_BG);
    } else {
        tft.drawCircle(startX, y, 10, COLOR_TEXT_DIM);
    }
    
    // 4x4
    startX += spacing;
    if (status.fourWheelDrive) {
        tft.fillCircle(startX, y, 10, COLOR_OK);
        tft.setTextColor(COLOR_BG);
        tft.setTextSize(1);
        tft.setCursor(startX - 6, y - 4);
        tft.print("4x4");
    } else {
        tft.drawCircle(startX, y, 10, COLOR_TEXT_DIM);
    }
    
    // Freno de parking
    startX += spacing;
    if (status.parkingBrake) {
        tft.fillCircle(startX, y, 10, COLOR_ERROR);
        tft.setTextColor(COLOR_TEXT);
        tft.setTextSize(2);
        tft.setCursor(startX - 5, y - 8);
        tft.print("P");
    } else {
        tft.drawCircle(startX, y, 10, COLOR_TEXT_DIM);
    }
    
    // Bluetooth
    startX = screenWidth - 130;
    if (status.bluetooth) {
        tft.fillCircle(startX, y, 10, COLOR_ACCENT);
        tft.setTextColor(COLOR_BG);
        tft.setTextSize(1);
        tft.setCursor(startX - 4, y - 4);
        tft.print("BT");
    } else {
        tft.drawCircle(startX, y, 10, COLOR_TEXT_DIM);
    }
    
    // WiFi
    startX += spacing;
    if (status.wifi) {
        tft.fillCircle(startX, y, 10, COLOR_ACCENT);
        drawIcon(startX, y, "wifi", COLOR_BG);
    } else {
        tft.drawCircle(startX, y, 10, COLOR_TEXT_DIM);
    }
    
    // Warnings
    startX += spacing;
    if (status.warnings) {
        tft.fillCircle(startX, y, 10, COLOR_ERROR);
        tft.setTextColor(COLOR_TEXT);
        tft.setTextSize(2);
        tft.setCursor(startX - 4, y - 8);
        tft.print("!");
    }
}

/**
 * @brief Dibuja un icono simple
 */
void Display::drawIcon(int16_t x, int16_t y, const char* iconType, uint16_t color) {
    tft.setTextColor(color);
    tft.setTextSize(1);
    
    if (strcmp(iconType, "light") == 0) {
        tft.setCursor(x - 3, y - 4);
        tft.print("L");
    } else if (strcmp(iconType, "wifi") == 0) {
        tft.setCursor(x - 4, y - 4);
        tft.print("W");
    }
}
