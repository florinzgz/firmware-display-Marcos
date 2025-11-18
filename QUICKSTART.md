# Guía de Inicio Rápido

Esta guía te ayudará a poner en marcha el firmware en menos de 30 minutos.

## Requisitos Previos

### Hardware
- ✅ ESP32-S3-DevKitC-1
- ✅ Pantalla TFT ILI9488 (320x480)
- ✅ Panel táctil XPT2046
- ✅ Encoder rotatorio con botón
- ✅ 3-4 potenciómetros de 10kΩ (para simular sensores)
- ✅ Cables de conexión
- ✅ Cable USB-C para programar
- ✅ Fuente de alimentación 5V/1A

### Software
- ✅ PlatformIO Core o PlatformIO IDE (VSCode)
- ✅ Python 3.x (se instala automáticamente con PlatformIO)
- ✅ Git

## Paso 1: Clonar el Repositorio

```bash
git clone https://github.com/florinzgz/firmware-display-Marcos.git
cd firmware-display-Marcos
```

## Paso 2: Conectar el Hardware

### Conexión Mínima (solo pantalla)

```
ESP32-S3    →    ILI9488
---------        --------
GPIO 13     →    SDI (MOSI)
GPIO 12     →    SDO (MISO)
GPIO 14     →    SCK
GPIO 15     →    CS
GPIO 2      →    DC
GPIO 4      →    RST
3.3V        →    VCC
GND         →    GND
```

### Conexión Táctil (opcional pero recomendado)

```
ESP32-S3    →    XPT2046
---------        ---------
GPIO 13     →    T_DIN (compartido)
GPIO 12     →    T_DO (compartido)
GPIO 14     →    T_CLK (compartido)
GPIO 33     →    T_CS
GPIO 25     →    T_IRQ
3.3V        →    VCC
GND         →    GND
```

### Conexión Encoder (opcional)

```
ESP32-S3    →    Encoder
---------        --------
GPIO 26     →    A
GPIO 27     →    B
GPIO 32     →    SW
GND         →    GND
```

### Conexión Sensores (opcional)

```
ESP32-S3    →    Potenciómetro
---------        --------------
GPIO 34     →    Pin central (dirección)
GPIO 35     →    Pin central (acelerador)
3.3V        →    Un extremo (todos)
GND         →    Otro extremo (todos)
```

**⚠️ IMPORTANTE**: NO conectar más de 3.3V a los pines GPIO del ESP32-S3.

## Paso 3: Compilar y Cargar

### Usando PlatformIO Core (Terminal)

```bash
# Instalar PlatformIO si no está instalado
pip install platformio

# Compilar el proyecto
pio run

# Subir a la placa (conectar USB-C)
pio run --target upload

# Ver monitor serie (opcional)
pio device monitor
```

### Usando PlatformIO IDE (VSCode)

1. Abrir VSCode
2. Instalar extensión "PlatformIO IDE"
3. Abrir carpeta del proyecto
4. Click en ícono de PlatformIO en la barra lateral
5. Click en "Build" para compilar
6. Click en "Upload" para cargar
7. Click en "Monitor" para ver salida

## Paso 4: Verificar Funcionamiento

### Al Encender

1. La pantalla debe mostrar "Initializing..."
2. Luego debe aparecer el coche visto desde arriba
3. Si hay sensores conectados, los valores deben cambiar

### Probar Funciones Básicas

#### Visualización del Coche
- ✓ Cuerpo del coche (azul) en el centro
- ✓ 4 ruedas en las esquinas
- ✓ Indicador de batería (esquina superior izquierda)
- ✓ Indicador de volante (esquina superior derecha)
- ✓ Indicador de pedal (esquina inferior izquierda)

#### Interacción (si tienes potenciómetros)
- Girar potenciómetro de dirección → El ángulo del volante cambia
- Girar potenciómetro de acelerador → La barra del pedal sube/baja
- Las ruedas rotan cuando hay aceleración

#### Menú de Calibración (si tienes táctil)
1. Mantener presionada esquina superior izquierda >1 segundo
2. Debe aparecer el menú de calibración
3. Si tienes encoder, girarlo debe cambiar la selección

## Paso 5: Prueba Rápida sin Sensores

Si no tienes potenciómetros aún, puedes usar el modo de demostración:

Editar `src/main.cpp` y agregar al final de `setup()`:

```cpp
// Valores de demostración
carDisplay.setWheelAngle(20.0);
carDisplay.setPedalPosition(0.6);
carDisplay.setBatteryLevel(0.85);
```

Recompilar y cargar.

## Solución de Problemas Rápidos

### Pantalla en blanco
```
✓ Verificar alimentación de pantalla
✓ Verificar conexiones SPI
✓ Verificar que GPIO 15 (CS) esté conectado
✓ Presionar reset en ESP32
```

### Pantalla muestra basura
```
✓ Verificar velocidad SPI en platformio.ini
✓ Verificar cables no muy largos (<20cm recomendado)
✓ Agregar capacitor 10µF entre VCC y GND de pantalla
```

### No compila
```
✓ Verificar que PlatformIO esté instalado correctamente
✓ Borrar carpeta .pio y volver a compilar
✓ Verificar conexión a Internet (descarga librerías)
```

### No sube el firmware
```
✓ Verificar que el cable USB sea de datos (no solo carga)
✓ Presionar botón BOOT mientras conecta
✓ Verificar drivers USB (CH340, CP2102, etc)
✓ Probar otro puerto USB
```

## Siguientes Pasos

Una vez que todo funciona:

1. 📖 Leer el [Manual de Usuario](docs/USER_MANUAL.md)
2. 🔧 Calibrar los sensores usando el menú
3. 🎨 Personalizar colores en `include/CarDisplay.h`
4. 🚀 Explorar ejemplos en carpeta `examples/`
5. 🤝 Contribuir mejoras (ver [CONTRIBUTING.md](CONTRIBUTING.md))

## Ayuda Adicional

- 📚 Documentación completa: `docs/`
- 🐛 Reportar problemas: [GitHub Issues](https://github.com/florinzgz/firmware-display-Marcos/issues)
- 💬 Preguntas: Abrir un issue con etiqueta "question"

## Recursos Útiles

- [Documentación ESP32-S3](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/)
- [TFT_eSPI Library](https://github.com/Bodmer/TFT_eSPI)
- [PlatformIO Docs](https://docs.platformio.org/)
- [Arduino ESP32](https://github.com/espressif/arduino-esp32)

¡Disfruta tu nuevo display de coche! 🚗✨
