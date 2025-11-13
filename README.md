# firmware-display-Marcos

Sistema completo de display para coche eléctrico infantil inteligente con ESP32-S3-DevKitC-1, pantalla TFT ILI9488 (480x320) y controlador táctil XPT2046.

## Descripción

Sistema de control y visualización de datos en tiempo real para un coche eléctrico infantil. Incluye dashboard completo con indicadores analógicos y digitales, widgets interactivos, sistema de menús con gestos táctiles, y visualización de datos de múltiples sensores.

### Características Principales

- 🚗 **Dashboard Dinámico**: Velocímetro analógico, tacómetro, odómetro, batería, temperaturas
- 🎮 **Widgets Interactivos**: Ruedas selectoras tipo iOS, sliders, botones táctiles
- 📱 **Sistema de Menús**: Navegación con gestos (swipe, long press, tap)
- 📊 **Visualización de Datos**: Gráficos de barras, líneas de tiempo, indicadores
- ⚡ **Animaciones**: Transiciones suaves, alertas, notificaciones toast
- 👆 **Gestos Táctiles Avanzados**: Detección de swipe, long press, tap
- 🎨 **Tema Oscuro**: Diseño moderno con colores cyan/azul

## Hardware

- **Microcontrolador**: ESP32-S3-DevKitC-1
- **Display**: TFT ILI9488 (480x320 píxeles, interfaz SPI)
- **Touch**: XPT2046 (resistivo, 4 hilos)
- **Framework**: Arduino con PlatformIO
- **Librerías**: TFT_eSPI v2.5.43, XPT2046_Touchscreen v1.4

## Conexiones Hardware

### Display TFT ILI9488 (SPI)
| Pin Display | Pin ESP32-S3 | Función |
|-------------|--------------|---------|
| VCC         | 3.3V         | Alimentación |
| GND         | GND          | Tierra |
| MOSI        | GPIO 11      | SPI MOSI |
| MISO        | GPIO 12      | SPI MISO |
| SCK         | GPIO 10      | SPI Clock |
| CS          | GPIO 8       | Chip Select |
| DC          | GPIO 13      | Data/Command |
| RST         | GPIO 14      | Reset |

### Controlador Táctil XPT2046
| Pin Touch | Pin ESP32-S3 | Función |
|-----------|--------------|---------|
| T_CS      | GPIO 22      | Touch Chip Select |
| T_IRQ     | GPIO 46      | Touch Interrupt |
| T_DIN     | GPIO 11      | Compartido con MOSI |
| T_DOUT    | GPIO 12      | Compartido con MISO |
| T_CLK     | GPIO 10      | Compartido con SCK |

## Dashboard Principal

El dashboard muestra información en tiempo real del vehículo:

### Indicadores Implementados

1. **Velocímetro Analógico** (izquierda, 120x160px)
   - Medidor de aguja estilo tradicional
   - Rango: 0-25 km/h
   - Marcas cada 5 km/h
   - Valor digital central grande

2. **Tacómetro/RPM** (derecha, 360x160px)
   - Indicador circular con arco de progreso
   - Rango: 0-5000 RPM
   - Colores: verde (bajo), amarillo (medio), rojo (alto)
   - Valor digital central

3. **Odómetro Digital** (inferior centro)
   - ODO: Kilómetros totales acumulados
   - TRIP: Kilómetros del viaje actual
   - Reinicio de TRIP desde menú

4. **Barra de Batería** (superior derecha)
   - Porcentaje visual con colores
   - Verde (>60%), Amarillo (30-60%), Rojo (<30%)
   - Voltaje debajo de la barra
   - Terminal + animado

5. **Temperaturas de Motores** (superior centro)
   - 4 indicadores circulares (M1, M2, M3, M4)
   - Colores según temperatura:
     - Verde: <40°C (OK)
     - Amarillo: 40-60°C (Atención)
     - Rojo: >60°C (Crítico)

6. **Indicador de Marcha** (centro superior)
   - Display grande con letra: P / N / R / D1 / D2
   - Fondo oscuro con borde cyan
   - Tamaño de fuente 4 (grande)

7. **Iconos de Estado** (inferior)
   - 🔦 Luces (ON/OFF)
   - 4️⃣ 4x4 (activado/desactivado)
   - 🅿️ Freno parking (activado/desactivado)
   - 📶 Bluetooth (conectado/desconectado)
   - 📡 WiFi (conectado/desconectado)
   - ⚠️ Warnings (activo cuando hay alertas)

