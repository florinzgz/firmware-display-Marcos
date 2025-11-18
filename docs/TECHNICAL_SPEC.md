# Especificación Técnica

## Arquitectura del Software

### Componentes Principales

#### 1. CarDisplay (Visualización del Coche)
**Archivo**: `include/CarDisplay.h`, `src/CarDisplay.cpp`

**Responsabilidades**:
- Renderizar la vista del coche desde arriba
- Dibujar y animar ruedas con rotación
- Mostrar indicadores de volante, pedal y batería
- Actualizar la visualización en tiempo real

**Métodos Públicos**:
```cpp
void begin()                        // Inicializar display
void update()                       // Actualizar pantalla completa
void setWheelAngle(float angle)    // Establecer ángulo de dirección (-45 a 45)
void setWheelRotation(float rot)   // Establecer rotación de ruedas (0 a 2π)
void setPedalPosition(float pos)   // Establecer posición del pedal (0.0 a 1.0)
void setBatteryLevel(float level)  // Establecer nivel de batería (0.0 a 1.0)
void clear()                        // Limpiar pantalla
```

**Métodos Privados**:
```cpp
void drawWheel(int x, int y, float angle, float rotation)
void drawSteeringWheel(int x, int y, float angle)
void drawPedal(int x, int y, float position)
void drawBattery(int x, int y, float level)
void drawCarBody()
```

#### 2. CalibrationMenu (Menú de Calibración)
**Archivo**: `include/CalibrationMenu.h`, `src/CalibrationMenu.cpp`

**Responsabilidades**:
- Mostrar menú de configuración de sensores
- Gestionar calibración de sensores
- Permitir activar/desactivar sensores
- Mostrar valores en tiempo real

**Métodos Públicos**:
```cpp
void begin()                                    // Inicializar menú
void show()                                     // Mostrar menú
void hide()                                     // Ocultar menú
void toggle()                                   // Alternar visibilidad
bool isVisible()                                // Consultar visibilidad
void update()                                   // Actualizar visualización
void handleEncoder(int delta)                   // Manejar input de encoder
void handleButton()                             // Manejar botón de encoder
void setSensorValue(SensorType, int value)     // Actualizar valor de sensor
void setSensorEnabled(SensorType, bool enabled) // Activar/desactivar sensor
bool isSensorEnabled(SensorType)               // Consultar estado de sensor
int getSensorMin(SensorType)                   // Obtener valor mínimo calibrado
int getSensorMax(SensorType)                   // Obtener valor máximo calibrado
```

#### 3. main.cpp (Aplicación Principal)
**Archivo**: `src/main.cpp`

**Responsabilidades**:
- Inicializar hardware (TFT, touch, encoder, ADC)
- Gestionar loop principal de la aplicación
- Leer sensores analógicos
- Coordinar interacciones entre componentes
- Gestionar gestos táctiles

### Flujo de Datos

```
Sensores Analógicos → ADC ESP32 → Mapeo de Valores → CarDisplay
                                                    ↓
                                            CalibrationMenu
                                                    ↓
Encoder Rotatorio → Interrupciones → Navegación de Menú
                                                    ↓
Panel Táctil → XPT2046 → Gestos → Toggle de Menú
```

## Configuración de Hardware

### Pantalla TFT ILI9488
- **Resolución**: 320x480 píxeles
- **Interface**: SPI
- **Frecuencia SPI**: 27 MHz (lectura: 20 MHz)
- **Profundidad de color**: 16-bit RGB565
- **Orientación**: Portrait (0°)

### Panel Táctil XPT2046
- **Resolución**: 4096 x 4096 puntos
- **Interface**: SPI (compartido con TFT)
- **Tipo**: Resistivo de 4 hilos
- **Coordenadas**: Mapeo de 200-3700 a 0-320/480

### ADC ESP32-S3
- **Resolución**: 12-bit (0-4095)
- **Voltaje de referencia**: 3.3V
- **Atenuación**: 11dB (rango completo 0-3.3V)
- **Frecuencia de muestreo**: ~10 Hz

### Encoder Rotatorio
- **Tipo**: Incremental de 2 canales (A/B)
- **Detección**: Interrupción por cambio de estado
- **Debounce**: Software (200ms para botón)
- **Resolución**: ~20 pasos por revolución (típico)

## Gestión de Memoria

### Heap
- TFT_eSPI buffer: ~307KB (320x480x2 bytes)
- Sprites no utilizados para minimizar uso de memoria
- Renderizado directo en framebuffer

### Stack
- Interrupciones: Mínimo (solo contadores)
- Loop principal: ~2KB
- Funciones de renderizado: ~4KB

### Variables Globales
- Objetos de display: ~100 bytes
- Estados de sensores: ~200 bytes
- Buffers de encoder: ~20 bytes

