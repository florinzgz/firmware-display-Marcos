#include "Display.h"
#include <Arduino.h>

/**
 * @brief Muestra menú oculto de configuración (activado con long press)
 */
void Display::showHiddenMenu() {
    clear(COLOR_BG);
    
    drawCenteredText("MENU CONFIGURACION", 20, COLOR_ACCENT, 2);
    
    // Opciones del menú
    const char* options[] = {
        "Calibracion",
        "Test Hardware",
        "Config WiFi/OTA",
        "Monitor INA226",
        "Estadisticas",
        "Volver"
    };
    
    int16_t y = 60;
    int16_t buttonHeight = 40;
    int16_t spacing = 50;
    int16_t buttonWidth = 300;
    int16_t x = (screenWidth - buttonWidth) / 2;
    
    for (int i = 0; i < 6; i++) {
        if (drawButton(x, y + i * spacing, buttonWidth, buttonHeight, options[i])) {
            switch (i) {
                case 0: showMenu(MenuType::CALIBRATION); break;
                case 1: showMenu(MenuType::HARDWARE_TEST); break;
                case 2: showMenu(MenuType::WIFI_CONFIG); break;
                case 3: showMenu(MenuType::INA226_MONITOR); break;
                case 4: showMenu(MenuType::STATISTICS); break;
                case 5: showMenu(MenuType::DASHBOARD); break;
            }
            delay(200);  // Debounce
        }
    }
}

/**
 * @brief Muestra menú rápido lateral (activado con swipe desde derecha)
 */
void Display::showQuickMenu() {
    // Panel lateral sobre el dashboard
    int16_t panelWidth = 150;
    int16_t panelX = screenWidth - panelWidth;
    
    // Fondo semitransparente simulado con patrón
    for (int16_t x = panelX; x < screenWidth; x += 2) {
        for (int16_t y = 0; y < screenHeight; y += 2) {
            tft.drawPixel(x, y, COLOR_BG);
        }
    }
    
    // Panel sólido
    tft.fillRect(panelX, 0, panelWidth, screenHeight, COLOR_BG_DARK);
    tft.drawLine(panelX, 0, panelX, screenHeight, COLOR_ACCENT);
    
    // Título
    tft.setTextColor(COLOR_ACCENT);
    tft.setTextSize(2);
    tft.setCursor(panelX + 10, 10);
    tft.print("ACCESOS");
    
    // Botones de acceso rápido
    int16_t btnY = 50;
    int16_t btnSpacing = 60;
    int16_t btnWidth = 130;
    int16_t btnHeight = 45;
    
    // Toggle Luces
    if (drawButton(panelX + 10, btnY, btnWidth, btnHeight, "Luces")) {
        vehicleData.status.lights = !vehicleData.status.lights;
        delay(200);
    }
    
    // Toggle 4x4
    btnY += btnSpacing;
    if (drawButton(panelX + 10, btnY, btnWidth, btnHeight, "4x4")) {
        vehicleData.status.fourWheelDrive = !vehicleData.status.fourWheelDrive;
        delay(200);
    }
    
    // Brillo
    btnY += btnSpacing;
    drawText("Brillo", panelX + 15, btnY, COLOR_TEXT, 1);
    // TODO: Slider de brillo
    
    // Botón cerrar
    btnY = screenHeight - 60;
    if (drawButton(panelX + 10, btnY, btnWidth, btnHeight, "Cerrar")) {
        showMenu(MenuType::DASHBOARD);
        delay(200);
    }
}

/**
 * @brief Muestra menú de calibración
 */
void Display::showCalibrationMenu() {
    clear(COLOR_BG);
    
    drawCenteredText("CALIBRACION", 20, COLOR_ACCENT, 2);
    
    const char* items[] = {
        "Calibrar Pedal",
        "Calibrar Encoder",
        "Calibrar INA226",
        "Reset Odometro",
        "Volver"
    };
    
    int16_t y = 70;
    int16_t buttonHeight = 45;
    int16_t spacing = 55;
    int16_t buttonWidth = 320;
    int16_t x = (screenWidth - buttonWidth) / 2;
    
    for (int i = 0; i < 5; i++) {
        if (drawButton(x, y + i * spacing, buttonWidth, buttonHeight, items[i])) {
            if (i == 4) {
                showMenu(MenuType::DASHBOARD);
            } else {
                // Simular calibración con barra de progreso
                clear(COLOR_BG);
                drawCenteredText("Calibrando...", 100, COLOR_TEXT, 2);
                
                for (int p = 0; p <= 100; p += 10) {
                    drawProgressBar(90, 150, 300, 40, p, items[i]);
                    delay(200);
                }
                
                showToast("Calibracion completada", 2000);
                delay(2000);
                showMenu(MenuType::CALIBRATION);
            }
            delay(200);
        }
    }
}

/**
 * @brief Muestra monitor INA226 en tiempo real
 */
