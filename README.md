# firmware-display-Marcos
Proyecto de integración de pantalla TFT ILI9488 + táctil XPT2046 en ESP32‑S3‑DevKitC‑1

## Descripción

Firmware completo para ESP32-S3 que implementa una visualización dinámica de un coche visto desde arriba con las siguientes características:

### Características Principales

1. **Vista Dinámica del Coche (Top-Down)**
   - Visualización del coche desde arriba en tiempo real
   - Ruedas dinámicas que rotan según el movimiento
   - Volante dinámico que refleja el ángulo de dirección
   - Pedal de acelerador con indicador visual
   - Indicador de nivel de batería con código de colores

2. **Menú de Calibración Oculto**
   - Accesible mediante pulsación larga en esquina superior izquierda (>1 segundo)
   - Calibración de sensores individuales
   - Activar/desactivar sensores
   - Visualización de valores mín/máx de cada sensor
   - Valores en tiempo real de todos los sensores

3. **Soporte para Encoder Rotatorio**
   - Navegación por el menú con encoder
   - Botón del encoder para selección/activación
   - Ajuste de valores de calibración

4. **Sensores Soportados**
   - Sensor de dirección (steering)
   - Sensor de acelerador
   - Sensor de freno
   - Sensor de batería

## Hardware Requerido

### Componentes Principales
- ESP32-S3-DevKitC-1
- Pantalla TFT ILI9488 (320x480)
- Panel táctil XPT2046
- Encoder rotatorio con botón

### Conexiones de Pines

#### Pantalla TFT ILI9488
- MISO: GPIO 12
- MOSI: GPIO 13
- SCK: GPIO 14
- CS: GPIO 15
- DC: GPIO 2
- RST: GPIO 4

#### Panel Táctil XPT2046
- CS: GPIO 33
- IRQ: GPIO 25

#### Encoder Rotatorio
- Encoder A: GPIO 26
- Encoder B: GPIO 27
- Botón: GPIO 32

#### Entradas Analógicas de Sensores
- Dirección: GPIO 34
- Acelerador: GPIO 35
- Freno: GPIO 36
- Batería: GPIO 39

## Instalación

### Requisitos
- PlatformIO Core o PlatformIO IDE
- Python 3.x (para PlatformIO)

### Pasos de Instalación

1. Clonar el repositorio:
```bash
git clone https://github.com/florinzgz/firmware-display-Marcos.git
cd firmware-display-Marcos
```

2. Compilar el proyecto:
```bash
pio run
```

3. Subir a la placa:
```bash
pio run --target upload
```

4. Monitorizar puerto serie:
```bash
pio device monitor
```

## Uso

### Pantalla Principal
Al iniciar, se muestra la vista del coche desde arriba con:
- **Ruedas**: Rotan según la velocidad del acelerador
- **Volante**: Muestra el ángulo de dirección (-45° a +45°)
- **Pedal**: Indica la posición del acelerador (0-100%)
- **Batería**: Muestra el nivel con colores:
  - Verde: >50%
  - Naranja: 20-50%
  - Rojo: <20%

### Acceso al Menú de Calibración
1. Mantener presionada la esquina superior izquierda de la pantalla durante >1 segundo
2. El menú de calibración aparecerá

### Navegación en el Menú
- **Girar encoder**: Seleccionar sensor
- **Pulsar botón**: Activar/desactivar sensor
- **Pulsación larga en pantalla**: Volver a vista principal

### Calibración de Sensores
Cada sensor muestra:
- Nombre del sensor
- Estado (ON/OFF)
- Valor actual
- Rango mín-máx

## Estructura del Proyecto

```
firmware-display-Marcos/
├── include/
│   ├── CarDisplay.h          # Clase para visualización del coche
│   └── CalibrationMenu.h     # Clase para menú de calibración
├── src/
│   ├── main.cpp              # Programa principal
│   ├── CarDisplay.cpp        # Implementación de visualización
│   └── CalibrationMenu.cpp   # Implementación del menú
├── platformio.ini            # Configuración de PlatformIO
└── README.md                 # Este archivo
```

## Bibliotecas Utilizadas

- **TFT_eSPI** (v2.5.43): Driver para pantalla TFT
- **XPT2046_Touchscreen** (v1.4): Driver para panel táctil

## Personalización

### Modificar Colores
Editar las definiciones en `include/CarDisplay.h`:
```cpp
#define COLOR_CAR_BODY 0x001F      // Azul
#define COLOR_STEERING 0xFD20      // Naranja
#define COLOR_PEDAL 0x07E0         // Verde
```

### Ajustar Dimensiones del Coche
Modificar constantes en `include/CarDisplay.h`:
```cpp
#define CAR_WIDTH 120
#define CAR_LENGTH 180
#define WHEEL_WIDTH 20
#define WHEEL_LENGTH 40
```

### Cambiar Pines
Actualizar definiciones en `src/main.cpp` y `platformio.ini`

## Resolución de Problemas

### La pantalla no muestra nada
- Verificar conexiones de pines TFT
- Comprobar que la pantalla recibe alimentación
- Revisar configuración en `platformio.ini`

### El táctil no responde
- Verificar conexiones CS e IRQ
- Comprobar calibración táctil en código
- Ajustar mapeo de coordenadas en `handleTouch()`

### Los sensores no funcionan
- Verificar que los pines analógicos estén correctamente conectados
- Comprobar resolución ADC (12-bit)
- Calibrar sensores desde el menú

### El encoder no funciona
- Verificar resistencias pull-up
- Comprobar conexiones de pines
- Revisar que las interrupciones estén activas

## Licencia

Este proyecto es de código abierto. Consultar LICENSE para más detalles.

## Contribuciones

Las contribuciones son bienvenidas. Por favor:
1. Fork el proyecto
2. Crear una rama para tu feature
3. Commit tus cambios
4. Push a la rama
5. Abrir un Pull Request

## Autor

Marcos - florinzgz

## Agradecimientos

- Bodmer por la biblioteca TFT_eSPI
- Paul Stoffregen por XPT2046_Touchscreen
