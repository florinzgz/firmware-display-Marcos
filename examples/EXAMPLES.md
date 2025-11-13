# Ejemplos de uso de la clase Display

## Ejemplo 1: Menú Simple con Botones Táctiles

```cpp
#include <Arduino.h>
#include "Display.h"

Display display;

// Estructura para botón táctil
struct Button {
    int16_t x, y, w, h;
    const char* label;
    uint16_t color;
};

// Definir botones del menú
Button buttons[] = {
    {10, 180, 140, 60, "OPCION 1", TFT_BLUE},
    {170, 180, 140, 60, "OPCION 2", TFT_GREEN},
    {330, 180, 140, 60, "OPCION 3", TFT_RED}
};

void drawButton(Button& btn, bool pressed = false) {
    uint16_t color = pressed ? TFT_WHITE : btn.color;
    display.getTFT().fillRoundRect(btn.x, btn.y, btn.w, btn.h, 10, color);
    display.getTFT().drawRoundRect(btn.x, btn.y, btn.w, btn.h, 10, TFT_WHITE);
    
    // Centrar texto
    int16_t textX = btn.x + (btn.w - strlen(btn.label) * 12) / 2;
    int16_t textY = btn.y + (btn.h - 16) / 2;
    display.drawText(btn.label, textX, textY, pressed ? btn.color : TFT_WHITE, 2);
}

int checkButtonPress(int16_t x, int16_t y) {
    for (int i = 0; i < 3; i++) {
        if (x >= buttons[i].x && x <= buttons[i].x + buttons[i].w &&
            y >= buttons[i].y && y <= buttons[i].y + buttons[i].h) {
            return i;
        }
    }
    return -1;
}

void setup() {
    Serial.begin(115200);
    display.init();
    display.clear(TFT_BLACK);
    
    display.drawText("MENU PRINCIPAL", 140, 50, TFT_CYAN, 3);
    
    // Dibujar botones
    for (int i = 0; i < 3; i++) {
        drawButton(buttons[i]);
    }
}

void loop() {
    if (display.isTouched()) {
        int16_t x, y;
        if (display.getTouchCoordinates(&x, &y)) {
            int btn = checkButtonPress(x, y);
            if (btn >= 0) {
                // Efecto visual
                drawButton(buttons[btn], true);
                delay(150);
                drawButton(buttons[btn], false);
                
                // Acción del botón
                Serial.printf("Botón %d presionado: %s\n", btn + 1, buttons[btn].label);
            }
        }
        delay(200);
    }
}
```

## Ejemplo 2: Lista Desplazable

```cpp
#include <Arduino.h>
#include "Display.h"

Display display;

const char* items[] = {
    "Item 1", "Item 2", "Item 3", "Item 4", "Item 5",
    "Item 6", "Item 7", "Item 8", "Item 9", "Item 10"
};
const int itemCount = 10;
const int itemHeight = 40;
int scrollOffset = 0;

void drawList() {
    display.clear(TFT_BLACK);
    display.drawText("LISTA DE ITEMS", 10, 10, TFT_CYAN, 2);
    
    int visibleItems = (display.getHeight() - 50) / itemHeight;
    
    for (int i = 0; i < visibleItems && (i + scrollOffset) < itemCount; i++) {
        int y = 50 + i * itemHeight;
        int itemIndex = i + scrollOffset;
        
        // Fondo del item
        display.getTFT().fillRect(10, y, display.getWidth() - 20, itemHeight - 5, TFT_DARKGREY);
        display.drawText(items[itemIndex], 20, y + 10, TFT_WHITE, 2);
    }
}

void setup() {
    Serial.begin(115200);
    display.init();
    drawList();
}

void loop() {
    if (display.isTouched()) {
        int16_t x, y;
        if (display.getTouchCoordinates(&x, &y)) {
            if (y < display.getHeight() / 2 && scrollOffset > 0) {
                // Scroll up
                scrollOffset--;
                drawList();
            } else if (y >= display.getHeight() / 2 && scrollOffset < itemCount - 5) {
                // Scroll down
                scrollOffset++;
                drawList();
            }
        }
        delay(300);
    }
}
```

## Ejemplo 3: Teclado Numérico

