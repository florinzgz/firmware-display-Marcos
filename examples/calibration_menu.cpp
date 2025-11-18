/**
 * Ejemplo de uso del menú de calibración
 * 
 * Este ejemplo muestra cómo usar la clase CalibrationMenu
 * independientemente del display del coche.
 */

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "CalibrationMenu.h"

#define ENCODER_A 26
#define ENCODER_B 27
#define ENCODER_BTN 32

TFT_eSPI tft = TFT_eSPI();
CalibrationMenu menu(&tft);

volatile int encoderPos = 0;
volatile int lastEncoded = 0;
volatile bool buttonPressed = false;

void IRAM_ATTR encoderISR() {
    int MSB = digitalRead(ENCODER_A);
    int LSB = digitalRead(ENCODER_B);
    
    int encoded = (MSB << 1) | LSB;
    int sum = (lastEncoded << 2) | encoded;
    
    if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
        encoderPos++;
    }
    if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
        encoderPos--;
    }
    
    lastEncoded = encoded;
}

void IRAM_ATTR buttonISR() {
    buttonPressed = true;
}

void setup() {
    Serial.begin(115200);
    
    // Inicializar TFT
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    
    // Configurar encoder
    pinMode(ENCODER_A, INPUT_PULLUP);
    pinMode(ENCODER_B, INPUT_PULLUP);
    pinMode(ENCODER_BTN, INPUT_PULLUP);
    
    attachInterrupt(digitalPinToInterrupt(ENCODER_A), encoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_B), encoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_BTN), buttonISR, FALLING);
    
    // Inicializar menú
    menu.begin();
    menu.show();
    
    // Configurar valores de ejemplo
    menu.setSensorValue(SENSOR_STEERING, 2048);
    menu.setSensorValue(SENSOR_ACCELERATOR, 1024);
    menu.setSensorValue(SENSOR_BRAKE, 512);
    menu.setSensorValue(SENSOR_BATTERY, 75);
}

void loop() {
    static int lastEncoderPos = 0;
    
    // Manejar encoder
    if (encoderPos != lastEncoderPos) {
        int delta = encoderPos - lastEncoderPos;
        menu.handleEncoder(delta);
        lastEncoderPos = encoderPos;
        Serial.printf("Encoder: %d\n", encoderPos);
    }
    
    // Manejar botón
    if (buttonPressed) {
        buttonPressed = false;
        menu.handleButton();
        Serial.println("Button pressed");
    }
    
    // Simular actualización de sensores
    static unsigned long lastUpdate = 0;
    if (millis() - lastUpdate > 500) {
        lastUpdate = millis();
        
        // Simular lectura de sensores
        int steering = random(0, 4096);
        int accelerator = random(0, 4096);
        int brake = random(0, 4096);
        int battery = random(50, 100);
        
        menu.setSensorValue(SENSOR_STEERING, steering);
        menu.setSensorValue(SENSOR_ACCELERATOR, accelerator);
        menu.setSensorValue(SENSOR_BRAKE, brake);
        menu.setSensorValue(SENSOR_BATTERY, battery);
        
        menu.update();
    }
    
    delay(10);
}
