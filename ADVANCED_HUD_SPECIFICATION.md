# Advanced HUD Specification - Mercedes Theme Display

## Visión General

Sistema de visualización avanzado tipo HUD (Head-Up Display) con tema Mercedes para coche eléctrico infantil. Muestra vista cenital del vehículo con datos en tiempo real de todos los componentes.

## 🎨 Diseño Visual Completo

### Layout Principal (480x320 px)

```
┌─────────────────────────────────────────────────────────────────────────────┐
│ Temp: 23°C                                            Bat: 24.5V 95%        │
├─────────────────────────────────────────────────────────────────────────────┤
│                                                                              │
│  ┌──────────┐      ┌──────────────────────────────────────────┐  ┌──────────┐
│  │  km/h    │      │  Vista Cenital Coche Mercedes            │  │   RPM    │
│  │   25     │      │                                           │  │   220    │
│  │  ┌───┐  │      │      ┌─────┐         ┌─────┐            │  │  ┌───┐  │
│  │ ╱  ●  ╲ │      │      │ FL  │─────────│ FR  │            │  │ ╱     ╲ │
│  │╱       ╲│      │      │85%░░│  ╔═╗    │90%░░│            │  │╱   ●   ╲│
│  └──────────┘      │      │35°C │  ║☆║    │32°C │            │  └──────────┘
│                    │      └──┬──┘  ╚═╝    └──┬──┘            │
│                    │         │               │                │
│                    │         │    Encoder    │                │
│                    │         │      45°      │                │
│                    │         │               │                │
│                    │      ┌──┴──┐         ┌──┴──┐            │
│                    │      │ RL  │─────────│ RR  │            │
│                    │      │78%░░│         │82%░░│            │
│                    │      │38°C │         │36°C │            │
│                    │      └─────┘         └─────┘            │
│                    └──────────────────────────────────────────┘
│                                                                              │
│        [ P | D2 | D1 | N | R ]         Pedal: ████████░░ 80%               │
└─────────────────────────────────────────────────────────────────────────────┘
```

### Elementos Principales

#### 1. Vista Cenital del Coche (Centrada, 240x200px)

**Posición**: X=170, Y=40 (centrado en pantalla)

**Componentes**:
- Silueta del vehículo estilo Mercedes (vista desde arriba)
- Logo Mercedes 3D en el techo (estrella de tres puntas)
- 4 ruedas con indicadores individuales
- Enlaces de dirección (líneas conectando ruedas delanteras)
- Volante central mostrando ángulo del encoder

**Ruedas (FL, FR, RL, RR)**:
- Tamaño: 50x40 px cada una
- Información por rueda:
  - % de esfuerzo (0-100%)
  - Temperatura (°C)
  - Barra de esfuerzo con colores dinámicos
  - Ángulo de dirección (solo delanteras)

**Colores Dinámicos por Esfuerzo/Temperatura**:
```
Verde:    0-25%  / <35°C   (0x07E0)
Amarillo: 26-50% / 35-45°C (0xFFE0)
Naranja:  51-75% / 46-55°C (0xFD20)
Rojo:     76-100%/ >55°C    (0xF800)
```

#### 2. Velocímetro (Izquierda del Coche, R=60px)

**Posición**: X=80, Y=160 (a la izquierda del coche centrado)

**Características**:
- Gauge circular 0-30 km/h
- Aguja roja con sombra
- Marcas cada 5 km/h
- Valor digital grande en centro
- Fondo oscuro con borde rojo
- Recorrido 270° (-135° a +135°)

#### 3. Tacómetro/RPM (Derecha del Coche, R=60px)

**Posición**: X=400, Y=160 (a la derecha del coche centrado)

**Características**:
- Gauge circular 0-220 RPM
- Aguja verde
- Zona roja >180 RPM
- Valor digital en centro
- Marcas distribuidas
- Recorrido 270° (-135° a +135°)

#### 4. Batería (Superior Derecha)

**Posición**: X=360, Y=5

**Formato**: `24.5V 95%`
- Voltaje dinámico (20-28V)
- Porcentaje calculado
- Color según nivel:
  - Verde: >60%
  - Amarillo: 30-60%
  - Rojo: <30%

