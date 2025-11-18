#ifndef CALIBRATION_MENU_H
#define CALIBRATION_MENU_H

#include <TFT_eSPI.h>

// Menu colors
#define MENU_BACKGROUND 0x18E3     // Dark gray
#define MENU_HEADER 0x001F         // Blue
#define MENU_TEXT 0xFFFF           // White
#define MENU_SELECTED 0x07E0       // Green
#define MENU_DISABLED 0xF800       // Red

// Sensor types
enum SensorType {
    SENSOR_STEERING,
    SENSOR_ACCELERATOR,
    SENSOR_BRAKE,
    SENSOR_BATTERY,
    SENSOR_COUNT
};

struct SensorConfig {
    const char* name;
    bool enabled;
    int minValue;
    int maxValue;
    int currentValue;
};

class CalibrationMenu {
private:
    TFT_eSPI* tft;
    bool menuVisible;
    int selectedItem;
    int encoderPosition;
    SensorConfig sensors[SENSOR_COUNT];
    bool calibrationMode;
    int calibratingSensor;
    
    void drawHeader();
    void drawSensorItem(int index, int y);
    void drawCalibrationScreen();

public:
    CalibrationMenu(TFT_eSPI* display);
    
    void begin();
    void show();
    void hide();
    void toggle();
    bool isVisible();
    void update();
    void handleEncoder(int delta);
    void handleButton();
    void setSensorValue(SensorType sensor, int value);
    void setSensorEnabled(SensorType sensor, bool enabled);
    bool isSensorEnabled(SensorType sensor);
    int getSensorMin(SensorType sensor);
    int getSensorMax(SensorType sensor);
};

#endif