## Temporización

### Actualización de Pantalla
- **Frecuencia**: 10 Hz (100ms)
- **Tiempo de renderizado**: ~80ms
- **Overhead**: ~20ms para procesamiento

### Lectura de Sensores
- **Frecuencia**: 10 Hz
- **Tiempo de conversión ADC**: ~1ms por canal
- **Total 4 canales**: ~4ms

### Procesamiento de Encoder
- **Interrupciones**: Tiempo real (<10µs)
- **Procesamiento**: En loop principal
- **Latencia**: <100ms

### Detección de Touch
- **Polling**: Cada ciclo de loop (~10ms)
- **Detección de gesto**: Acumulativa
- **Timeout de long-press**: 1000ms

## Protocolo de Comunicación

### SPI (TFT + Touch)
```
Velocidad TFT:   27 MHz (write), 20 MHz (read)
Velocidad Touch: 2.5 MHz (por defecto en XPT2046)
Modo:           SPI Mode 0 (CPOL=0, CPHA=0)
Bits:           8-bit
Orden:          MSB first
```

### Interrupciones
```
GPIO 25 (Touch IRQ):  FALLING edge
GPIO 26 (Encoder A):  CHANGE
GPIO 27 (Encoder B):  CHANGE  
GPIO 32 (Button):     FALLING edge (con debounce)
```

## Paleta de Colores (RGB565)

```cpp
COLOR_BACKGROUND       = 0x0000  // Negro
COLOR_CAR_BODY         = 0x001F  // Azul
COLOR_WHEEL            = 0xFFFF  // Blanco
COLOR_TIRE             = 0x39E7  // Gris oscuro
COLOR_STEERING         = 0xFD20  // Naranja
COLOR_PEDAL            = 0x07E0  // Verde
COLOR_BATTERY_OK       = 0x07E0  // Verde (>50%)
COLOR_BATTERY_LOW      = 0xFD20  // Naranja (20-50%)
COLOR_BATTERY_CRITICAL = 0xF800  // Rojo (<20%)
COLOR_TEXT             = 0xFFFF  // Blanco
MENU_BACKGROUND        = 0x18E3  // Gris oscuro
MENU_HEADER            = 0x001F  // Azul
MENU_SELECTED          = 0x07E0  // Verde
MENU_DISABLED          = 0xF800  // Rojo
```

## Requisitos de Energía

### Consumo Estimado
- ESP32-S3 (activo): ~100mA
- TFT ILI9488: ~250mA (blanco) / ~100mA (promedio)
- Touch XPT2046: ~1mA
- Encoder: <1mA
- **Total**: ~400-500mA @ 3.3V

### Alimentación Recomendada
- Voltaje: 5V regulado
- Corriente mínima: 1A
- Regulador 3.3V interno del ESP32-S3: LDO de 600mA

## Optimizaciones

### Rendimiento
- Uso de interrupciones para encoder (no polling)
- Actualización parcial en menú (solo items modificados)
- Sin buffer doble (renderizado directo)
- Cálculos trigonométricos minimizados

### Memoria
- Sin uso de sprites para reducir heap
- Variables volátiles solo donde es necesario
- Constantes en flash (PROGMEM no necesario en ESP32)

### Energía
- Sin modo sleep implementado
- Display siempre activo
- Posible mejora futura: dim de backlight

## Extensibilidad

### Agregar Nuevos Sensores
1. Definir enum en `CalibrationMenu.h`
2. Agregar a array de `SensorConfig`
3. Actualizar `SENSOR_COUNT`
4. Mapear en `main.cpp` → `updateSensors()`

### Personalizar Visualización
1. Modificar constantes de dimensiones en `CarDisplay.h`
2. Cambiar colores en definiciones
3. Ajustar funciones `draw*()` según necesidad

### Agregar Nuevas Pantallas
1. Crear nueva clase (similar a `CalibrationMenu`)
2. Instanciar en `main.cpp`
3. Gestionar transiciones con variable de estado

## Limitaciones Conocidas

1. **Calibración no persistente**: Los valores se pierden al reiniciar
   - Solución futura: EEPROM o NVS
   
2. **Sin protección contra rebotes en encoder**: Puede causar saltos
   - Solución: Agregar filtrado capacitivo en hardware
   
3. **Touch sin calibración automática**: Coordenadas fijas
   - Solución futura: Rutina de calibración táctil
   
4. **Sin gestión de errores de SPI**: Asume comunicación siempre exitosa
   - Solución: Agregar timeouts y reintentos

5. **Actualización completa de pantalla**: No hay renderizado diferencial
   - Causa: Complejidad vs beneficio en pantalla pequeña