## Widgets Interactivos

### Rueda Selectora Circular (Tipo iOS)
```cpp
int newValue = Display::drawWheel(x, y, radius, currentValue, min, max, "Brillo LED");
```
- Selector circular táctil
- Indicador de posición animado
- Touch para girar y cambiar valor
- Perfecto para: brillo, volumen, ajustes continuos

### Slider Horizontal
```cpp
int newValue = Display::drawSlider(x, y, width, currentValue, min, max, "Volumen");
```
- Barra deslizante horizontal
- Handle (perilla) arrastrable
- Visualización del progreso
- Valor numérico a la derecha

### Botón Táctil
```cpp
if (Display::drawButton(x, y, width, height, "Confirmar")) {
    // Acción al presionar
}
```
- Feedback visual inmediato
- Cambio de color al presionar
- Bordes redondeados
- Detección precisa de toque

### Gráficos de Datos
```cpp
// Barras de corriente de 4 motores
Display::drawCurrentBars(corrientes);

// Gráfico histórico de voltaje
Display::drawVoltageGraph(voltajeActual);

// Barra de progreso
Display::drawProgressBar(x, y, w, h, porcentaje, "Calibrando...");
```

## Sistema de Menús

### Menú Oculto
**Activación**: Mantener presionado 3 segundos en área de batería (esquina superior derecha)

**Opciones**:
- 🔧 Calibración (pedal, encoder, INA226)
- 🧪 Test Hardware (verificación de componentes)
- 📡 Config WiFi/OTA (actualización remota)
- 📊 Monitor INA226 (4 canales en tiempo real)
- 📈 Estadísticas (tiempo, distancias, consumos)

### Menú Rápido
**Activación**: Swipe desde borde derecho hacia izquierda

**Accesos Rápidos**:
- Toggle Luces 🔦
- Toggle 4x4 4️⃣
- Ajuste Brillo 🔆
- Botón Cerrar

### Menú de Calibración
- Calibrar Pedal
- Calibrar Encoder  
- Calibrar INA226
- Reset Odómetro
- Barras de progreso animadas durante calibración

### Monitor INA226
Muestra datos en tiempo real de 4 canales:
- Voltaje (V)
- Corriente (A)
- Potencia (W)
- Tabla actualizada continuamente

### Test de Hardware
Verifica estado de componentes:
- ✅ Display TFT
- ✅ Touch XPT2046
- ✅ INA226 x4
- ❌ Encoders
- ✅ Sensores Temperatura
- ✅ Controladores Motor

### Config WiFi/OTA
- Información de conexión (SSID, IP)
- Escanear redes disponibles
- Cambiar credenciales
- Actualización OTA con barra de progreso

### Estadísticas
8 métricas del sistema:
- Tiempo encendido
- Distancia total/hoy
- Velocidad media/máxima
- Consumo medio
- Temperatura media
- Ciclos de batería

## Gestos Táctiles

El sistema detecta automáticamente los siguientes gestos:

### TAP (Toque Simple)
- Presionar y soltar rápidamente
- Activación de botones
- Selección de elementos

### LONG PRESS (Mantener Presionado)
- Mantener >3 segundos
- Abre menú oculto desde área de batería (esquina superior derecha)
- Funciones especiales

### SWIPE (Deslizar)
- **SWIPE_LEFT**: Deslizar de derecha a izquierda
  - Desde borde derecho → Abre menú rápido
- **SWIPE_RIGHT**: Deslizar de izquierda a derecha
- **SWIPE_UP**: Deslizar de abajo hacia arriba
- **SWIPE_DOWN**: Deslizar de arriba hacia abajo

## Alertas y Notificaciones

### Alertas Modales (Bloqueantes)
```cpp
Display::showAlert(AlertLevel::ERROR, "Temperatura alta!");
```
Niveles:
- `INFO` - Información general (cyan)
- `WARNING` - Advertencia (amarillo)
- `ERROR` - Error (rojo)
- `CRITICAL` - Crítico (rojo parpadeante)

### Notificaciones Toast (No Bloqueantes)
```cpp
Display::showToast("Calibracion completada", 2000);
```
- Aparece en parte inferior
- Desaparece automáticamente
- No bloquea interacción

## Animaciones

