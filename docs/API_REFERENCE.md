# Display Class - Quick Reference

## Inicialización

```cpp
Display display;  // Crear objeto display

display.init();   // Inicializar display y touch
```

## Métodos de Dibujo

### clear()
Limpia la pantalla con un color específico.
```cpp
display.clear();                  // Negro (por defecto)
display.clear(TFT_WHITE);        // Blanco
display.clear(TFT_BLUE);         // Azul
```

### drawText()
Dibuja texto en coordenadas específicas.
```cpp
// drawText(texto, x, y, color, tamaño)
display.drawText("Hola", 10, 20, TFT_WHITE, 2);
display.drawText("Mundo", 10, 50, TFT_CYAN, 3);
```

Parámetros:
- `texto`: String a mostrar
- `x`: Posición horizontal (píxeles desde la izquierda)
- `y`: Posición vertical (píxeles desde arriba)
- `color`: Color RGB565 (por defecto: TFT_WHITE)
- `tamaño`: Multiplicador de tamaño 1-10 (por defecto: 2)

## Métodos de Touch

### isTouched()
Verifica si hay toque activo.
```cpp
if (display.isTouched()) {
    // Hacer algo cuando se toca
}
```

### getTouchCoordinates()
Obtiene las coordenadas del toque.
```cpp
int16_t x, y;
if (display.getTouchCoordinates(&x, &y)) {
    Serial.printf("Tocado en X:%d Y:%d\n", x, y);
}
```

## Métodos de Configuración

### setTextColor()
Establece el color del texto.
```cpp
display.setTextColor(TFT_RED);              // Color de texto
display.setTextColor(TFT_RED, TFT_BLACK);  // Texto con fondo
```

### setTextSize()
Establece el tamaño del texto.
```cpp
display.setTextSize(1);  // Pequeño
display.setTextSize(2);  // Mediano (por defecto)
display.setTextSize(3);  // Grande
```

## Métodos de Información

### getWidth() / getHeight()
Obtiene las dimensiones de la pantalla.
```cpp
uint16_t ancho = display.getWidth();   // 480 píxeles
uint16_t alto = display.getHeight();   // 320 píxeles
```

### getTFT()
Acceso directo al objeto TFT_eSPI para operaciones avanzadas.
```cpp
// Dibujar línea
display.getTFT().drawLine(x1, y1, x2, y2, TFT_GREEN);

// Dibujar círculo
display.getTFT().drawCircle(x, y, radio, TFT_RED);

// Rellenar círculo
display.getTFT().fillCircle(x, y, radio, TFT_BLUE);

// Dibujar rectángulo
display.getTFT().drawRect(x, y, ancho, alto, TFT_WHITE);

// Rellenar rectángulo
display.getTFT().fillRect(x, y, ancho, alto, TFT_YELLOW);

// Dibujar rectángulo redondeado
display.getTFT().drawRoundRect(x, y, w, h, radio, color);
display.getTFT().fillRoundRect(x, y, w, h, radio, color);

// Dibujar triángulo
display.getTFT().drawTriangle(x1, y1, x2, y2, x3, y3, color);
display.getTFT().fillTriangle(x1, y1, x2, y2, x3, y3, color);
```

## Colores Predefinidos (RGB565)

### Colores Básicos
```cpp
TFT_BLACK       // Negro     0x0000
TFT_WHITE       // Blanco    0xFFFF
TFT_RED         // Rojo      0xF800
TFT_GREEN       // Verde     0x07E0
TFT_BLUE        // Azul      0x001F
TFT_CYAN        // Cian      0x07FF
TFT_MAGENTA     // Magenta   0xF81F
TFT_YELLOW      // Amarillo  0xFFE0
```

### Colores Extendidos
```cpp
TFT_NAVY        // Azul marino
TFT_DARKGREEN   // Verde oscuro
TFT_DARKCYAN    // Cian oscuro
TFT_MAROON      // Marrón
TFT_PURPLE      // Púrpura
TFT_OLIVE       // Oliva
TFT_LIGHTGREY   // Gris claro
TFT_DARKGREY    // Gris oscuro
TFT_ORANGE      // Naranja
TFT_GREENYELLOW // Verde amarillo
TFT_PINK        // Rosa
```

### Crear Color Personalizado
```cpp
// color565(r, g, b) - r:0-255, g:0-255, b:0-255
uint16_t miColor = display.getTFT().color565(128, 64, 200);
```

## Patrones de Uso Comunes

### Botón Simple
```cpp
void drawButton(int16_t x, int16_t y, const char* label) {
    display.getTFT().fillRect(x, y, 100, 40, TFT_BLUE);
    display.getTFT().drawRect(x, y, 100, 40, TFT_WHITE);
    display.drawText(label, x+10, y+12, TFT_WHITE, 2);
}

bool isButtonPressed(int16_t x, int16_t y, int16_t bx, int16_t by) {
    return (x >= bx && x <= bx+100 && y >= by && y <= by+40);
}
```

### Actualización Selectiva
```cpp
// Limpiar área específica antes de redibujar
display.getTFT().fillRect(x, y, ancho, alto, TFT_BLACK);
display.drawText("Nuevo texto", x, y, TFT_WHITE, 2);
```

### Debouncing del Touch
```cpp
unsigned long lastTouch = 0;
const unsigned long DEBOUNCE = 200;

if (display.isTouched() && (millis() - lastTouch > DEBOUNCE)) {
    lastTouch = millis();
    // Procesar toque
}
```

## Tips y Mejores Prácticas

1. **Performance**: Minimiza llamadas a `clear()` - limpia solo las áreas que cambien
2. **Touch Calibration**: Ajusta valores en `Display.cpp` si las coordenadas no son precisas
3. **Debouncing**: Siempre usa debounce para evitar múltiples lecturas del mismo toque
4. **Batching**: Agrupa múltiples operaciones de dibujo para mejor performance
5. **Memoria**: Los objetos grandes (imágenes, sprites) consumen RAM - usa PSRAM si está disponible

## Resolución de Pantalla

- **Orientación 0**: 320x480 (vertical)
- **Orientación 1**: 480x320 (horizontal) ← Por defecto
- **Orientación 2**: 320x480 (vertical invertido)
- **Orientación 3**: 480x320 (horizontal invertido)

Para cambiar orientación:
```cpp
display.getTFT().setRotation(0);  // 0, 1, 2, o 3
```

## Coordenadas

```
(0,0) ────────────────── (479,0)
  │                         │
  │    Área de Display     │
  │      480 x 320         │
  │                         │
(0,319) ──────────────── (479,319)
```

## Ejemplo Completo Mínimo

```cpp
#include <Arduino.h>
#include "Display.h"

Display display;

void setup() {
    Serial.begin(115200);
    display.init();
    display.clear(TFT_BLACK);
    display.drawText("Sistema Listo", 150, 140, TFT_GREEN, 3);
}

void loop() {
    if (display.isTouched()) {
        int16_t x, y;
        if (display.getTouchCoordinates(&x, &y)) {
            // Dibujar punto en posición tocada
            display.getTFT().fillCircle(x, y, 5, TFT_RED);
        }
        delay(50);
    }
}
```
