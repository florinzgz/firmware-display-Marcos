#include <Arduino.h>
#include "Display.h"

// Simulación de datos del vehículo
float simulatedSpeed = 0.0f;
float simulatedRPM = 0.0f;
float simulatedBatteryVoltage = 12.5f;
float simulatedBatteryPercent = 85.0f;

// Temporizador para actualizar datos simulados
unsigned long lastDataUpdate = 0;
const unsigned long DATA_UPDATE_INTERVAL = 100;  // 100ms

void setup() {
    // Inicializar comunicación serial
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n========================================");
    Serial.println("SISTEMA DE DISPLAY - COCHE ELECTRICO");
    Serial.println("ESP32-S3 + ILI9488 + XPT2046");
    Serial.println("========================================\n");
    
    // Inicializar display
    Display::init();
    
    Serial.println("Sistema iniciado correctamente");
    Serial.println("Mostrando dashboard principal...\n");
    
    // Configurar menú inicial
    Display::showMenu(MenuType::DASHBOARD);
}

void loop() {
    unsigned long currentTime = millis();
    
    // Actualizar datos simulados periódicamente
    if (currentTime - lastDataUpdate >= DATA_UPDATE_INTERVAL) {
        lastDataUpdate = currentTime;
        
        // Simular variación de velocidad (oscila entre 0-20 km/h)
        static float speedTarget = 15.0f;
        simulatedSpeed += (speedTarget - simulatedSpeed) * 0.05f;
        
        // Cambiar objetivo cada 5 segundos
        static unsigned long lastSpeedChange = 0;
        if (currentTime - lastSpeedChange > 5000) {
            lastSpeedChange = currentTime;
            speedTarget = random(0, 21);
        }
        
        // Simular RPM proporcional a la velocidad
        simulatedRPM = simulatedSpeed * 150.0f + random(-100, 100);
        
        // Simular descarga de batería lenta
        simulatedBatteryPercent -= 0.001f;
        if (simulatedBatteryPercent < 20.0f) {
            simulatedBatteryPercent = 100.0f;
        }
        simulatedBatteryVoltage = 11.0f + (simulatedBatteryPercent / 100.0f) * 2.5f;
        
        // Actualizar estructura de datos del vehículo
        VehicleData data;
        data.speed = simulatedSpeed;
        data.rpm = simulatedRPM;
        data.odoTotal = 145.7f;
        data.odoTrip = 12.3f;
        data.batteryVoltage = simulatedBatteryVoltage;
        data.batteryPercent = simulatedBatteryPercent;
        
        // Simular temperaturas de motores (25-45°C con variación)
        for (int i = 0; i < 4; i++) {
            data.motorTemp[i] = 30.0f + simulatedSpeed * 0.5f + random(-5, 5);
        }
        
        // Simular corrientes de motores proporcionales a velocidad
        for (int i = 0; i < 4; i++) {
            data.motorCurrent[i] = simulatedSpeed * 0.3f + random(-10, 10) / 10.0f;
        }
        
        // Estado de marcha (cambiar con toque)
        data.gear = GearPosition::DRIVE1;
        
        // Estados del sistema
        data.status.lights = false;
        data.status.fourWheelDrive = true;
        data.status.parkingBrake = false;
        data.status.bluetooth = true;
        data.status.wifi = true;
        data.status.warnings = (simulatedBatteryPercent < 30.0f);
        
        // Actualizar display con nuevos datos
        Display::updateVehicleData(data);
        
        // Imprimir datos por serial cada segundo
        static unsigned long lastSerialPrint = 0;
        if (currentTime - lastSerialPrint > 1000) {
            lastSerialPrint = currentTime;
            Serial.printf("Velocidad: %.1f km/h | RPM: %.0f | Batería: %.1f%% (%.2fV)\n",
                         data.speed, data.rpm, data.batteryPercent, data.batteryVoltage);
        }
    }
    
    // Actualizar display (procesa touch y redibuja si es necesario)
    Display::update();
    
    // Pequeña pausa para no saturar CPU
    delay(10);
}
