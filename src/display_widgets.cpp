#include "Display.h"
#include <Arduino.h>

/**
 * @brief Dibuja y gestiona rueda selectora circular tipo iOS
 */
int Display::drawWheel(int16_t x, int16_t y, int16_t radius, 
                      int value, int min, int max, const char* label) {
    // Círculo exterior
    tft.drawCircle(x, y, radius, COLOR_ACCENT);
    tft.drawCircle(x, y, radius - 1, COLOR_ACCENT);
    
    // Calcular ángulo basado en valor
    float percent = (float)(value - min) / (float)(max - min);
    float angle = 270 * percent - 135;  // -135° a 135° (270° total)
    float rad = angle * DEG_TO_RAD;
    
    // Indicador de posición
    int16_t indX = x + cos(rad) * (radius - 10);
    int16_t indY = y + sin(rad) * (radius - 10);
    tft.fillCircle(indX, indY, 8, COLOR_ACCENT);
    
    // Valor en el centro
    char valueText[10];
    sprintf(valueText, "%d", value);
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(3);
    int16_t textWidth = strlen(valueText) * 18;
    tft.setCursor(x - textWidth / 2, y - 12);
    tft.print(valueText);
    
    // Etiqueta debajo
    tft.setTextColor(COLOR_TEXT_DIM);
    tft.setTextSize(1);
    int16_t labelWidth = strlen(label) * 6;
    tft.setCursor(x - labelWidth / 2, y + 15);
    tft.print(label);
    
    // Detectar toque
    int16_t touchX, touchY;
    if (getTouchCoordinates(&touchX, &touchY)) {
        int16_t dx = touchX - x;
        int16_t dy = touchY - y;
        int16_t distance = sqrt(dx * dx + dy * dy);
        
        if (distance < radius) {
            // Calcular nuevo valor basado en ángulo de toque
            float touchAngle = atan2(dy, dx) * RAD_TO_DEG;
            if (touchAngle < -135) touchAngle += 360;
            
            float touchPercent = (touchAngle + 135) / 270.0;
            int newValue = min + (int)((max - min) * touchPercent);
            newValue = constrain(newValue, min, max);
            
            return newValue;
        }
    }
    
    return value;
}

/**
 * @brief Dibuja y gestiona slider horizontal
 */
int Display::drawSlider(int16_t x, int16_t y, int16_t width, 
                       int value, int min, int max, const char* label) {
    int16_t height = 20;
    int16_t handleRadius = 15;
    
    // Etiqueta
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(1);
    tft.setCursor(x, y - 15);
    tft.print(label);
    
    // Barra del slider
    tft.fillRoundRect(x, y, width, height, height / 2, COLOR_BG_DARK);
    
    // Parte llena (progreso)
    float percent = (float)(value - min) / (float)(max - min);
    int16_t fillWidth = width * percent;
    tft.fillRoundRect(x, y, fillWidth, height, height / 2, COLOR_ACCENT);
    
    // Handle (perilla)
    int16_t handleX = x + fillWidth;
    int16_t handleY = y + height / 2;
    tft.fillCircle(handleX, handleY, handleRadius, COLOR_ACCENT);
    tft.fillCircle(handleX, handleY, handleRadius - 3, COLOR_BG);
    
    // Valor actual
    char valueText[10];
    sprintf(valueText, "%d", value);
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(2);
    int16_t textWidth = strlen(valueText) * 12;
    tft.setCursor(x + width + 10, y);
    tft.print(valueText);
    
    // Detectar toque
    int16_t touchX, touchY;
    if (getTouchCoordinates(&touchX, &touchY)) {
        if (isTouchInRect(touchX, touchY, x, y - handleRadius, 
                         width, height + 2 * handleRadius)) {
            // Calcular nuevo valor basado en posición X del toque
            int newValue = map(touchX, x, x + width, min, max);
            newValue = constrain(newValue, min, max);
            return newValue;
        }
    }
    
    return value;
}

/**
 * @brief Dibuja y gestiona botón táctil con feedback visual
 */
bool Display::drawButton(int16_t x, int16_t y, int16_t w, int16_t h, 
                        const char* label) {
    bool pressed = false;
    
    // Detectar toque
    int16_t touchX, touchY;
    if (getTouchCoordinates(&touchX, &touchY)) {
        if (isTouchInRect(touchX, touchY, x, y, w, h)) {
            pressed = true;
        }
    }
    
    // Dibujar botón con estado
    uint16_t bgColor = pressed ? COLOR_ACCENT : COLOR_BG_DARK;
    uint16_t textColor = pressed ? COLOR_BG : COLOR_TEXT;
    
    tft.fillRoundRect(x, y, w, h, 8, bgColor);
    tft.drawRoundRect(x, y, w, h, 8, COLOR_ACCENT);
    
    // Texto centrado
    tft.setTextColor(textColor);
    tft.setTextSize(2);
    int16_t textWidth = strlen(label) * 12;
    int16_t textHeight = 16;
    tft.setCursor(x + (w - textWidth) / 2, y + (h - textHeight) / 2);
    tft.print(label);
    
    return pressed;
}

/**
 * @brief Dibuja gráfico de barras para corriente de 4 motores
 */
