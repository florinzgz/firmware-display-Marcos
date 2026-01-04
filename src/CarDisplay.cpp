#include "CarDisplay.h"
#include <Arduino.h>

CarDisplay::CarDisplay(TFT_eSPI* display) {
    tft = display;
    wheelAngle = 0.0;
    wheelRotation = 0.0;
    pedalPosition = 0.0;
    batteryLevel = 1.0;
}

void CarDisplay::begin() {
    clear();
}

void CarDisplay::clear() {
    tft->fillScreen(COLOR_BACKGROUND);
}

void CarDisplay::drawCarBody() {
    // Draw main car body (rectangle with rounded corners)
    int bodyX = CAR_CENTER_X - CAR_WIDTH / 2;
    int bodyY = CAR_CENTER_Y - CAR_LENGTH / 2;
    
    tft->fillRoundRect(bodyX, bodyY, CAR_WIDTH, CAR_LENGTH, 15, COLOR_CAR_BODY);
    tft->drawRoundRect(bodyX, bodyY, CAR_WIDTH, CAR_LENGTH, 15, COLOR_TEXT);
    
    // Draw windshield
    tft->fillRect(bodyX + 20, bodyY + 20, CAR_WIDTH - 40, 30, 0x39C7);
    
    // Draw rear window
    tft->fillRect(bodyX + 20, bodyY + CAR_LENGTH - 50, CAR_WIDTH - 40, 30, 0x39C7);
}

void CarDisplay::drawWheel(int x, int y, float angle, float rotation) {
    // Save current state
    int centerX = x;
    int centerY = y;
    
    // Calculate rotated wheel position based on steering angle
    float angleRad = angle * PI / 180.0;
    
    // Draw tire (black rectangle)
    tft->fillRoundRect(x - WHEEL_WIDTH / 2, y - WHEEL_LENGTH / 2, 
                       WHEEL_WIDTH, WHEEL_LENGTH, 3, COLOR_TIRE);
    
    // Draw rim (white circle in center)
    tft->fillCircle(x, y, WHEEL_WIDTH / 2 - 2, COLOR_WHEEL);
    
    // Draw rotation indicator lines
    for (int i = 0; i < 4; i++) {
        float lineAngle = rotation + i * PI / 2;
        int x1 = x + cos(lineAngle) * 3;
        int y1 = y + sin(lineAngle) * 3;
        int x2 = x + cos(lineAngle) * (WHEEL_WIDTH / 2 - 4);
        int y2 = y + sin(lineAngle) * (WHEEL_WIDTH / 2 - 4);
        tft->drawLine(x1, y1, x2, y2, COLOR_TIRE);
    }
}

void CarDisplay::drawSteeringWheel(int x, int y, float angle) {
    // Draw steering wheel circle
    tft->drawCircle(x, y, 25, COLOR_STEERING);
    tft->drawCircle(x, y, 26, COLOR_STEERING);
    
    // Draw center
    tft->fillCircle(x, y, 5, COLOR_STEERING);
    
    // Draw spokes rotated by angle
    float angleRad = angle * PI / 180.0;
    for (int i = 0; i < 3; i++) {
        float spokeAngle = angleRad + i * 2 * PI / 3;
        int x1 = x + cos(spokeAngle) * 5;
        int y1 = y + sin(spokeAngle) * 5;
        int x2 = x + cos(spokeAngle) * 23;
        int y2 = y + sin(spokeAngle) * 23;
        tft->drawLine(x1, y1, x2, y2, COLOR_STEERING);
        tft->drawLine(x1 + 1, y1, x2 + 1, y2, COLOR_STEERING);
    }
    
    // Draw angle indicator
    tft->setTextColor(COLOR_TEXT);
    tft->setTextDatum(MC_DATUM);
    tft->drawString(String((int)angle) + "°", x, y + 40, 2);
}

void CarDisplay::drawPedal(int x, int y, float position) {
    // Draw pedal background
    tft->drawRect(x - 15, y - 30, 30, 60, COLOR_TEXT);
    
    // Draw pedal position (filled from bottom)
    int fillHeight = (int)(position * 56);
    int fillY = y + 30 - 2 - fillHeight;
    tft->fillRect(x - 13, fillY, 26, fillHeight, COLOR_PEDAL);
    
    // Draw percentage
    tft->setTextColor(COLOR_TEXT);
    tft->setTextDatum(MC_DATUM);
    tft->drawString(String((int)(position * 100)) + "%", x, y + 45, 2);
}

