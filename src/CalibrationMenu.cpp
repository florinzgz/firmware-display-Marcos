#include "CalibrationMenu.h"
#include <Arduino.h>

CalibrationMenu::CalibrationMenu(TFT_eSPI* display) {
    tft = display;
    menuVisible = false;
    selectedItem = 0;
    encoderPosition = 0;
    calibrationMode = false;
    calibratingSensor = -1;
    
    // Initialize sensor configurations
    sensors[SENSOR_STEERING] = {"Steering", true, 0, 4095, 2048};
    sensors[SENSOR_ACCELERATOR] = {"Accelerator", true, 0, 4095, 0};
    sensors[SENSOR_BRAKE] = {"Brake", true, 0, 4095, 0};
    sensors[SENSOR_BATTERY] = {"Battery", true, 0, 100, 100};
}

void CalibrationMenu::begin() {
    // Nothing to initialize
}

void CalibrationMenu::show() {
    menuVisible = true;
    update();
}

void CalibrationMenu::hide() {
    menuVisible = false;
}

void CalibrationMenu::toggle() {
    if (menuVisible) {
        hide();
    } else {
        show();
    }
}

bool CalibrationMenu::isVisible() {
    return menuVisible;
}

void CalibrationMenu::drawHeader() {
    tft->fillRect(0, 0, 320, 40, MENU_HEADER);
    tft->setTextColor(MENU_TEXT);
    tft->setTextDatum(MC_DATUM);
    tft->drawString("SENSOR CALIBRATION", 160, 20, 4);
}

void CalibrationMenu::drawSensorItem(int index, int y) {
    SensorConfig* sensor = &sensors[index];
    
    // Highlight selected item
    if (index == selectedItem) {
        tft->fillRect(0, y, 320, 50, MENU_SELECTED);
    } else {
        tft->fillRect(0, y, 320, 50, MENU_BACKGROUND);
    }
    
    // Draw separator line
    tft->drawLine(0, y + 50, 320, y + 50, MENU_TEXT);
    
    // Draw sensor name
    tft->setTextColor(MENU_TEXT);
    tft->setTextDatum(ML_DATUM);
    tft->drawString(sensor->name, 10, y + 15, 2);
    
    // Draw enabled/disabled status
    if (sensor->enabled) {
        tft->setTextColor(MENU_SELECTED);
        tft->drawString("ON", 10, y + 35, 2);
    } else {
        tft->setTextColor(MENU_DISABLED);
        tft->drawString("OFF", 10, y + 35, 2);
    }
    
    // Draw current value
    tft->setTextColor(MENU_TEXT);
    tft->setTextDatum(MR_DATUM);
    String valueStr = String(sensor->currentValue);
    tft->drawString(valueStr, 310, y + 15, 2);
    
    // Draw min/max range
    String rangeStr = String(sensor->minValue) + "-" + String(sensor->maxValue);
    tft->drawString(rangeStr, 310, y + 35, 2);
}

void CalibrationMenu::drawCalibrationScreen() {
    tft->fillScreen(MENU_BACKGROUND);
    drawHeader();
    
    SensorConfig* sensor = &sensors[calibratingSensor];
    
    // Instructions
    tft->setTextColor(MENU_TEXT);
    tft->setTextDatum(MC_DATUM);
    tft->drawString("Calibrating: " + String(sensor->name), 160, 60, 2);
    tft->drawString("Move sensor to MIN position", 160, 90, 2);
    tft->drawString("then press button", 160, 110, 2);
    
    // Current value
    tft->drawString("Current: " + String(sensor->currentValue), 160, 150, 4);
    
    // Progress bar
    int barWidth = 280;
    int barX = 20;
    int barY = 200;
    tft->drawRect(barX, barY, barWidth, 30, MENU_TEXT);
    
    // Calculate position in range
    int fillWidth = map(sensor->currentValue, sensor->minValue, sensor->maxValue, 0, barWidth - 4);
    if (fillWidth < 0) fillWidth = 0;
    if (fillWidth > barWidth - 4) fillWidth = barWidth - 4;
    tft->fillRect(barX + 2, barY + 2, fillWidth, 26, MENU_SELECTED);
    
    // Current range
    tft->setTextDatum(MC_DATUM);
    tft->drawString("Min: " + String(sensor->minValue), 80, 250, 2);
    tft->drawString("Max: " + String(sensor->maxValue), 240, 250, 2);
    
    // Instructions to exit
    tft->drawString("Long press to save and exit", 160, 300, 2);
}

void CalibrationMenu::update() {
    if (!menuVisible) return;
    
    if (calibrationMode) {
        drawCalibrationScreen();
        return;
    }
    
    tft->fillScreen(MENU_BACKGROUND);
    drawHeader();
    
    // Draw sensor items
    for (int i = 0; i < SENSOR_COUNT; i++) {
        drawSensorItem(i, 50 + i * 55);
    }
    
    // Draw footer instructions
    tft->fillRect(0, 440, 320, 40, MENU_HEADER);
    tft->setTextColor(MENU_TEXT);
    tft->setTextDatum(MC_DATUM);
    tft->drawString("Encoder: Select | Button: Toggle/Calibrate", 160, 460, 2);
}

void CalibrationMenu::handleEncoder(int delta) {
    if (!menuVisible) return;
    
    if (calibrationMode) {
        // In calibration mode, encoder adjusts the current value
        SensorConfig* sensor = &sensors[calibratingSensor];
        sensor->currentValue += delta * 10;
        if (sensor->currentValue < 0) sensor->currentValue = 0;
        if (sensor->currentValue > 4095) sensor->currentValue = 4095;
        update();
    } else {
        // In menu mode, encoder selects items
        selectedItem += delta;
        if (selectedItem < 0) selectedItem = 0;
        if (selectedItem >= SENSOR_COUNT) selectedItem = SENSOR_COUNT - 1;
        update();
    }
}

void CalibrationMenu::handleButton() {
    if (!menuVisible) return;
    
    if (calibrationMode) {
        // Exit calibration mode
        calibrationMode = false;
        calibratingSensor = -1;
        update();
    } else {
        // Toggle sensor or enter calibration mode
        // Short press: toggle enabled/disabled
        sensors[selectedItem].enabled = !sensors[selectedItem].enabled;
        update();
    }
}

void CalibrationMenu::setSensorValue(SensorType sensor, int value) {
    if (sensor >= 0 && sensor < SENSOR_COUNT) {
        sensors[sensor].currentValue = value;
    }
}

void CalibrationMenu::setSensorEnabled(SensorType sensor, bool enabled) {
    if (sensor >= 0 && sensor < SENSOR_COUNT) {
        sensors[sensor].enabled = enabled;
    }
}

bool CalibrationMenu::isSensorEnabled(SensorType sensor) {
    if (sensor >= 0 && sensor < SENSOR_COUNT) {
        return sensors[sensor].enabled;
    }
    return false;
}

int CalibrationMenu::getSensorMin(SensorType sensor) {
    if (sensor >= 0 && sensor < SENSOR_COUNT) {
        return sensors[sensor].minValue;
    }
    return 0;
}

int CalibrationMenu::getSensorMax(SensorType sensor) {
    if (sensor >= 0 && sensor < SENSOR_COUNT) {
        return sensors[sensor].maxValue;
    }
    return 4095;
}