void Display::showINA226Monitor() {
    clear(COLOR_BG);
    
    drawCenteredText("MONITOR INA226", 10, COLOR_ACCENT, 2);
    
    // Simular datos de 4 canales INA226
    float voltages[4] = {12.1f, 12.0f, 11.9f, 12.2f};
    float currents[4] = {2.5f, 3.1f, 2.8f, 2.3f};
    float powers[4];
    
    int16_t y = 50;
    int16_t rowHeight = 60;
    
    tft.setTextColor(COLOR_TEXT_DIM);
    tft.setTextSize(1);
    tft.setCursor(20, y);
    tft.print("Canal");
    tft.setCursor(120, y);
    tft.print("Voltaje");
    tft.setCursor(220, y);
    tft.print("Corriente");
    tft.setCursor(340, y);
    tft.print("Potencia");
    
    y += 25;
    
    for (int i = 0; i < 4; i++) {
        powers[i] = voltages[i] * currents[i];
        
        // Canal
        char label[10];
        sprintf(label, "INA%d", i + 1);
        tft.setTextColor(COLOR_ACCENT);
        tft.setTextSize(2);
        tft.setCursor(20, y);
        tft.print(label);
        
        // Voltaje
        char voltText[10];
        sprintf(voltText, "%.2fV", voltages[i]);
        tft.setTextColor(COLOR_TEXT);
        tft.setCursor(110, y);
        tft.print(voltText);
        
        // Corriente
        char currText[10];
        sprintf(currText, "%.2fA", currents[i]);
        tft.setCursor(210, y);
        tft.print(currText);
        
        // Potencia
        char powText[10];
        sprintf(powText, "%.1fW", powers[i]);
        tft.setCursor(330, y);
        tft.print(powText);
        
        y += rowHeight;
    }
    
    // Botón volver
    if (drawButton(160, 270, 160, 40, "Volver")) {
        showMenu(MenuType::DASHBOARD);
        delay(200);
    }
}

/**
 * @brief Muestra test de hardware completo
 */
void Display::showHardwareTest() {
    clear(COLOR_BG);
    
    drawCenteredText("TEST HARDWARE", 10, COLOR_ACCENT, 2);
    
    const char* components[] = {
        "Display TFT",
        "Touch XPT2046",
        "INA226 x4",
        "Encoders",
        "Sensores Temp",
        "Controladores Motor"
    };
    
    bool status[] = {true, true, true, false, true, true};
    
    int16_t y = 60;
    int16_t rowHeight = 35;
    
    for (int i = 0; i < 6; i++) {
        // Nombre del componente
        tft.setTextColor(COLOR_TEXT);
        tft.setTextSize(2);
        tft.setCursor(20, y);
        tft.print(components[i]);
        
        // Estado
        if (status[i]) {
            tft.fillCircle(400, y + 8, 10, COLOR_OK);
            tft.setTextColor(COLOR_OK);
            tft.setCursor(420, y);
            tft.print("OK");
        } else {
            tft.fillCircle(400, y + 8, 10, COLOR_ERROR);
            tft.setTextColor(COLOR_ERROR);
            tft.setCursor(420, y);
            tft.print("FAIL");
        }
        
        y += rowHeight;
    }
    
    // Botón volver
    if (drawButton(160, 270, 160, 40, "Volver")) {
        showMenu(MenuType::DASHBOARD);
        delay(200);
    }
}

/**
 * @brief Muestra configuración WiFi/OTA
 */
void Display::showWiFiConfig() {
    clear(COLOR_BG);
    
    drawCenteredText("CONFIG WiFi/OTA", 10, COLOR_ACCENT, 2);
    
    // Estado actual
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(1);
    tft.setCursor(20, 50);
    tft.print("SSID: SmartCar_001");
    tft.setCursor(20, 70);
    tft.print("IP: 192.168.1.100");
    tft.setCursor(20, 90);
    tft.print("Estado: Conectado");
    
    // Opciones
    int16_t y = 130;
    int16_t buttonHeight = 45;
    int16_t spacing = 55;
    int16_t buttonWidth = 280;
    int16_t x = (screenWidth - buttonWidth) / 2;
    
    const char* options[] = {
        "Escanear Redes",
        "Cambiar SSID/Pass",
        "Actualizar OTA",
        "Volver"
    };
    
    for (int i = 0; i < 4; i++) {
        if (drawButton(x, y + i * spacing, buttonWidth, buttonHeight, options[i])) {
            if (i == 3) {
                showMenu(MenuType::DASHBOARD);
            } else if (i == 2) {
                // Simular actualización OTA
                clear(COLOR_BG);
                drawCenteredText("Actualizando...", 100, COLOR_TEXT, 2);
                
                for (int p = 0; p <= 100; p += 5) {
                    drawProgressBar(90, 150, 300, 40, p, "Descargando firmware");
                    delay(100);
                }
                
                showToast("Actualizacion completada", 2000);
                delay(2000);
                showMenu(MenuType::WIFI_CONFIG);
            }
            delay(200);
        }
    }
}

