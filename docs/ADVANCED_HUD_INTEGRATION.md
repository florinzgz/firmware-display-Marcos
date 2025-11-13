# Advanced HUD - Ejemplo de Integración

## Uso Básico

```cpp
#include <Arduino.h>
#include "AdvancedHUD.h"

// Datos del HUD
HUDData hudData;

void setup() {
    Serial.begin(115200);
    
    // Inicializar Advanced HUD
    AdvancedHUD::init();
    
    // Inicializar datos
    hudData.speed = 0.0f;
    hudData.rpm = 0.0f;
    hudData.batteryVoltage = 24.0f;
    hudData.batteryPercent = 100.0f;
    hudData.ambientTemp = 22.0f;
    hudData.pedalPosition = 0;
    hudData.gear = GearPosition::PARK;
    hudData.encoderAngle = 0;
    
    // Inicializar ruedas
    for (int i = 0; i < 4; i++) {
        WheelData* wheel = nullptr;
        if (i == 0) wheel = &hudData.wheelFL;
        else if (i == 1) wheel = &hudData.wheelFR;
        else if (i == 2) wheel = &hudData.wheelRL;
        else wheel = &hudData.wheelRR;
        
        wheel->effort = 0.0f;
        wheel->temperature = 25.0f;
        wheel->current = 0.0f;
        wheel->voltage = 24.0f;
        wheel->angle = 0;
        wheel->online = true;
    }
}

void loop() {
    // Actualizar datos desde sensores reales
    leerSensores();
    
    // Actualizar HUD
    AdvancedHUD::updateHUDData(hudData);
    AdvancedHUD::update();
    
    delay(10);
}

void leerSensores() {
    // Ejemplo de lectura de sensores INA226
    hudData.wheelFL.current = ina226_FL.readShuntCurrent();
    hudData.wheelFL.voltage = ina226_FL.readBusVoltage();
    hudData.wheelFL.effort = AdvancedHUD::calculateEffort(hudData.wheelFL.current);
    hudData.wheelFL.temperature = leerTemperaturaFL();
    hudData.wheelFL.online = ina226_FL.isConnected();
    
    // Repetir para FR, RL, RR...
    
    // Leer velocidad y RPM
    hudData.speed = calcularVelocidad();
    hudData.rpm = leerRPM();
    
    // Leer batería
    hudData.batteryVoltage = leerVoltajeBateria();
    hudData.batteryPercent = calcularPorcentajeBateria();
    
    // Leer pedal y encoder
    hudData.pedalPosition = leerPedal();
    hudData.encoderAngle = leerEncoder();  // -350 a +350 grados
    
    // Los ángulos de las ruedas vienen del firmware con geometría Ackermann calculada
    hudData.wheelFL.angle = getWheelAngleFL();  // Desde firmware/sistema de dirección
    hudData.wheelFR.angle = getWheelAngleFR();  // Desde firmware/sistema de dirección
}
```

## Integración con Sistema Existente

```cpp
#include "Display.h"
#include "AdvancedHUD.h"

enum DisplayMode {
    MODE_DASHBOARD,    // Dashboard original
    MODE_ADVANCED_HUD  // HUD avanzado
};

DisplayMode currentMode = MODE_ADVANCED_HUD;

void setup() {
    Serial.begin(115200);
    
    // Inicializar display base
    Display::init();
    
    // Inicializar modo actual
    if (currentMode == MODE_ADVANCED_HUD) {
        AdvancedHUD::init();
    } else {
        Display::showMenu(MenuType::DASHBOARD);
    }
}

void loop() {
    // Detectar cambio de modo (ejemplo: long press centro)
    if (detectarCambioModo()) {
        currentMode = (currentMode == MODE_DASHBOARD) ? 
                      MODE_ADVANCED_HUD : MODE_DASHBOARD;
        
        if (currentMode == MODE_ADVANCED_HUD) {
            AdvancedHUD::init();
        } else {
            Display::clear();
            Display::showMenu(MenuType::DASHBOARD);
        }
    }
    
    // Actualizar modo activo
    if (currentMode == MODE_ADVANCED_HUD) {
        HUDData hudData = leerDatosHUD();
        AdvancedHUD::updateHUDData(hudData);
        AdvancedHUD::update();
    } else {
        VehicleData vehicleData = leerDatosVehiculo();
        Display::updateVehicleData(vehicleData);
        Display::update();
    }
}
```

## Optimización de Lecturas

```cpp
void loop() {
    static unsigned long lastUpdate50 = 0;   // 50ms - Pedal, RPM
    static unsigned long lastUpdate100 = 0;  // 100ms - Velocidad, ruedas
    static unsigned long lastUpdate500 = 0;  // 500ms - Temperatura, batería
    
    unsigned long now = millis();
    
    // Actualización rápida (20 Hz)
    if (now - lastUpdate50 >= 50) {
        lastUpdate50 = now;
        hudData.pedalPosition = readPedalADC();
        hudData.rpm = calculateRPMFromEncoder();
        hudData.encoderAngle = readSteeringEncoder();
    }
    
    // Actualización media (10 Hz)
    if (now - lastUpdate100 >= 100) {
        lastUpdate100 = now;
        hudData.speed = calculateSpeedFromWheels();
        
        // Leer 4 canales INA226
        readINA226Channels();
    }
    
    // Actualización lenta (2 Hz)
    if (now - lastUpdate500 >= 500) {
        lastUpdate500 = now;
        hudData.batteryVoltage = analogReadFiltered(BATTERY_PIN);
        hudData.batteryPercent = mapBatteryPercent(hudData.batteryVoltage);
        hudData.ambientTemp = readDS18B20();
    }
    
    // Actualizar HUD (20 FPS)
    AdvancedHUD::updateHUDData(hudData);
    AdvancedHUD::update();
}

void readINA226Channels() {
    // FL
    if (ina226_FL.begin()) {
        hudData.wheelFL.voltage = ina226_FL.getBusVoltage_V();
        hudData.wheelFL.current = ina226_FL.getCurrent_mA() / 1000.0f;
        hudData.wheelFL.effort = AdvancedHUD::calculateEffort(hudData.wheelFL.current);
        hudData.wheelFL.online = true;
    } else {
        hudData.wheelFL.online = false;
    }
    
    // FR
    if (ina226_FR.begin()) {
        hudData.wheelFR.voltage = ina226_FR.getBusVoltage_V();
        hudData.wheelFR.current = ina226_FR.getCurrent_mA() / 1000.0f;
        hudData.wheelFR.effort = AdvancedHUD::calculateEffort(hudData.wheelFR.current);
        hudData.wheelFR.online = true;
    } else {
        hudData.wheelFR.online = false;
    }
    
    // RL, RR similar...
}
```

