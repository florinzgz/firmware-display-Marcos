# firmware-display-Marcos

Proyecto de integración de pantalla TFT ILI9488 + táctil XPT2046 en ESP32-S3-DevKitC-1

## Descripción

Este proyecto implementa un sistema de visualización y entrada táctil para el microcontrolador ESP32-S3-DevKitC-1 utilizando:
- **Display**: TFT ILI9488 (480x320 píxeles, interfaz SPI)
- **Controlador táctil**: XPT2046
- **Framework**: Arduino con PlatformIO
- **Librerías**: TFT_eSPI y XPT2046_Touchscreen

## Características

- ✅ Clase `Display` con interfaz simplificada para:
  - `init()`: Inicialización del display y controlador táctil
  - `clear()`: Limpieza de pantalla
  - `drawText()`: Dibujado de texto en coordenadas específicas
  - Soporte completo para entrada táctil con captura de coordenadas
- ✅ Aplicación de demostración que muestra texto y captura coordenadas de toque
- ✅ Base lista para implementar sistema de menús interactivos

## Conexiones Hardware

### Display TFT ILI9488 (SPI)
| Pin Display | Pin ESP32-S3 | Función |
|-------------|--------------|---------|
| VCC         | 3.3V         | Alimentación |
| GND         | GND          | Tierra |
| MOSI        | GPIO 11      | SPI MOSI |
| MISO        | GPIO 13      | SPI MISO |
| SCK         | GPIO 12      | SPI Clock |
| CS          | GPIO 10      | Chip Select |
| DC          | GPIO 9       | Data/Command |
| RST         | GPIO 14      | Reset |

### Controlador Táctil XPT2046
| Pin Touch | Pin ESP32-S3 | Función |
|-----------|--------------|---------|
| T_CS      | GPIO 8       | Touch Chip Select |
| T_IRQ     | GPIO 7       | Touch Interrupt (opcional) |
| T_DIN     | GPIO 11      | Compartido con MOSI |
| T_DOUT    | GPIO 13      | Compartido con MISO |
| T_CLK     | GPIO 12      | Compartido con SCK |

## Requisitos

- [PlatformIO](https://platformio.org/) instalado
- ESP32-S3-DevKitC-1
- Display TFT ILI9488 (480x320, SPI)
- Módulo táctil XPT2046
- Cable USB-C para programación

## Instalación y Compilación

1. **Clonar el repositorio**:
   ```bash
   git clone https://github.com/florinzgz/firmware-display-Marcos.git
   cd firmware-display-Marcos
   ```

2. **Compilar el proyecto**:
   ```bash
   pio run
   ```

3. **Cargar en el ESP32-S3**:
   ```bash
   pio run --target upload
   ```

4. **Monitorear salida serial**:
   ```bash
   pio device monitor
   ```

   O todo en uno:
   ```bash
   pio run --target upload && pio device monitor
   ```

## Estructura del Proyecto

```
firmware-display-Marcos/
├── include/
│   ├── Display.h         # Clase Display (interfaz)
│   └── User_Setup.h      # Configuración TFT_eSPI para ILI9488
├── src/
│   ├── Display.cpp       # Implementación de la clase Display
│   └── main.cpp          # Aplicación principal de demostración
├── platformio.ini        # Configuración PlatformIO
├── .gitignore
└── README.md
```

## Uso de la Clase Display

### Ejemplo básico

```cpp
#include "Display.h"

Display display;

void setup() {
    Serial.begin(115200);
    
    // Inicializar display
    display.init();
    
    // Limpiar pantalla
    display.clear(TFT_BLACK);
    
    // Mostrar texto
    display.drawText("Hola Mundo!", 10, 10, TFT_WHITE, 2);
}

void loop() {
    // Detectar toque
    if (display.isTouched()) {
        int16_t x, y;
        if (display.getTouchCoordinates(&x, &y)) {
            Serial.printf("Touch: X=%d, Y=%d\n", x, y);
        }
    }
}
```

### API de la Clase Display

#### Métodos principales

- `bool init()` - Inicializa display y touch
- `void clear(uint16_t color = TFT_BLACK)` - Limpia pantalla con color
- `void drawText(const char* text, int16_t x, int16_t y, uint16_t color = TFT_WHITE, uint8_t size = 2)` - Dibuja texto
- `bool isTouched()` - Detecta si hay toque activo
- `bool getTouchCoordinates(int16_t* x, int16_t* y)` - Obtiene coordenadas de toque

#### Métodos auxiliares

- `uint16_t getWidth()` - Retorna ancho de pantalla
- `uint16_t getHeight()` - Retorna alto de pantalla
- `TFT_eSPI& getTFT()` - Acceso directo al objeto TFT para operaciones avanzadas
- `void setTextColor(uint16_t color, uint16_t bgcolor)` - Establece color de texto
- `void setTextSize(uint8_t size)` - Establece tamaño de texto

### Colores disponibles (RGB565)

```cpp
TFT_BLACK, TFT_NAVY, TFT_DARKGREEN, TFT_DARKCYAN,
TFT_MAROON, TFT_PURPLE, TFT_OLIVE, TFT_LIGHTGREY,
TFT_DARKGREY, TFT_BLUE, TFT_GREEN, TFT_CYAN,
TFT_RED, TFT_MAGENTA, TFT_YELLOW, TFT_WHITE,
TFT_ORANGE, TFT_GREENYELLOW, TFT_PINK
```

## Aplicación de Demostración

La aplicación incluida (`main.cpp`) demuestra:
- Inicialización del sistema
- Visualización de información del dispositivo
- Detección de toques en tiempo real
- Visualización de coordenadas táctiles
- Retroalimentación visual en pantalla

Al tocar la pantalla:
- Se muestran las coordenadas X, Y en pantalla
- Se dibuja un círculo rojo en la posición tocada
- Se imprime la información en el monitor serial
- Se muestra un indicador visual verde

## Próximos Pasos

Este proyecto sirve como base para implementar:
- Sistema de menús interactivos
- Botones táctiles virtuales
- Gráficos y animaciones
- Interfaces de usuario personalizadas
- Dashboards de datos

## Calibración del Touch

Si las coordenadas táctiles no son precisas, ajusta los valores de mapeo en `Display.cpp`:

```cpp
*x = map(p.x, 200, 3800, 0, screenWidth);  // Ajustar 200 y 3800
*y = map(p.y, 200, 3800, 0, screenHeight); // según tu display
```

## Resolución de Problemas

### El display no muestra nada
- Verifica las conexiones físicas
- Revisa que los pines en `User_Setup.h` coincidan con tu conexión
- Asegúrate de que el display recibe 3.3V

### El touch no responde
- Verifica la conexión del pin T_CS (GPIO 8)
- Comprueba que T_IRQ esté conectado (GPIO 7)
- Revisa que los pines SPI estén compartidos correctamente

### Problemas de compilación
- Ejecuta `pio lib update` para actualizar librerías
- Limpia el proyecto: `pio run --target clean`
- Vuelve a compilar: `pio run`

## Licencia

Este proyecto está disponible bajo licencia MIT.

## Autor

Marcos - ESP32-S3 Display Integration Project

## Referencias

- [TFT_eSPI Library](https://github.com/Bodmer/TFT_eSPI)
- [XPT2046_Touchscreen Library](https://github.com/PaulStoffregen/XPT2046_Touchscreen)
- [ESP32-S3 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/)
- [PlatformIO Documentation](https://docs.platformio.org/)