#### 5. Temperatura Ambiente (Superior Izquierda)

**Posición**: X=10, Y=5

**Formato**: `Temp: 23°C`
- Sensor ambiente
- Color fijo blanco

#### 6. Shifter/Marcha (Inferior Centro)

**Posición**: X=100, Y=290

**Estados**: `[ P | D2 | D1 | N | R ]`
- Marcha actual iluminada en cyan brillante
- Otras marchas en gris oscuro
- Separadores verticales `|`
- Fondo oscuro con borde

#### 7. Pedal (Inferior Derecha)

**Posición**: X=280, Y=290

**Formato**: `Pedal: ████████░░ 80%`
- Barra horizontal 0-100%
- Colores dinámicos:
  - Azul: 0-33% (suave)
  - Verde: 34-66% (medio)
  - Rojo: 67-100% (máximo)
- Porcentaje numérico al final

## 📊 Datos en Tiempo Real

### Estructura de Datos

```cpp
struct WheelData {
    float effort;        // % esfuerzo (0-100)
    float temperature;   // Temperatura en °C
    float current;       // Corriente INA226 en A
    float voltage;       // Voltaje INA226 en V
    int16_t angle;       // Ángulo dirección (solo FL/FR)
    bool online;         // INA226 online/offline
};

struct HUDData {
    // Ruedas
    WheelData wheelFL;   // Front Left
    WheelData wheelFR;   // Front Right
    WheelData wheelRL;   // Rear Left
    WheelData wheelRR;   // Rear Right
    
    // Vehículo
    float speed;         // km/h (0-30)
    float rpm;           // RPM (0-220)
    float batteryVoltage;// V (20-28)
    float batteryPercent;// % (0-100)
    float ambientTemp;   // °C
    int8_t pedalPosition;// % (0-100)
    GearPosition gear;   // P/D2/D1/N/R
    
    // Encoder
    int16_t encoderAngle;// Ángulo volante (-350 a +350, 700° total)
};
```

### Cálculo de Esfuerzos

```cpp
// Esfuerzo = (Corriente / Corriente_Max) * 100
// Corriente_Max = 10A por motor
float calculateEffort(float current) {
    return constrain((current / 10.0f) * 100.0f, 0.0f, 100.0f);
}

// Temperatura calculada desde INA226 o sensor dedicado
// Color según temperatura
uint16_t getTemperatureColor(float temp) {
    if (temp < 35.0f) return 0x07E0;      // Verde
    if (temp < 45.0f) return 0xFFE0;      // Amarillo
    if (temp < 55.0f) return 0xFD20;      // Naranja
    return 0xF800;                         // Rojo
}
```

## 💻 Código de Integración

### Clase AdvancedHUD

```cpp
class AdvancedHUD {
public:
    static void init();
    static void update();
    static void updateHUDData(const HUDData& data);
    
private:
    static void drawTopDownCar();
    static void drawWheel(int16_t x, int16_t y, const WheelData& wheel, 
                         const char* label, bool isFront);
    static void drawMercedesLogo(int16_t x, int16_t y, int16_t size);
    static void drawSteeringWheel(int16_t x, int16_t y, int16_t angle);
    static void drawSteeringLinks();
    static void drawSpeedometerHUD(float speed);
    static void drawTachometerHUD(float rpm);
    static void drawBatteryHUD(float voltage, float percent);
    static void drawShifter(GearPosition gear);
    static void drawPedalBar(int8_t position);
    
    static HUDData hudData;
};
```

### Actualización Optimizada