## Calibración de Sensores

```cpp
// Calibración INA226
void calibrarINA226() {
    float shuntResistor = 0.01;  // 10 mΩ
    float maxCurrent = 15.0;     // 15A máximo
    
    ina226_FL.setShuntResistor(shuntResistor);
    ina226_FL.setMaxCurrent(maxCurrent);
    ina226_FL.setAverage(INA226_AVERAGES_16);
    ina226_FL.setBusVoltageConversionTime(INA226_BUS_CONV_TIME_1100US);
    ina226_FL.setShuntVoltageConversionTime(INA226_SHUNT_CONV_TIME_1100US);
    
    // Repetir para FR, RL, RR
}

// Calibración encoder de dirección
void calibrarEncoder() {
    // Centrar volante manualmente
    Serial.println("Centra el volante y presiona ENTER");
    while (!Serial.available()) delay(100);
    
    int16_t centerValue = analogRead(ENCODER_PIN);
    
    // Girar a tope izquierda
    Serial.println("Gira a tope IZQUIERDA y presiona ENTER");
    while (!Serial.available()) delay(100);
    
    int16_t leftValue = analogRead(ENCODER_PIN);
    
    // Girar a tope derecha
    Serial.println("Gira a tope DERECHA y presiona ENTER");
    while (!Serial.available()) delay(100);
    
    int16_t rightValue = analogRead(ENCODER_PIN);
    
    // Guardar en EEPROM
    saveEncoderCalibration(centerValue, leftValue, rightValue);
}

// Mapeo de encoder a ángulo
int16_t readSteeringEncoder() {
    int16_t raw = analogRead(ENCODER_PIN);
    
    // Mapear a -350° / +350° (rango completo del volante)
    if (raw < encoderCenter) {
        return map(raw, encoderLeft, encoderCenter, -350, 0);
    } else {
        return map(raw, encoderCenter, encoderRight, 0, 350);
    }
}
```

## Filtrado y Suavizado

```cpp
// Filtro de media móvil para temperatura
class MovingAverage {
private:
    float values[10];
    int index = 0;
    int count = 0;
    
public:
    void add(float value) {
        values[index] = value;
        index = (index + 1) % 10;
        if (count < 10) count++;
    }
    
    float get() {
        float sum = 0;
        for (int i = 0; i < count; i++) {
            sum += values[i];
        }
        return sum / count;
    }
};

MovingAverage tempFL, tempFR, tempRL, tempRR;

void leerTemperaturas() {
    tempFL.add(readTempSensorFL());
    tempFR.add(readTempSensorFR());
    tempRL.add(readTempSensorRL());
    tempRR.add(readTempSensorRR());
    
    hudData.wheelFL.temperature = tempFL.get();
    hudData.wheelFR.temperature = tempFR.get();
    hudData.wheelRL.temperature = tempRL.get();
    hudData.wheelRR.temperature = tempRR.get();
}

// Filtro exponencial para corriente (más responsive)
float exponentialFilter(float newValue, float oldValue, float alpha) {
    return alpha * newValue + (1 - alpha) * oldValue;
}

void leerCorrientes() {
    static float currentFL = 0, currentFR = 0, currentRL = 0, currentRR = 0;
    
    currentFL = exponentialFilter(ina226_FL.getCurrent_mA() / 1000.0f, currentFL, 0.3f);
    currentFR = exponentialFilter(ina226_FR.getCurrent_mA() / 1000.0f, currentFR, 0.3f);
    currentRL = exponentialFilter(ina226_RL.getCurrent_mA() / 1000.0f, currentRL, 0.3f);
    currentRR = exponentialFilter(ina226_RR.getCurrent_mA() / 1000.0f, currentRR, 0.3f);
    
    hudData.wheelFL.current = currentFL;
    hudData.wheelFR.current = currentFR;
    hudData.wheelRL.current = currentRL;
    hudData.wheelRR.current = currentRR;
}
```

## Detección de Errores

```cpp
void verificarSensores() {
    // Verificar INA226
    if (!hudData.wheelFL.online) {
        Serial.println("ERROR: INA226 FL offline");
        // Mostrar advertencia en HUD
    }
    
    // Verificar temperaturas fuera de rango
    if (hudData.wheelFL.temperature > 70.0f) {
        Serial.println("ALERTA: Temperatura FL crítica!");
        // Reducir potencia motor FL
        // Mostrar alerta en HUD
    }
    
    // Verificar voltaje batería
    if (hudData.batteryVoltage < 20.0f) {
        Serial.println("ALERTA: Batería baja!");
        // Limitar velocidad
    }
}
```