void CarDisplay::drawBattery(int x, int y, float level) {
    // Determine color based on battery level
    uint16_t batteryColor;
    if (level > 0.5) {
        batteryColor = COLOR_BATTERY_OK;
    } else if (level > 0.2) {
        batteryColor = COLOR_BATTERY_LOW;
    } else {
        batteryColor = COLOR_BATTERY_CRITICAL;
    }
    
    // Draw battery outline
    tft->drawRect(x - 25, y - 15, 50, 30, COLOR_TEXT);
    tft->fillRect(x + 25, y - 5, 3, 10, COLOR_TEXT); // Battery terminal
    
    // Draw battery level
    int fillWidth = (int)(level * 46);
    tft->fillRect(x - 23, y - 13, fillWidth, 26, batteryColor);
    
    // Draw percentage
    tft->setTextColor(COLOR_TEXT);
    tft->setTextDatum(MC_DATUM);
    tft->drawString(String((int)(level * 100)) + "%", x, y + 25, 2);
}

void CarDisplay::update() {
    clear();
    
    // Draw car body
    drawCarBody();
    
    // Calculate wheel positions
    int frontLeftX = CAR_CENTER_X - CAR_WIDTH / 2 - WHEEL_WIDTH / 2;
    int frontLeftY = CAR_CENTER_Y - CAR_LENGTH / 2 + 30;
    
    int frontRightX = CAR_CENTER_X + CAR_WIDTH / 2 + WHEEL_WIDTH / 2;
    int frontRightY = CAR_CENTER_Y - CAR_LENGTH / 2 + 30;
    
    int rearLeftX = CAR_CENTER_X - CAR_WIDTH / 2 - WHEEL_WIDTH / 2;
    int rearLeftY = CAR_CENTER_Y + CAR_LENGTH / 2 - 30;
    
    int rearRightX = CAR_CENTER_X + CAR_WIDTH / 2 + WHEEL_WIDTH / 2;
    int rearRightY = CAR_CENTER_Y + CAR_LENGTH / 2 - 30;
    
    // Draw wheels with steering angle (front wheels) and rotation
    drawWheel(frontLeftX, frontLeftY, wheelAngle, wheelRotation);
    drawWheel(frontRightX, frontRightY, wheelAngle, wheelRotation);
    drawWheel(rearLeftX, rearLeftY, 0, wheelRotation);
    drawWheel(rearRightX, rearRightY, 0, wheelRotation);
    
    // Draw steering wheel indicator (top right)
    drawSteeringWheel(270, 50, wheelAngle);
    
    // Draw pedal indicator (bottom left)
    tft->setTextColor(COLOR_TEXT);
    tft->setTextDatum(TL_DATUM);
    tft->drawString("Pedal", 10, 400, 2);
    drawPedal(35, 450, pedalPosition);
    
    // Draw battery indicator (top left)
    tft->setTextColor(COLOR_TEXT);
    tft->setTextDatum(TL_DATUM);
    tft->drawString("Battery", 10, 10, 2);
    drawBattery(50, 50, batteryLevel);
}

void CarDisplay::setWheelAngle(float angle) {
    // Clamp angle to -45 to 45 degrees
    if (angle < -45.0) angle = -45.0;
    if (angle > 45.0) angle = 45.0;
    wheelAngle = angle;
}

void CarDisplay::setWheelRotation(float rotation) {
    wheelRotation = rotation;
}

void CarDisplay::setPedalPosition(float position) {
    // Clamp position to 0.0 to 1.0
    if (position < 0.0) position = 0.0;
    if (position > 1.0) position = 1.0;
    pedalPosition = position;
}

void CarDisplay::setBatteryLevel(float level) {
    // Clamp level to 0.0 to 1.0
    if (level < 0.0) level = 0.0;
    if (level > 1.0) level = 1.0;
    batteryLevel = level;
}