```cpp
// Diferentes tasas de actualización para eficiencia

void loop() {
    static unsigned long lastUpdate50 = 0;   // 50ms - Pedal, RPM
    static unsigned long lastUpdate100 = 0;  // 100ms - Velocidad, ruedas
    static unsigned long lastUpdate500 = 0;  // 500ms - Temperatura, batería
    
    unsigned long now = millis();
    
    // Actualización rápida (20 Hz) - Controles críticos
    if (now - lastUpdate50 >= 50) {
        lastUpdate50 = now;
        hudData.pedalPosition = readPedalPosition();
        hudData.rpm = readRPM();
        hudData.encoderAngle = readEncoderAngle();
    }
    
    // Actualización media (10 Hz) - Dinámica del vehículo
    if (now - lastUpdate100 >= 100) {
        lastUpdate100 = now;
        hudData.speed = readSpeed();
        updateWheelData();  // INA226 de 4 ruedas
    }
    
    // Actualización lenta (2 Hz) - Valores lentos
    if (now - lastUpdate500 >= 500) {
        lastUpdate500 = now;
        hudData.batteryVoltage = readBatteryVoltage();
        hudData.batteryPercent = calculateBatteryPercent();
        hudData.ambientTemp = readAmbientTemperature();
    }
    
    AdvancedHUD::updateHUDData(hudData);
    AdvancedHUD::update();
}
```

### Lectura INA226 (4 Canales)

```cpp
void updateWheelData() {
    // INA226 para cada rueda
    hudData.wheelFL.voltage = ina226_FL.readBusVoltage();
    hudData.wheelFL.current = ina226_FL.readShuntCurrent();
    hudData.wheelFL.effort = calculateEffort(hudData.wheelFL.current);
    hudData.wheelFL.temperature = readTempFL();  // Sensor temp dedicado
    hudData.wheelFL.online = ina226_FL.isOnline();
    
    hudData.wheelFR.voltage = ina226_FR.readBusVoltage();
    hudData.wheelFR.current = ina226_FR.readShuntCurrent();
    hudData.wheelFR.effort = calculateEffort(hudData.wheelFR.current);
    hudData.wheelFR.temperature = readTempFR();
    hudData.wheelFR.online = ina226_FR.isOnline();
    
    // Repetir para RL y RR
    // ...
    
    // Ángulos de dirección (solo ruedas delanteras)
    // Los ángulos de las ruedas vienen del firmware con Ackermann calculado
    hudData.wheelFL.angle = getWheelAngleFL();  // Desde firmware
    hudData.wheelFR.angle = getWheelAngleFR();  // Desde firmware
}
```

## 📐 Layout Exacto 480x320 px

### Coordenadas Precisas

```
Temperatura Ambiente:   X=10,  Y=5,   W=100, H=15
Batería:                X=360, Y=5,   W=110, H=15

Vista Cenital Coche:    X=20,  Y=40,  W=240, H=200
  - Logo Mercedes:      X=140, Y=100, R=15
  - Volante:            X=140, Y=140, R=20
  - Rueda FL:           X=60,  Y=60,  W=50,  H=40
  - Rueda FR:           X=210, Y=60,  W=50,  H=40
  - Rueda RL:           X=60,  Y=180, W=50,  H=40
  - Rueda RR:           X=210, Y=180, W=50,  H=40

Velocímetro:            X=340, Y=40,  R=60
Tacómetro:              X=340, Y=170, R=60

Shifter:                X=100, Y=290, W=160, H=25
Pedal:                  X=280, Y=290, W=180, H=25
```

### Dimensiones Componentes

- **Rueda Individual**: 50x40 px
  - Rectángulo redondeado
  - Barra esfuerzo interior: 46x8 px
  - Texto label: Font size 1
  - Texto datos: Font size 1
  
- **Gauges (Velocímetro/Tacómetro)**: Radio 60 px
  - Círculo exterior: R=60
  - Marcas: R=55 a R=60
  - Aguja: R=50
  - Texto central: Font size 3
  
- **Logo Mercedes**: Radio 15 px
  - Estrella de 3 puntas
  - Color cyan/plata
  
- **Volante**: Radio 20 px
  - Círculo con línea indicadora
  - Rota según encoder
  
- **Shifter**: 160x25 px
  - 5 posiciones (P/D2/D1/N/R)
  - Ancho por posición: 32 px
  
- **Pedal Bar**: 180x25 px
  - Barra progreso: 120x15 px
  - Texto porcentaje: 50 px

## 🎨 Tema Visual

### Colores Específicos Mercedes

