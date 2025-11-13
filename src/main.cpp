#include <Arduino.h>
#include "Display.h"

// Global display object
Display display;

// Variables for touch coordinate tracking
int16_t lastTouchX = -1;
int16_t lastTouchY = -1;
unsigned long lastTouchTime = 0;
const unsigned long TOUCH_DEBOUNCE = 200;  // ms

void setup() {
    // Initialize serial communication
    Serial.begin(115200);
    delay(1000);  // Wait for serial to initialize
    
    Serial.println("\n=================================");
    Serial.println("ESP32-S3 TFT Display + Touch Demo");
    Serial.println("=================================\n");
    
    // Initialize display
    if (!display.init()) {
        Serial.println("ERROR: Display initialization failed!");
        while (1) {
            delay(1000);
        }
    }
    
    // Clear screen and show welcome message
    display.clear(TFT_BLACK);
    
    // Draw title
    display.drawText("ESP32-S3 Display Demo", 10, 10, TFT_CYAN, 3);
    
    // Draw device info
    display.drawText("Device: ESP32-S3-DevKitC-1", 10, 50, TFT_WHITE, 2);
    display.drawText("Display: ILI9488 (480x320)", 10, 75, TFT_WHITE, 2);
    display.drawText("Touch: XPT2046", 10, 100, TFT_WHITE, 2);
    
    // Draw instruction
    display.drawText("Touch the screen to see coordinates", 10, 140, TFT_YELLOW, 2);
    
    // Draw separator line
    display.getTFT().drawLine(0, 170, display.getWidth(), 170, TFT_GREEN);
    
    // Touch area label
    display.drawText("Touch Coordinates:", 10, 180, TFT_GREEN, 2);
    
    Serial.println("Setup complete. Touch the screen!");
}

void loop() {
    // Check for touch input
    if (display.isTouched()) {
        unsigned long currentTime = millis();
        
        // Debounce touch input
        if (currentTime - lastTouchTime > TOUCH_DEBOUNCE) {
            int16_t x, y;
            
            if (display.getTouchCoordinates(&x, &y)) {
                // Only update if coordinates changed significantly
                if (abs(x - lastTouchX) > 5 || abs(y - lastTouchY) > 5) {
                    lastTouchX = x;
                    lastTouchY = y;
                    lastTouchTime = currentTime;
                    
                    // Clear previous coordinates
                    display.getTFT().fillRect(10, 210, 460, 100, TFT_BLACK);
                    
                    // Display new coordinates
                    char coordText[50];
                    sprintf(coordText, "X: %d  Y: %d", x, y);
                    display.drawText(coordText, 10, 210, TFT_WHITE, 3);
                    
                    // Draw a small circle at touch position
                    display.getTFT().fillCircle(x, y, 5, TFT_RED);
                    
                    // Print to serial
                    Serial.print("Touch detected at X: ");
                    Serial.print(x);
                    Serial.print(", Y: ");
                    Serial.println(y);
                    
                    // Visual feedback - flash indicator
                    display.getTFT().fillCircle(450, 20, 15, TFT_GREEN);
                    delay(50);
                    display.getTFT().fillCircle(450, 20, 15, TFT_DARKGREEN);
                }
            }
        }
    }
    
    // Small delay to prevent overwhelming the CPU
    delay(10);
}