- **Arranque**: Logo "SMART CAR" con barra de progreso
- **Transiciones**: Fade entre menús
- **Progress Bars**: Operaciones largas (calibración, OTA)
- **Indicadores**: Parpadeo en warnings

## Requisitos

- [PlatformIO](https://platformio.org/) instalado
- ESP32-S3-DevKitC-1
- Display TFT ILI9488 (480x320, SPI)
- Módulo táctil XPT2046
- Cable USB-C para programación

## Instalación y Compilación

```bash
# 1. Clonar el repositorio
git clone https://github.com/florinzgz/firmware-display-Marcos.git
cd firmware-display-Marcos

# 2. Compilar el proyecto
pio run

# 3. Cargar en el ESP32-S3
pio run --target upload

# 4. Monitorear salida serial
pio device monitor

# O todo en uno:
pio run --target upload && pio device monitor
```

## Estructura del Proyecto

```
firmware-display-Marcos/
├── include/
│   ├── Display.h            # Clase Display - API principal
│   ├── display_types.h      # Tipos, enums, estructuras
│   └── User_Setup.h         # Configuración TFT_eSPI
├── src/
│   ├── display.cpp          # Core (init, update, gestos)
│   ├── display_dashboard.cpp    # Dashboard e indicadores
│   ├── display_widgets.cpp      # Widgets interactivos
│   ├── display_menus.cpp        # Sistema de menús
│   └── main.cpp             # Aplicación demo
├── platformio.ini           # Configuración PlatformIO
├── .gitignore
└── README.md
```

## Uso de la Clase Display

### Inicialización Básica

```cpp
#include "Display.h"

void setup() {
    Serial.begin(115200);
    
    // Inicializar display y touch
    Display::init();
    
    // Mostrar dashboard
    Display::showMenu(MenuType::DASHBOARD);
}

void loop() {
    // Actualizar display (procesa touch y redibuja si necesario)
    Display::update();
}
```

### Actualización de Datos del Vehículo

```cpp
// Crear estructura con datos
VehicleData data;
data.speed = obtenerVelocidad();
data.rpm = obtenerRPM();
data.batteryVoltage = leerVoltajeBateria();
data.batteryPercent = calcularPorcentajeBateria();
data.odoTotal = leerOdometroTotal();
data.odoTrip = leerOdometroTrip();

// Temperaturas de 4 motores
data.motorTemp[0] = leerTemperaturaMotor1();
data.motorTemp[1] = leerTemperaturaMotor2();
data.motorTemp[2] = leerTemperaturaMotor3();
data.motorTemp[3] = leerTemperaturaMotor4();

// Corrientes de 4 motores
data.motorCurrent[0] = leerCorrienteMotor1();
// ... etc

// Marcha actual
data.gear = GearPosition::DRIVE1;

// Estados del sistema
data.status.lights = estadoLuces();
data.status.fourWheelDrive = estado4x4();
data.status.parkingBrake = estadoFrenoParking();
data.status.bluetooth = estadoBluetooth();
data.status.wifi = estadoWiFi();
data.status.warnings = hayAlertas();

// Actualizar display con nuevos datos
Display::updateVehicleData(data);
```

### Uso de Widgets

```cpp
// Rueda selectora para brillo LED (0-255)
static int brillo = 128;
brillo = Display::drawWheel(100, 160, 80, brillo, 0, 255, "Brillo LED");

// Slider para volumen (0-30)
static int volumen = 15;
volumen = Display::drawSlider(50, 100, 300, volumen, 0, 30, "Volumen");

// Botón táctil
if (Display::drawButton(180, 200, 140, 50, "Confirmar")) {
    // Acción al presionar botón
    Serial.println("Boton confirmado!");
}
```

### Mostrar Alertas

```cpp
// Alerta crítica
Display::showAlert(AlertLevel::CRITICAL, "Bateria baja!");

// Advertencia
Display::showAlert(AlertLevel::WARNING, "Temperatura alta");

// Información
Display::showAlert(AlertLevel::INFO, "Sistema OK");

// Notificación no bloqueante
Display::showToast("Configuracion guardada", 2000);
```

### Navegación entre Menús

```cpp
// Cambiar a menú específico
Display::showMenu(MenuType::CALIBRATION);
Display::showMenu(MenuType::WIFI_CONFIG);
Display::showMenu(MenuType::STATISTICS);

// Volver al dashboard
Display::showMenu(MenuType::DASHBOARD);
```

## API Completa

### Métodos Principales

#### Inicialización y Core
- `Display::init()` - Inicializa display y touch
- `Display::update()` - Actualiza display, procesa touch (llamar en loop)

#### Dashboard
- `Display::showDashboard()` - Muestra dashboard completo
- `Display::drawSpeedometer(speed)` - Velocímetro analógico
- `Display::drawTachometer(rpm)` - Tacómetro/RPM
- `Display::drawOdometer(total, trip)` - Odómetro digital
- `Display::drawBatteryStatus(voltage, percent)` - Barra de batería
- `Display::drawTemperatures(t1, t2, t3, t4)` - 4 temperaturas
- `Display::drawGearIndicator(gear)` - Indicador de marcha
- `Display::drawStatusIcons(status)` - Iconos de estado

#### Menús
- `Display::showMenu(type)` - Cambia a menú específico
- `Display::showHiddenMenu()` - Menú de configuración oculto
- `Display::showQuickMenu()` - Menú rápido lateral
- `Display::showCalibrationMenu()` - Menú de calibración
- `Display::showINA226Monitor()` - Monitor INA226
- `Display::showHardwareTest()` - Test de hardware
- `Display::showWiFiConfig()` - Configuración WiFi/OTA
- `Display::showStatistics()` - Estadísticas del sistema

#### Widgets
- `Display::drawWheel(x, y, radius, value, min, max, label)` - Rueda selectora
- `Display::drawSlider(x, y, width, value, min, max, label)` - Slider
- `Display::drawButton(x, y, w, h, label)` - Botón táctil
- `Display::drawCurrentBars(currents[4])` - Gráfico barras corriente
- `Display::drawVoltageGraph(voltage)` - Gráfico línea voltaje
- `Display::drawProgressBar(x, y, w, h, percent, label)` - Barra progreso

#### Alertas
- `Display::showAlert(level, msg)` - Alerta modal
- `Display::showToast(msg, duration)` - Notificación toast

#### Touch y Gestos
- `Display::detectGesture()` - Detecta gesto táctil
- `Display::isTouched()` - Verifica si hay toque activo
- `Display::getTouchCoordinates(&x, &y)` - Obtiene coordenadas
- `Display::isTouchInRect(x, y, rx, ry, rw, rh)` - Verifica toque en área

#### Utilidades
- `Display::clear(color)` - Limpia pantalla
- `Display::drawText(text, x, y, color, size)` - Dibuja texto
- `Display::drawCenteredText(text, y, color, size)` - Texto centrado
- `Display::updateVehicleData(data)` - Actualiza datos vehículo
- `Display::getTFT()` - Acceso directo a TFT_eSPI
- `Display::getWidth()` / `getHeight()` - Dimensiones pantalla
- `Display::requestRedraw()` - Fuerza redibujado completo

## Colores del Tema

```cpp
COLOR_BG         0x0000  // Negro (fondo principal)
COLOR_BG_DARK    0x1082  // Gris oscuro (paneles)
COLOR_ACCENT     0x07FF  // Cyan (elementos activos)
COLOR_TEXT       0xFFFF  // Blanco (texto principal)
COLOR_TEXT_DIM   0x7BEF  // Gris claro (texto secundario)
COLOR_OK         0x07E0  // Verde (estado OK)
COLOR_WARNING    0xFFE0  // Amarillo (advertencias)
COLOR_ERROR      0xF800  // Rojo (errores)
```

## Tipos y Estructuras

### MenuType (enum class)
- `DASHBOARD` - Dashboard principal
- `SETTINGS` - Configuración
- `CALIBRATION` - Calibración
- `HARDWARE_TEST` - Test hardware
- `WIFI_CONFIG` - Config WiFi/OTA
- `INA226_MONITOR` - Monitor INA226
- `STATISTICS` - Estadísticas
- `QUICK_MENU` - Menú rápido

### AlertLevel (enum class)
- `INFO` - Información
- `WARNING` - Advertencia
- `ERROR` - Error
- `CRITICAL` - Crítico

### GearPosition (enum class)
- `PARK` - Parking (P)
- `NEUTRAL` - Neutral (N)
- `REVERSE` - Reversa (R)
- `DRIVE1` - Drive 1 (D1)
- `DRIVE2` - Drive 2 (D2)

### GestureType (enum class)
- `TAP` - Toque simple
- `LONG_PRESS` - Mantener presionado
- `SWIPE_LEFT` - Deslizar izquierda
- `SWIPE_RIGHT` - Deslizar derecha
- `SWIPE_UP` - Deslizar arriba
- `SWIPE_DOWN` - Deslizar abajo

## Integración con Firmware Existente

Para integrar con tu firmware actual:

1. **Incluir Display.h** en tu código principal
2. **Llamar Display::init()** en setup()
3. **Llamar Display::update()** en loop()
4. **Actualizar datos** con Display::updateVehicleData()
5. **Usar widgets** según necesites
6. **Navegar menús** con Display::showMenu()

Ejemplo de integración:

```cpp
#include "Display.h"
#include "TuSistema.h"  // Tu firmware existente

void setup() {
    // Tu inicialización
    inicializarSensores();
    inicializarMotores();
    
    // Inicializar display
    Display::init();
}

void loop() {
    // Tu lógica existente
    procesarSensores();
    controlarMotores();
    
    // Actualizar datos para display
    VehicleData data = obtenerDatosVehiculo();
    Display::updateVehicleData(data);
    
    // Actualizar display (20+ FPS)
    Display::update();
}
```

## Optimización de Rendimiento

- **Update Rate**: 20-60 FPS (actualiza cada 50ms)
- **Actualización Selectiva**: Solo redibuja áreas que cambian
- **Flag needsRedraw**: Control manual de redibujado
- **Gestos Eficientes**: Detección sin bloqueo
- **Sin Delays**: Loop no bloqueante

## Calibración del Touch

Si las coordenadas táctiles no son precisas, ajusta los valores de mapeo en `display.cpp`:

```cpp
*x = map(p.x, 200, 3800, 0, screenWidth);  // Ajustar 200 y 3800
*y = map(p.y, 200, 3800, 0, screenHeight); // según tu display
```

Procedimiento:
1. Tocar esquinas conocidas
2. Anotar valores raw de p.x y p.y
3. Ajustar valores de mapeo
4. Recompilar y probar

## Resolución de Problemas

### El display no muestra nada
- ✓ Verificar conexiones físicas
- ✓ Revisar pines en `User_Setup.h` coincidan
- ✓ Asegurar display recibe 3.3V
- ✓ Verificar comunicación SPI con osciloscopio

### El touch no responde
- ✓ Verificar conexión T_CS (GPIO 22)
- ✓ Comprobar T_IRQ conectado (GPIO 46)
- ✓ Revisar pines SPI compartidos
- ✓ Calibrar coordenadas de mapeo

### Dashboard se ve mal
- ✓ Verificar rotación correcta (landscape=1)
- ✓ Comprobar resolución 480x320
- ✓ Ajustar colores si necesario

### Gestos no detectan
- ✓ Verificar touch funciona básicamente
- ✓ Ajustar thresholds en `updateGestureDetection()`
- ✓ Revisar tiempo de long press (3000ms)
- ✓ Verificar distancia mínima swipe (50px)

### Problemas de compilación
- ✓ Ejecutar `pio lib update`
- ✓ Limpiar: `pio run --target clean`
- ✓ Recompilar: `pio run`
- ✓ Verificar versiones de librerías en platformio.ini

## Características de Producción

✅ **Código Modular** - Separación clara de responsabilidades
✅ **Documentación Completa** - Comentarios en español
✅ **API Estática** - No requiere instanciación
✅ **Gestos Avanzados** - Sistema robusto de detección
✅ **Optimizado** - Actualización selectiva, 20+ FPS
✅ **Extensible** - Fácil añadir widgets/menús
✅ **Production-Ready** - Listo para integrar

## Desarrollo Futuro

Posibles mejoras:
- [ ] Gráficos más complejos (sprites, imágenes)
- [ ] Animaciones más elaboradas
- [ ] Soporte multi-idioma
- [ ] Temas personalizables
- [ ] Grabación de datos en SD
- [ ] Conectividad Bluetooth para app móvil

## Licencia

Este proyecto está disponible bajo licencia MIT.

## Autor

Marcos - Sistema de Display para Coche Eléctrico Inteligente

## Referencias

- [TFT_eSPI Library](https://github.com/Bodmer/TFT_eSPI)
- [XPT2046_Touchscreen Library](https://github.com/PaulStoffregen/XPT2046_Touchscreen)
- [ESP32-S3 Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/)
- [PlatformIO Documentation](https://docs.platformio.org/)
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