```cpp
// Mercedes Theme Colors
#define COLOR_MERCEDES_SILVER  0xC618  // Plata Mercedes
#define COLOR_MERCEDES_BLUE    0x0356  // Azul Mercedes oscuro
#define COLOR_STAR_GOLD        0xFEA0  // Dorado estrella
#define COLOR_GAUGE_RED        0xF800  // Rojo aguja
#define COLOR_GAUGE_GREEN      0x07E0  // Verde aguja
#define COLOR_HUD_BG           0x0000  // Negro
#define COLOR_HUD_PANEL        0x18C3  // Gris panel
#define COLOR_HUD_ACCENT       0x07FF  // Cyan accent
```

### Estilo

- Fondo negro puro para contraste HUD
- Paneles con gris muy oscuro
- Gauges con fondos transparentes/oscuros
- Agujas con sombras para profundidad
- Texto con anti-aliasing si disponible
- Bordes sutiles en cyan
- Iconos minimalistas estilo Mercedes

## ⚡ Optimización

### Rendimiento

- **Tasa de actualización global**: 20 FPS (50ms)
- **Actualización selectiva por zona**:
  - Ruedas: Solo si cambio >2% effort o >1°C temp
  - Velocímetro: Solo si cambio >0.5 km/h
  - Tacómetro: Solo si cambio >5 RPM
  - Pedal: Cada frame (muy dinámico)
  - Shifter: Solo al cambiar marcha
  - Batería: Cada 500ms
  
### Memoria

- Buffer doble para zona central (vista coche)
- Sprites pre-renderizados para:
  - Logo Mercedes
  - Iconos de marcha
  - Números grandes
  
### CPU

- Uso de lookup tables para:
  - Conversión ángulos a coordenadas
  - Mapeo de colores
- DMA SPI para transferencias grandes
- Cálculos en punto fijo cuando sea posible

## 🔄 Integración

### Con Sistema Actual

```cpp
// En main.cpp
#include "Display.h"
#include "AdvancedHUD.h"

bool useAdvancedHUD = true;  // Toggle entre HUD y Dashboard

void setup() {
    if (useAdvancedHUD) {
        AdvancedHUD::init();
    } else {
        Display::init();
    }
}

void loop() {
    if (useAdvancedHUD) {
        // Leer sensores y actualizar
        HUDData data = readAllSensors();
        AdvancedHUD::updateHUDData(data);
        AdvancedHUD::update();
    } else {
        // Sistema dashboard original
        VehicleData data = readVehicleData();
        Display::updateVehicleData(data);
        Display::update();
    }
}
```

### Transición entre Modos

```cpp
// Cambio con long press en zona específica
if (gesture == LONG_PRESS && x > 200 && x < 280 && y > 140 && y < 180) {
    useAdvancedHUD = !useAdvancedHUD;
    // Reinicializar display en nuevo modo
    if (useAdvancedHUD) {
        AdvancedHUD::init();
    } else {
        Display::init();
        Display::showMenu(MenuType::DASHBOARD);
    }
}
```

## 📝 Notas de Implementación

1. **Logo Mercedes**: Dibujar con primitivas (líneas, triángulos) para estrella 3D
2. **Enlaces de Dirección**: Líneas desde volante a ruedas delanteras (ángulos desde firmware)
3. **Rotación ruedas**: Indicador visual de dirección solo en FL/FR
4. **INA226**: Polling cada 100ms, cache de valores para evitar lecturas I2C excesivas
5. **Temperatura**: Sensores dedicados o estimación desde corriente/tiempo
6. **Encoder**: Rango completo -350° a +350° (700° giro total)
7. **Pedal**: ADC con oversampling para suavizado

## 🚀 Estado de Implementación

- [ ] Estructura HUDData y WheelData
- [ ] Clase AdvancedHUD base
- [ ] Vista cenital del coche
- [ ] Logo Mercedes 3D
- [ ] 4 ruedas con datos individuales
- [ ] Volante con encoder
- [ ] Enlaces de dirección
- [ ] Velocímetro HUD
- [ ] Tacómetro HUD
- [ ] Batería HUD
- [ ] Shifter iluminado
- [ ] Barra de pedal
- [ ] Integración INA226
- [ ] Optimización actualización
- [ ] Documentación completa