```cpp
#include <Arduino.h>
#include "Display.h"

Display display;
String input = "";

struct Key {
    int16_t x, y, w, h;
    const char* label;
};

Key keys[] = {
    {10, 100, 70, 60, "1"}, {90, 100, 70, 60, "2"}, {170, 100, 70, 60, "3"},
    {10, 170, 70, 60, "4"}, {90, 170, 70, 60, "5"}, {170, 170, 70, 60, "6"},
    {10, 240, 70, 60, "7"}, {90, 240, 70, 60, "8"}, {170, 240, 70, 60, "9"},
    {10, 310, 150, 60, "0"}, {170, 310, 70, 60, "CLR"}
};

void drawKeypad() {
    for (int i = 0; i < 11; i++) {
        display.getTFT().fillRoundRect(keys[i].x, keys[i].y, keys[i].w, keys[i].h, 8, TFT_BLUE);
        display.getTFT().drawRoundRect(keys[i].x, keys[i].y, keys[i].w, keys[i].h, 8, TFT_WHITE);
        
        int16_t textX = keys[i].x + (keys[i].w - strlen(keys[i].label) * 12) / 2;
        int16_t textY = keys[i].y + (keys[i].h - 16) / 2;
        display.drawText(keys[i].label, textX, textY, TFT_WHITE, 2);
    }
}

void updateDisplay() {
    display.getTFT().fillRect(0, 30, display.getWidth(), 50, TFT_BLACK);
    display.drawText(input.c_str(), 20, 40, TFT_YELLOW, 3);
}

void setup() {
    Serial.begin(115200);
    display.init();
    display.clear(TFT_BLACK);
    
    display.drawText("TECLADO NUMERICO", 60, 5, TFT_CYAN, 2);
    drawKeypad();
    updateDisplay();
}

void loop() {
    if (display.isTouched()) {
        int16_t x, y;
        if (display.getTouchCoordinates(&x, &y)) {
            for (int i = 0; i < 11; i++) {
                if (x >= keys[i].x && x <= keys[i].x + keys[i].w &&
                    y >= keys[i].y && y <= keys[i].y + keys[i].h) {
                    
                    if (strcmp(keys[i].label, "CLR") == 0) {
                        input = "";
                    } else {
                        input += keys[i].label;
                    }
                    updateDisplay();
                    Serial.println("Entrada: " + input);
                    break;
                }
            }
        }
        delay(200);
    }
}
```

## Ejemplo 4: Gráficos en Tiempo Real

```cpp
#include <Arduino.h>
#include "Display.h"

Display display;
int dataPoints[100];
int dataIndex = 0;

void drawGraph() {
    // Limpiar área del gráfico
    display.getTFT().fillRect(30, 80, 420, 200, TFT_BLACK);
    
    // Dibujar ejes
    display.getTFT().drawLine(30, 280, 450, 280, TFT_WHITE);
    display.getTFT().drawLine(30, 80, 30, 280, TFT_WHITE);
    
    // Dibujar puntos de datos
    for (int i = 1; i < dataIndex; i++) {
        int x1 = 30 + (i - 1) * 4;
        int y1 = 280 - dataPoints[i - 1];
        int x2 = 30 + i * 4;
        int y2 = 280 - dataPoints[i];
        display.getTFT().drawLine(x1, y1, x2, y2, TFT_GREEN);
    }
}

void setup() {
    Serial.begin(115200);
    display.init();
    display.clear(TFT_BLACK);
    
    display.drawText("MONITOR EN TIEMPO REAL", 100, 20, TFT_CYAN, 2);
    display.drawText("Toca para reset", 150, 50, TFT_YELLOW, 1);
}

void loop() {
    // Generar dato aleatorio (simulación de sensor)
    int value = random(50, 150);
    dataPoints[dataIndex++] = value;
    
    if (dataIndex >= 100) {
        dataIndex = 0;
    }
    
    drawGraph();
    
    // Reset al tocar
    if (display.isTouched()) {
        dataIndex = 0;
        display.clear(TFT_BLACK);
        display.drawText("MONITOR EN TIEMPO REAL", 100, 20, TFT_CYAN, 2);
        delay(500);
    }
    
    delay(100);
}
```

## Funciones Útiles Adicionales

```cpp
// Dibujar rectángulo con borde
void drawBox(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
    display.getTFT().fillRect(x, y, w, h, color);
    display.getTFT().drawRect(x, y, w, h, TFT_WHITE);
}

// Centrar texto horizontalmente
void drawCenteredText(const char* text, int16_t y, uint16_t color, uint8_t size) {
    int16_t x = (display.getWidth() - strlen(text) * 6 * size) / 2;
    display.drawText(text, x, y, color, size);
}

// Verificar si un punto está dentro de un rectángulo
bool isInside(int16_t x, int16_t y, int16_t rx, int16_t ry, int16_t rw, int16_t rh) {
    return (x >= rx && x <= rx + rw && y >= ry && y <= ry + rh);
}

// Dibujar barra de progreso
void drawProgressBar(int16_t x, int16_t y, int16_t w, int16_t h, uint8_t percent) {
    display.getTFT().drawRect(x, y, w, h, TFT_WHITE);
    int16_t fillW = (w - 4) * percent / 100;
    display.getTFT().fillRect(x + 2, y + 2, fillW, h - 4, TFT_GREEN);
}
```
