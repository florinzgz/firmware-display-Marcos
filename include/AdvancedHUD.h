#ifndef ADVANCED_HUD_H
#define ADVANCED_HUD_H

#include <TFT_eSPI.h>
#include "display_types.h"

// Colores tema Mercedes
#define COLOR_MERCEDES_SILVER  0xC618  // Plata Mercedes
#define COLOR_MERCEDES_BLUE    0x0356  // Azul Mercedes oscuro
#define COLOR_STAR_GOLD        0xFEA0  // Dorado estrella
#define COLOR_GAUGE_RED        0xF800  // Rojo aguja
#define COLOR_GAUGE_GREEN      0x07E0  // Verde aguja
#define COLOR_HUD_BG           0x0000  // Negro
#define COLOR_HUD_PANEL        0x18C3  // Gris panel
#define COLOR_HUD_ACCENT       0x07FF  // Cyan accent

/**
 * @brief Datos de una rueda individual
 */
struct WheelData {
    float effort;        // % esfuerzo (0-100) calculado desde corriente
    float temperature;   // Temperatura en °C
    float current;       // Corriente INA226 en A
    float voltage;       // Voltaje INA226 en V
    int16_t angle;       // Ángulo dirección grados (solo FL/FR)
    bool online;         // INA226 online/offline
};

/**
 * @brief Datos completos para el HUD avanzado
 */
struct HUDData {
    // Ruedas (Front Left, Front Right, Rear Left, Rear Right)
    WheelData wheelFL;
    WheelData wheelFR;
    WheelData wheelRL;
    WheelData wheelRR;
    
    // Datos del vehículo
    float speed;              // km/h (0-30)
    float rpm;                // RPM (0-220)
    float batteryVoltage;     // V (20-28)
    float batteryPercent;     // % (0-100)
    float ambientTemp;        // °C temperatura ambiente
    int8_t pedalPosition;     // % (0-100)
    GearPosition gear;        // P/D2/D1/N/R
    
    // Control
    int16_t encoderAngle;     // Ángulo volante (-90 a +90 grados)
};

/**
 * @brief Clase Advanced HUD con tema Mercedes
 * 
 * Sistema de visualización tipo HUD con vista cenital del vehículo,
 * datos en tiempo real de 4 ruedas (INA226), velocímetro, tacómetro,
 * y controles estilo Mercedes.
 */
class AdvancedHUD {
public:
    /**
     * @brief Inicializa el sistema HUD
     */
    static void init();
    
    /**
     * @brief Actualiza la visualización del HUD
     * Llamar en loop() - rate 20 FPS
     */
    static void update();
    
    /**
     * @brief Actualiza los datos del HUD
     * @param data Estructura con todos los datos del vehículo
     */
    static void updateHUDData(const HUDData& data);
    
    /**
     * @brief Calcula esfuerzo desde corriente
     * @param current Corriente en amperios
     * @return Porcentaje de esfuerzo (0-100)
     */
    static float calculateEffort(float current);
    
    /**
     * @brief Obtiene color según temperatura
     * @param temp Temperatura en °C
     * @return Color RGB565
     */
    static uint16_t getTemperatureColor(float temp);
    
    /**
     * @brief Obtiene color según esfuerzo
     * @param effort Porcentaje de esfuerzo (0-100)
     * @return Color RGB565
     */
    static uint16_t getEffortColor(float effort);
    
    /**
     * @brief Fuerza redibujado completo en próximo update
     */
    static void requestRedraw();

private:
    // Referencia al display TFT
    static TFT_eSPI* tft;
    
    // Datos actuales del HUD
    static HUDData hudData;
    static HUDData prevHudData;  // Para detectar cambios
    
    // Control de actualización
    static unsigned long lastUpdateTime;
    static bool needsFullRedraw;
    
    // === FUNCIONES DE DIBUJADO ===
    
    /**
     * @brief Dibuja la vista cenital completa del coche Mercedes
     */
    static void drawTopDownCar();
    
    /**
     * @brief Dibuja una rueda individual con sus datos
     * @param x, y Coordenadas esquina superior izquierda
     * @param wheel Datos de la rueda
     * @param label Etiqueta (FL/FR/RL/RR)
     * @param isFront true si es rueda delantera (muestra ángulo)
     */
    static void drawWheel(int16_t x, int16_t y, const WheelData& wheel, 
                         const char* label, bool isFront);
    
    /**
     * @brief Dibuja el logo Mercedes 3D (estrella de 3 puntas)
     * @param x, y Centro del logo
     * @param size Radio del logo
     */
    static void drawMercedesLogo(int16_t x, int16_t y, int16_t size);
    
    /**
     * @brief Dibuja el volante con indicación de ángulo
     * @param x, y Centro del volante
     * @param angle Ángulo del encoder (-90 a +90)
     */
    static void drawSteeringWheel(int16_t x, int16_t y, int16_t angle);
    
    /**
     * @brief Dibuja los cardanes virtuales Ackermann
     * Conecta volante con ruedas delanteras
     */
    static void drawAckermannLinks();
    
    /**
     * @brief Dibuja velocímetro estilo HUD
     * @param speed Velocidad en km/h (0-30)
     */
    static void drawSpeedometerHUD(float speed);
    
    /**
     * @brief Dibuja tacómetro estilo HUD
     * @param rpm RPM (0-220)
     */
    static void drawTachometerHUD(float rpm);
    
    /**
     * @brief Dibuja indicador de batería
     * @param voltage Voltaje
     * @param percent Porcentaje
     */
    static void drawBatteryHUD(float voltage, float percent);
    
    /**
     * @brief Dibuja temperatura ambiente
     * @param temp Temperatura en °C
     */
    static void drawAmbientTemp(float temp);
    
    /**
     * @brief Dibuja selector de marcha iluminado
     * @param gear Marcha actual
     */
    static void drawShifter(GearPosition gear);
    
    /**
     * @brief Dibuja barra de posición del pedal
     * @param position Posición del pedal (0-100%)
     */
    static void drawPedalBar(int8_t position);
    
    /**
     * @brief Dibuja gauge circular genérico
     * @param x, y Centro del gauge
     * @param radius Radio del gauge
     * @param value Valor actual
     * @param maxValue Valor máximo
     * @param label Etiqueta
     * @param needleColor Color de la aguja
     */
    static void drawCircularGauge(int16_t x, int16_t y, int16_t radius,
                                  float value, float maxValue,
                                  const char* label, uint16_t needleColor);
    
    /**
     * @brief Calcula ángulo Ackermann para una rueda
     * @param encoderAngle Ángulo del encoder central
     * @param isLeft true si es rueda izquierda
     * @return Ángulo de la rueda en grados
     */
    static int16_t calculateAckermannAngle(int16_t encoderAngle, bool isLeft);
    
    /**
     * @brief Verifica si un valor cambió significativamente
     */
    static bool hasChanged(float current, float previous, float threshold);
};

#endif // ADVANCED_HUD_H
