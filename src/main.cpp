#include <Arduino.h>
#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "CarDisplay.h"
#include "CalibrationMenu.h"

// Pin definitions
#define TOUCH_CS 33
#define TOUCH_IRQ 25
#define ENCODER_A 26
#define ENCODER_B 27
#define ENCODER_BTN 32

// Analog input pins for sensors
#define STEERING_PIN 34
#define ACCELERATOR_PIN 35
#define BRAKE_PIN 36
#define BATTERY_PIN 39

// TFT and Touch objects
TFT_eSPI tft = TFT_eSPI();
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);

// Application objects
CarDisplay carDisplay(&tft);
CalibrationMenu calibMenu(&tft);

// State variables
volatile int encoderPos = 0;
volatile int lastEncoded = 0;
volatile bool buttonPressed = false;
unsigned long lastButtonPress = 0;
unsigned long lastUpdate = 0;
float wheelRotationAngle = 0.0;

// Touch gesture detection
bool touchActive = false;
unsigned long touchStartTime = 0;
int touchStartX = 0;
int touchStartY = 0;

// Function prototypes
void IRAM_ATTR encoderISR();
void IRAM_ATTR buttonISR();
void updateSensors();
void handleTouch();
void setup();
void loop();

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
    unsigned long currentTime = millis();
    if (currentTime - lastButtonPress > 200) { // Debounce
        buttonPressed = true;
        lastButtonPress = currentTime;
    }
}

void updateSensors() {
    // Read analog sensors
    int steeringRaw = analogRead(STEERING_PIN);
    int acceleratorRaw = analogRead(ACCELERATOR_PIN);
    int brakeRaw = analogRead(BRAKE_PIN);
    int batteryRaw = analogRead(BATTERY_PIN);
    
    // Update calibration menu with raw values
    calibMenu.setSensorValue(SENSOR_STEERING, steeringRaw);
    calibMenu.setSensorValue(SENSOR_ACCELERATOR, acceleratorRaw);
    calibMenu.setSensorValue(SENSOR_BRAKE, brakeRaw);
    
    // Map battery voltage (assuming 0-4.2V range mapped to 0-4095)
    float batteryVoltage = (batteryRaw / 4095.0) * 4.2;
    int batteryPercent = map(batteryVoltage * 100, 300, 420, 0, 100);
    if (batteryPercent < 0) batteryPercent = 0;
    if (batteryPercent > 100) batteryPercent = 100;
    calibMenu.setSensorValue(SENSOR_BATTERY, batteryPercent);
    
    // Only update car display if sensors are enabled
    if (calibMenu.isSensorEnabled(SENSOR_STEERING)) {
        // Map steering sensor to -45 to 45 degrees
        int minVal = calibMenu.getSensorMin(SENSOR_STEERING);
        int maxVal = calibMenu.getSensorMax(SENSOR_STEERING);
        float steeringAngle = map(steeringRaw, minVal, maxVal, -45, 45);
        carDisplay.setWheelAngle(steeringAngle);
    }
    
    if (calibMenu.isSensorEnabled(SENSOR_ACCELERATOR)) {
        // Map accelerator to 0.0-1.0
        int minVal = calibMenu.getSensorMin(SENSOR_ACCELERATOR);
        int maxVal = calibMenu.getSensorMax(SENSOR_ACCELERATOR);
        float pedalPos = (float)(acceleratorRaw - minVal) / (maxVal - minVal);
        carDisplay.setPedalPosition(pedalPos);
    }
    
    if (calibMenu.isSensorEnabled(SENSOR_BATTERY)) {
        carDisplay.setBatteryLevel(batteryPercent / 100.0);
    }
}

void handleTouch() {
    if (touch.touched()) {
        TS_Point p = touch.getPoint();
        
        // Map touch coordinates to screen coordinates
        int x = map(p.x, 200, 3700, 0, 320);
        int y = map(p.y, 200, 3700, 0, 480);
        
        if (!touchActive) {
            // Touch started
            touchActive = true;
            touchStartTime = millis();
            touchStartX = x;
            touchStartY = y;
        }
    } else {
        if (touchActive) {
            // Touch ended
            unsigned long touchDuration = millis() - touchStartTime;
            
            // Long press in top-left corner (50x50 area) toggles menu
            if (touchStartX < 50 && touchStartY < 50 && touchDuration > 1000) {
                calibMenu.toggle();
            }
            
            touchActive = false;
        }
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("ESP32-S3 Car Display Initializing...");
    
    // Initialize TFT
    tft.init();
    tft.setRotation(0);  // Portrait mode
    tft.fillScreen(TFT_BLACK);
    
    // Display startup message
    tft.setTextColor(TFT_WHITE);
    tft.setTextDatum(MC_DATUM);
    tft.drawString("Initializing...", 160, 240, 4);
    
    // Initialize touch
    touch.begin();
    touch.setRotation(0);
    
    Serial.println("Touch initialized");
    
    // Configure encoder pins
    pinMode(ENCODER_A, INPUT_PULLUP);
    pinMode(ENCODER_B, INPUT_PULLUP);
    pinMode(ENCODER_BTN, INPUT_PULLUP);
    
    // Attach interrupts
    attachInterrupt(digitalPinToInterrupt(ENCODER_A), encoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_B), encoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(ENCODER_BTN), buttonISR, FALLING);
    
    Serial.println("Encoder initialized");
    
    // Configure analog input pins
    pinMode(STEERING_PIN, INPUT);
    pinMode(ACCELERATOR_PIN, INPUT);
    pinMode(BRAKE_PIN, INPUT);
    pinMode(BATTERY_PIN, INPUT);
    
    // Set ADC resolution
    analogReadResolution(12);  // 12-bit ADC (0-4095)
    
    Serial.println("Sensors initialized");
    
    // Initialize car display
    carDisplay.begin();
    
    // Initialize calibration menu
    calibMenu.begin();
    
    Serial.println("Display components initialized");
    
    delay(1000);
    
    // Initial display update
    carDisplay.update();
    
    Serial.println("Initialization complete!");
}

void loop() {
    unsigned long currentTime = millis();
    
    // Handle encoder for menu navigation
    static int lastEncoderPos = 0;
    if (encoderPos != lastEncoderPos) {
        int delta = encoderPos - lastEncoderPos;
        calibMenu.handleEncoder(delta);
        lastEncoderPos = encoderPos;
    }
    
    // Handle encoder button
    if (buttonPressed) {
        buttonPressed = false;
        calibMenu.handleButton();
    }
    
    // Handle touch input
    handleTouch();
    
    // Update sensor readings
    updateSensors();
    
    // Animate wheel rotation based on accelerator
    if (calibMenu.isSensorEnabled(SENSOR_ACCELERATOR)) {
        int accelRaw = analogRead(ACCELERATOR_PIN);
        int minVal = calibMenu.getSensorMin(SENSOR_ACCELERATOR);
        int maxVal = calibMenu.getSensorMax(SENSOR_ACCELERATOR);
        float accelPercent = (float)(accelRaw - minVal) / (maxVal - minVal);
        if (accelPercent > 0.1) {
            wheelRotationAngle += accelPercent * 0.2;
            if (wheelRotationAngle > 2 * PI) wheelRotationAngle -= 2 * PI;
            carDisplay.setWheelRotation(wheelRotationAngle);
        }
    }
    
    // Update display at 10Hz
    if (currentTime - lastUpdate > 100) {
        lastUpdate = currentTime;
        
        if (calibMenu.isVisible()) {
            calibMenu.update();
        } else {
            carDisplay.update();
        }
    }
    
    delay(10);  // Small delay to prevent excessive CPU usage
}