/**
 * @brief Muestra estadísticas del sistema
 */
void Display::showStatistics() {
    clear(COLOR_BG);
    
    drawCenteredText("ESTADISTICAS", 10, COLOR_ACCENT, 2);
    
    // Datos de ejemplo
    struct Stat {
        const char* label;
        const char* value;
    };
    
    Stat stats[] = {
        {"Tiempo Encendido", "02:35:12"},
        {"Distancia Total", "45.3 km"},
        {"Distancia Hoy", "3.2 km"},
        {"Velocidad Media", "12.5 km/h"},
        {"Velocidad Maxima", "24.8 km/h"},
        {"Consumo Medio", "8.5 A"},
        {"Temperatura Media", "38.2 C"},
        {"Ciclos Bateria", "127"}
    };
    
    int16_t y = 50;
    int16_t rowHeight = 30;
    
    for (int i = 0; i < 8; i++) {
        // Etiqueta
        tft.setTextColor(COLOR_TEXT_DIM);
        tft.setTextSize(1);
        tft.setCursor(20, y);
        tft.print(stats[i].label);
        
        // Valor
        tft.setTextColor(COLOR_ACCENT);
        tft.setTextSize(2);
        tft.setCursor(280, y - 3);
        tft.print(stats[i].value);
        
        y += rowHeight;
    }
    
    // Botón volver
    if (drawButton(160, 270, 160, 40, "Volver")) {
        showMenu(MenuType::DASHBOARD);
        delay(200);
    }
}

/**
 * @brief Muestra alerta visual con popup modal
 */
void Display::showAlert(AlertLevel level, const char* msg) {
    // Fondo semitransparente
    for (int16_t x = 0; x < screenWidth; x += 2) {
        for (int16_t y = 0; y < screenHeight; y += 2) {
            tft.drawPixel(x, y, COLOR_BG);
        }
    }
    
    // Popup central
    int16_t popupWidth = 320;
    int16_t popupHeight = 180;
    int16_t popupX = (screenWidth - popupWidth) / 2;
    int16_t popupY = (screenHeight - popupHeight) / 2;
    
    uint16_t borderColor;
    const char* levelText;
    
    switch (level) {
        case AlertLevel::INFO:
            borderColor = COLOR_ACCENT;
            levelText = "INFORMACION";
            break;
        case AlertLevel::WARNING:
            borderColor = COLOR_WARNING;
            levelText = "ADVERTENCIA";
            break;
        case AlertLevel::ERROR:
            borderColor = COLOR_ERROR;
            levelText = "ERROR";
            break;
        case AlertLevel::CRITICAL:
            borderColor = COLOR_ERROR;
            levelText = "CRITICO";
            break;
    }
    
    tft.fillRoundRect(popupX, popupY, popupWidth, popupHeight, 10, COLOR_BG_DARK);
    tft.drawRoundRect(popupX, popupY, popupWidth, popupHeight, 10, borderColor);
    tft.drawRoundRect(popupX + 1, popupY + 1, popupWidth - 2, popupHeight - 2, 10, borderColor);
    
    // Título
    tft.setTextColor(borderColor);
    tft.setTextSize(2);
    int16_t titleWidth = strlen(levelText) * 12;
    tft.setCursor(popupX + (popupWidth - titleWidth) / 2, popupY + 20);
    tft.print(levelText);
    
    // Mensaje
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(2);
    // TODO: Word wrap para mensajes largos
    int16_t msgWidth = strlen(msg) * 12;
    tft.setCursor(popupX + (popupWidth - msgWidth) / 2, popupY + 70);
    tft.print(msg);
    
    // Botón OK
    int16_t btnWidth = 120;
    int16_t btnHeight = 40;
    int16_t btnX = popupX + (popupWidth - btnWidth) / 2;
    int16_t btnY = popupY + popupHeight - btnHeight - 20;
    
    while (true) {
        if (drawButton(btnX, btnY, btnWidth, btnHeight, "OK")) {
            needsRedraw = true;
            delay(200);
            break;
        }
        delay(50);
    }
}

/**
 * @brief Muestra notificación tipo toast no bloqueante
 */
void Display::showToast(const char* msg, uint32_t duration) {
    // Toast en la parte inferior
    int16_t toastHeight = 40;
    int16_t toastY = screenHeight - toastHeight - 10;
    int16_t toastWidth = 300;
    int16_t toastX = (screenWidth - toastWidth) / 2;
    
    tft.fillRoundRect(toastX, toastY, toastWidth, toastHeight, 8, COLOR_BG_DARK);
    tft.drawRoundRect(toastX, toastY, toastWidth, toastHeight, 8, COLOR_ACCENT);
    
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(1);
    int16_t msgWidth = strlen(msg) * 6;
    tft.setCursor(toastX + (toastWidth - msgWidth) / 2, toastY + 15);
    tft.print(msg);
    
    // El toast desaparecerá automáticamente después de duration ms
    // (se sobreescribirá con el próximo update)
}
