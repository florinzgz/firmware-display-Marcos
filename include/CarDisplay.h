#ifndef CAR_DISPLAY_H
#define CAR_DISPLAY_H

#include <TFT_eSPI.h>

// Colors
#define COLOR_BACKGROUND 0x0000    // Black
#define COLOR_CAR_BODY 0x001F      // Blue
#define COLOR_WHEEL 0xFFFF         // White
#define COLOR_TIRE 0x39E7          // Dark gray
#define COLOR_STEERING 0xFD20      // Orange
#define COLOR_PEDAL 0x07E0         // Green
#define COLOR_BATTERY_OK 0x07E0    // Green
#define COLOR_BATTERY_LOW 0xFD20   // Orange
#define COLOR_BATTERY_CRITICAL 0xF800  // Red
#define COLOR_TEXT 0xFFFF          // White

// Car dimensions (top view)
#define CAR_CENTER_X 160
#define CAR_CENTER_Y 240
#define CAR_WIDTH 120
#define CAR_LENGTH 180
#define WHEEL_WIDTH 20
#define WHEEL_LENGTH 40

class CarDisplay {
private:
    TFT_eSPI* tft;
    
    // Dynamic states
    float wheelAngle;        // Steering angle in degrees (-45 to 45)
    float wheelRotation;     // Wheel rotation for movement animation
    float pedalPosition;     // Pedal position (0.0 to 1.0)
    float batteryLevel;      // Battery level (0.0 to 1.0)
    
    void drawWheel(int x, int y, float angle, float rotation);
    void drawSteeringWheel(int x, int y, float angle);
    void drawPedal(int x, int y, float position);
    void drawBattery(int x, int y, float level);
    void drawCarBody();

public:
    CarDisplay(TFT_eSPI* display);
    
    void begin();
    void update();
    void setWheelAngle(float angle);
    void setWheelRotation(float rotation);
    void setPedalPosition(float position);
    void setBatteryLevel(float level);
    void clear();
};

#endif
