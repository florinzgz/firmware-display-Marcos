#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>

// Touch controller pins
#define TOUCH_CS 8     // Touch chip select
#define TOUCH_IRQ 7    // Touch interrupt (optional, can be -1)

/**
 * @brief Display class for managing TFT display and touch input
 * 
 * This class provides a simple interface for:
 * - Initializing the ILI9488 TFT display
 * - Clearing the screen
 * - Drawing text at specified positions
 * - Reading touch coordinates for menu interaction
 */
class Display {
private:
    TFT_eSPI tft;                    // TFT display object
    XPT2046_Touchscreen touch;       // Touch controller object
    uint16_t screenWidth;            // Screen width in pixels
    uint16_t screenHeight;           // Screen height in pixels
    bool touchEnabled;               // Touch functionality status

public:
    /**
     * @brief Constructor for Display class
     */
    Display();

    /**
     * @brief Initialize the display and touch controller
     * @return true if initialization successful, false otherwise
     */
    bool init();

    /**
     * @brief Clear the screen with a specified color
     * @param color 16-bit RGB565 color (default: black)
     */
    void clear(uint16_t color = TFT_BLACK);

    /**
     * @brief Draw text at specified coordinates
     * @param text Text string to display
     * @param x X coordinate (pixels from left)
     * @param y Y coordinate (pixels from top)
     * @param color Text color in RGB565 format (default: white)
     * @param size Text size multiplier (default: 2)
     */
    void drawText(const char* text, int16_t x, int16_t y, 
                  uint16_t color = TFT_WHITE, uint8_t size = 2);

    /**
     * @brief Check if screen is currently being touched
     * @return true if touch detected, false otherwise
     */
    bool isTouched();

    /**
     * @brief Get the current touch coordinates
     * @param x Pointer to store X coordinate
     * @param y Pointer to store Y coordinate
     * @return true if valid touch coordinates obtained, false otherwise
     */
    bool getTouchCoordinates(int16_t* x, int16_t* y);

    /**
     * @brief Get screen width
     * @return Screen width in pixels
     */
    uint16_t getWidth() const { return screenWidth; }

    /**
     * @brief Get screen height
     * @return Screen height in pixels
     */
    uint16_t getHeight() const { return screenHeight; }

    /**
     * @brief Get direct access to TFT object for advanced operations
     * @return Reference to TFT_eSPI object
     */
    TFT_eSPI& getTFT() { return tft; }

    /**
     * @brief Set text color for subsequent text operations
     * @param color Text color in RGB565 format
     * @param bgcolor Background color in RGB565 format
     */
    void setTextColor(uint16_t color, uint16_t bgcolor = TFT_BLACK);

    /**
     * @brief Set text size for subsequent text operations
     * @param size Text size multiplier
     */
    void setTextSize(uint8_t size);
};

#endif // DISPLAY_H
