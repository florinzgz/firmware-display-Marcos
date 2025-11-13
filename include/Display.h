#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include "display_types.h"

// Touch controller pins - Configuración coche eléctrico
#define TOUCH_CS 22     // Touch chip select
#define TOUCH_IRQ 46    // Touch interrupt

// Colores del tema
#define COLOR_BG         0x0000  // Negro
#define COLOR_BG_DARK    0x1082  // Gris oscuro
#define COLOR_ACCENT     0x07FF  // Cyan
#define COLOR_TEXT       0xFFFF  // Blanco
#define COLOR_TEXT_DIM   0x7BEF  // Gris claro
#define COLOR_OK         0x07E0  // Verde
#define COLOR_WARNING    0xFFE0  // Amarillo
#define COLOR_ERROR      0xF800  // Rojo

/**
 * @brief Clase Display completa para sistema de coche eléctrico
 * 
 * Gestiona dashboard, menús interactivos, widgets y visualización de datos
 * en tiempo real para un coche eléctrico infantil controlado por ESP32-S3
 */
class Display {
private:
    static TFT_eSPI tft;                    // Objeto TFT display
    static XPT2046_Touchscreen touch;       // Objeto touch controller
    static uint16_t screenWidth;            // Ancho de pantalla en píxeles
    static uint16_t screenHeight;           // Alto de pantalla en píxeles
    static bool touchEnabled;               // Estado funcionalidad táctil
    static MenuType currentMenu;            // Menú actual
    static VehicleData vehicleData;         // Datos del vehículo
    static TouchGesture lastGesture;        // Último gesto detectado
    static uint32_t lastUpdateTime;         // Tiempo de última actualización
    static bool needsRedraw;                // Flag para redibujar pantalla
    
    // Variables para detección de gestos
    static bool touching;
    static int16_t touchStartX, touchStartY;
    static uint32_t touchStartTime;
    static uint32_t lastTouchTime;

public:
    // ============ INICIALIZACIÓN Y CORE ============
    
    /**
     * @brief Inicializa el display y el controlador táctil
     */
    static void init();
    
    /**
     * @brief Actualiza el display - llamar en loop()
     */
    static void update();
    
    // ============ DASHBOARD PRINCIPAL ============
    
    /**
     * @brief Muestra el dashboard principal con todos los indicadores
     */
    static void showDashboard();
    
    /**
     * @brief Dibuja velocímetro analógico estilo aguja
     * @param speed Velocidad en km/h (0-25)
     */
    static void drawSpeedometer(float speed);
    
    /**
     * @brief Dibuja tacómetro/RPM circular
     * @param rpm Revoluciones por minuto
     */
    static void drawTachometer(float rpm);
    
    /**
     * @brief Dibuja odómetro digital (total + parcial)
     * @param total Kilómetros totales
     * @param trip Kilómetros parciales
     */
    static void drawOdometer(float total, float trip);
    
    /**
     * @brief Dibuja barra de batería animada con porcentaje y voltaje
     * @param voltage Voltaje de batería
     * @param percent Porcentaje (0-100)
     */
    static void drawBatteryStatus(float voltage, float percent);
    
    /**
     * @brief Dibuja 4 indicadores de temperatura de motores
     * @param t1, t2, t3, t4 Temperaturas en °C
     */
    static void drawTemperatures(float t1, float t2, float t3, float t4);
    
    /**
     * @brief Dibuja indicador de marcha actual
     * @param gear Posición de marcha (P/N/R/D1/D2)
     */
    static void drawGearIndicator(GearPosition gear);
    
    /**
     * @brief Dibuja iconos de estado del sistema
     * @param status Estructura con estados de luces, 4x4, freno, etc.
     */
    static void drawStatusIcons(const SystemStatus& status);
    
    // ============ SISTEMA DE MENÚS ============
    
    /**
     * @brief Muestra un menú específico
     * @param type Tipo de menú a mostrar
     */
    static void showMenu(MenuType type);
    
    /**
     * @brief Muestra menú oculto de configuración
     */
    static void showHiddenMenu();
    
    /**
     * @brief Muestra menú rápido lateral
     */
    static void showQuickMenu();
    
    /**
     * @brief Muestra menú de calibración
     */
    static void showCalibrationMenu();
    
    /**
     * @brief Muestra monitor INA226 en tiempo real
     */
    static void showINA226Monitor();
    
    /**
     * @brief Muestra test de hardware completo
     */
    static void showHardwareTest();
    
    /**
     * @brief Muestra configuración WiFi/OTA
     */
    static void showWiFiConfig();
    
    /**
     * @brief Muestra estadísticas del sistema
     */
    static void showStatistics();
    
    // ============ WIDGETS INTERACTIVOS ============
    
    /**
     * @brief Dibuja y gestiona rueda selectora circular tipo iOS
     * @param x, y Coordenadas del centro
     * @param radius Radio de la rueda
     * @param value Valor actual
     * @param min, max Rango de valores
     * @param label Etiqueta del widget
     * @return Nuevo valor después de interacción
     */
    static int drawWheel(int16_t x, int16_t y, int16_t radius, 
                        int value, int min, int max, const char* label);
    
