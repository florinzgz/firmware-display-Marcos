#include "Display.h"

Display::Display() 
    : tft(TFT_eSPI()),
      touch(XPT2046_Touchscreen(TOUCH_CS, TOUCH_IRQ)),
      screenWidth(0),
      screenHeight(0),
      touchEnabled(false) {
}

bool Display::init() {
    // Initialize TFT display
    tft.init();
    tft.setRotation(1);  // Landscape mode (480x320)
    
    // Get screen dimensions after rotation
    screenWidth = tft.width();
    screenHeight = tft.height();
    
    // Clear screen to black
    clear(TFT_BLACK);
    
    // Initialize touch controller
    touchEnabled = touch.begin();
    
    if (touchEnabled) {
        touch.setRotation(1);  // Match display rotation
        Serial.println("Touch controller initialized successfully");
    } else {
        Serial.println("Warning: Touch controller initialization failed");
    }
    
    // Display initialization message
    Serial.println("Display initialized");
    Serial.print("Screen size: ");
    Serial.print(screenWidth);
    Serial.print("x");
    Serial.println(screenHeight);
    
    return true;
}

void Display::clear(uint16_t color) {
    tft.fillScreen(color);
}

void Display::drawText(const char* text, int16_t x, int16_t y, 
                       uint16_t color, uint8_t size) {
    tft.setTextColor(color);
    tft.setTextSize(size);
    tft.setCursor(x, y);
    tft.print(text);
}

bool Display::isTouched() {
    if (!touchEnabled) {
        return false;
    }
    return touch.touched();
}

bool Display::getTouchCoordinates(int16_t* x, int16_t* y) {
    if (!touchEnabled || !touch.touched()) {
        return false;
    }
    
    TS_Point p = touch.getPoint();
    
    // Map touch coordinates to screen coordinates
    // The touch controller coordinates need to be calibrated/mapped
    // These values may need adjustment based on your specific display
    *x = map(p.x, 200, 3800, 0, screenWidth);
    *y = map(p.y, 200, 3800, 0, screenHeight);
    
    // Clamp values to screen boundaries
    *x = constrain(*x, 0, screenWidth - 1);
    *y = constrain(*y, 0, screenHeight - 1);
    
    return true;
}

void Display::setTextColor(uint16_t color, uint16_t bgcolor) {
    tft.setTextColor(color, bgcolor);
}

void Display::setTextSize(uint8_t size) {
    tft.setTextSize(size);
}
