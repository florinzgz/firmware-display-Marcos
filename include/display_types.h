#ifndef DISPLAY_TYPES_H
#define DISPLAY_TYPES_H

#include <stdint.h>

// Tipos de menú
enum class MenuType {
    NONE,
    DASHBOARD,
    SETTINGS,
    CALIBRATION,
    HARDWARE_TEST,
    WIFI_CONFIG,
    INA226_MONITOR,
    STATISTICS,
    QUICK_MENU
};

// Niveles de alerta
enum class AlertLevel {
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

// Posiciones de marcha
enum class GearPosition {
    PARK,      // P
    NEUTRAL,   // N
    REVERSE,   // R
    DRIVE1,    // D1
    DRIVE2     // D2
};

// Estados del sistema
struct SystemStatus {
    bool lights;
    bool fourWheelDrive;
    bool parkingBrake;
    bool bluetooth;
    bool wifi;
    bool warnings;
};

// Datos del vehículo
struct VehicleData {
    float speed;              // km/h (0-25)
    float rpm;                // Revoluciones por minuto
    float odoTotal;           // Odómetro total en km
    float odoTrip;            // Odómetro parcial en km
    float batteryVoltage;     // Voltaje de batería
    float batteryPercent;     // Porcentaje de batería (0-100)
    float motorTemp[4];       // Temperaturas de los 4 motores
    float motorCurrent[4];    // Corriente de los 4 motores
    GearPosition gear;        // Marcha actual
    SystemStatus status;      // Estado del sistema
};

// Datos de un widget wheel (selector circular)
struct WheelWidget {
    int16_t x, y;
    int16_t radius;
    int value;
    int minValue;
    int maxValue;
    const char* label;
    bool touched;
};

// Datos de un slider
struct SliderWidget {
    int16_t x, y;
    int16_t width;
    int16_t height;
    int value;
    int minValue;
    int maxValue;
    const char* label;
    bool touched;
};

// Datos de un botón
struct ButtonWidget {
    int16_t x, y;
    int16_t width;
    int16_t height;
    const char* label;
    bool pressed;
    bool enabled;
};

// Tipo de gesto táctil
enum class GestureType {
    NONE,
    TAP,
    LONG_PRESS,
    SWIPE_LEFT,
    SWIPE_RIGHT,
    SWIPE_UP,
    SWIPE_DOWN
};

// Datos de un gesto
struct TouchGesture {
    GestureType type;
    int16_t startX, startY;
    int16_t endX, endY;
    uint32_t duration;
};

#endif // DISPLAY_TYPES_H