    /**
     * @brief Dibuja y gestiona slider horizontal
     * @param x, y Coordenadas esquina superior izquierda
     * @param width Ancho del slider
     * @param value Valor actual
     * @param min, max Rango de valores
     * @param label Etiqueta del widget
     * @return Nuevo valor después de interacción
     */
    static int drawSlider(int16_t x, int16_t y, int16_t width, 
                         int value, int min, int max, const char* label);
    
    /**
     * @brief Dibuja y gestiona botón táctil con feedback visual
     * @param x, y Coordenadas esquina superior izquierda
     * @param w, h Dimensiones del botón
     * @param label Texto del botón
     * @return true si el botón fue presionado
     */
    static bool drawButton(int16_t x, int16_t y, int16_t w, int16_t h, 
                          const char* label);
    
    // ============ VISUALIZACIÓN DE DATOS ============
    
    /**
     * @brief Dibuja gráfico de barras para corriente de 4 motores
     * @param currents Array con 4 valores de corriente
     */
    static void drawCurrentBars(const float currents[4]);
    
    /**
     * @brief Dibuja gráfico de línea histórico de voltaje
     * @param voltage Voltaje actual a añadir al histórico
     */
    static void drawVoltageGraph(float voltage);
    
    // ============ ALERTAS Y NOTIFICACIONES ============
    
    /**
     * @brief Muestra alerta visual con popup modal
     * @param level Nivel de alerta (INFO/WARNING/ERROR/CRITICAL)
     * @param msg Mensaje a mostrar
     */
    static void showAlert(AlertLevel level, const char* msg);
    
    /**
     * @brief Muestra notificación tipo toast no bloqueante
     * @param msg Mensaje a mostrar
     * @param duration Duración en ms
     */
    static void showToast(const char* msg, uint32_t duration = 2000);
    
    // ============ ANIMACIONES ============
    
    /**
     * @brief Animación de transición entre menús
     * @param type Tipo de transición (fade, slide)
     */
    static void playTransition(const char* type);
    
    /**
     * @brief Animación de arranque del sistema
     */
    static void playBootAnimation();
    
    /**
     * @brief Muestra barra de progreso para operaciones largas
     * @param percent Porcentaje completado (0-100)
     * @param label Texto descriptivo
     */
    static void drawProgressBar(int16_t x, int16_t y, int16_t width, 
                                int16_t height, int percent, const char* label);
    
    // ============ GESTIÓN TÁCTIL Y GESTOS ============
    
    /**
     * @brief Detecta y procesa gestos táctiles
     * @return Tipo de gesto detectado
     */
    static TouchGesture detectGesture();
    
    /**
     * @brief Verifica si hay toque activo
     * @return true si se está tocando la pantalla
     */
    static bool isTouched();
    
    /**
     * @brief Obtiene coordenadas del toque actual
     * @param x, y Punteros para almacenar coordenadas
     * @return true si se obtuvieron coordenadas válidas
     */
    static bool getTouchCoordinates(int16_t* x, int16_t* y);
    
    /**
     * @brief Verifica si un punto está dentro de un rectángulo
     */
    static bool isTouchInRect(int16_t x, int16_t y, int16_t rx, int16_t ry, 
                             int16_t rw, int16_t rh);
    
    // ============ UTILIDADES ============
    
    /**
     * @brief Limpia la pantalla
     * @param color Color de fondo
     */
    static void clear(uint16_t color = COLOR_BG);
    
    /**
     * @brief Dibuja texto en coordenadas específicas
     */
    static void drawText(const char* text, int16_t x, int16_t y, 
                        uint16_t color = COLOR_TEXT, uint8_t size = 2);
    
    /**
     * @brief Dibuja texto centrado horizontalmente
     */
    static void drawCenteredText(const char* text, int16_t y, 
                                 uint16_t color = COLOR_TEXT, uint8_t size = 2);
    
    /**
     * @brief Actualiza datos del vehículo
     * @param data Nueva estructura de datos
     */
    static void updateVehicleData(const VehicleData& data);
    
    /**
     * @brief Obtiene referencia al objeto TFT para operaciones avanzadas
     */
    static TFT_eSPI& getTFT() { return tft; }
    
    /**
     * @brief Obtiene dimensiones de la pantalla
     */
    static uint16_t getWidth() { return screenWidth; }
    static uint16_t getHeight() { return screenHeight; }
    
    /**
     * @brief Fuerza redibujado completo en próximo update()
     */
    static void requestRedraw() { needsRedraw = true; }

private:
    // Funciones auxiliares privadas
    static void drawArc(int16_t x, int16_t y, int16_t r, int16_t startAngle, 
                       int16_t endAngle, uint16_t color, int16_t thickness);
    static void drawIcon(int16_t x, int16_t y, const char* iconType, uint16_t color);
    static uint16_t getTemperatureColor(float temp);
    static void updateGestureDetection();
};

#endif // DISPLAY_H