void Display::drawCurrentBars(const float currents[4]) {
    int16_t x = 50;
    int16_t y = 180;
    int16_t barWidth = 60;
    int16_t maxHeight = 100;
    int16_t spacing = 80;
    float maxCurrent = 10.0f;  // Corriente máxima para escala
    
    for (int i = 0; i < 4; i++) {
        int16_t barX = x + i * spacing;
        
        // Calcular altura de la barra
        float percent = constrain(currents[i] / maxCurrent, 0.0f, 1.0f);
        int16_t barHeight = maxHeight * percent;
        
        // Fondo de la barra
        tft.fillRect(barX, y - maxHeight, barWidth, maxHeight, COLOR_BG_DARK);
        
        // Barra de corriente con color según nivel
        uint16_t color;
        if (percent < 0.5f) color = COLOR_OK;
        else if (percent < 0.8f) color = COLOR_WARNING;
        else color = COLOR_ERROR;
        
        tft.fillRect(barX, y - barHeight, barWidth, barHeight, color);
        tft.drawRect(barX, y - maxHeight, barWidth, maxHeight, COLOR_TEXT_DIM);
        
        // Etiqueta del motor
        char label[8];
        sprintf(label, "M%d", i + 1);
        tft.setTextColor(COLOR_TEXT_DIM);
        tft.setTextSize(1);
        tft.setCursor(barX + 20, y + 5);
        tft.print(label);
        
        // Valor de corriente
        char currentText[10];
        sprintf(currentText, "%.1fA", currents[i]);
        tft.setTextColor(COLOR_TEXT);
        tft.setCursor(barX + 10, y - barHeight - 15);
        tft.print(currentText);
    }
}

/**
 * @brief Dibuja gráfico de línea histórico de voltaje
 */
void Display::drawVoltageGraph(float voltage) {
    static float voltageHistory[100] = {0};
    static int historyIndex = 0;
    
    // Añadir nuevo valor al histórico
    voltageHistory[historyIndex] = voltage;
    historyIndex = (historyIndex + 1) % 100;
    
    int16_t graphX = 50;
    int16_t graphY = 50;
    int16_t graphWidth = 380;
    int16_t graphHeight = 80;
    float minVoltage = 10.0f;
    float maxVoltage = 14.0f;
    
    // Fondo del gráfico
    tft.fillRect(graphX, graphY, graphWidth, graphHeight, COLOR_BG_DARK);
    tft.drawRect(graphX, graphY, graphWidth, graphHeight, COLOR_TEXT_DIM);
    
    // Líneas de referencia
    for (int i = 0; i <= 4; i++) {
        int16_t lineY = graphY + (graphHeight * i) / 4;
        tft.drawLine(graphX, lineY, graphX + graphWidth, lineY, COLOR_BG);
    }
    
    // Dibujar línea de voltaje
    for (int i = 1; i < 100; i++) {
        int prevIndex = (historyIndex + i - 1) % 100;
        int currIndex = (historyIndex + i) % 100;
        
        if (voltageHistory[prevIndex] > 0 && voltageHistory[currIndex] > 0) {
            int16_t x1 = graphX + ((i - 1) * graphWidth) / 100;
            int16_t y1 = graphY + graphHeight - 
                        ((voltageHistory[prevIndex] - minVoltage) / (maxVoltage - minVoltage)) * graphHeight;
            int16_t x2 = graphX + (i * graphWidth) / 100;
            int16_t y2 = graphY + graphHeight - 
                        ((voltageHistory[currIndex] - minVoltage) / (maxVoltage - minVoltage)) * graphHeight;
            
            tft.drawLine(x1, y1, x2, y2, COLOR_ACCENT);
        }
    }
    
    // Etiqueta
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(1);
    tft.setCursor(graphX + 5, graphY + 5);
    tft.print("Voltaje (V)");
    
    // Valor actual
    char voltText[10];
    sprintf(voltText, "%.2fV", voltage);
    tft.setTextColor(COLOR_ACCENT);
    tft.setTextSize(2);
    tft.setCursor(graphX + graphWidth - 60, graphY + 5);
    tft.print(voltText);
}

/**
 * @brief Dibuja barra de progreso para operaciones largas
 */
void Display::drawProgressBar(int16_t x, int16_t y, int16_t width, 
                              int16_t height, int percent, const char* label) {
    // Etiqueta
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(2);
    tft.setCursor(x, y - 25);
    tft.print(label);
    
    // Marco de la barra
    tft.drawRect(x, y, width, height, COLOR_TEXT);
    
    // Relleno de progreso
    int16_t fillWidth = (width - 4) * constrain(percent, 0, 100) / 100;
    tft.fillRect(x + 2, y + 2, fillWidth, height - 4, COLOR_ACCENT);
    
    // Porcentaje
    char percentText[8];
    sprintf(percentText, "%d%%", percent);
    tft.setTextColor(COLOR_TEXT);
    tft.setTextSize(2);
    int16_t textWidth = strlen(percentText) * 12;
    tft.setCursor(x + (width - textWidth) / 2, y + (height - 16) / 2);
    tft.print(percentText);
}
