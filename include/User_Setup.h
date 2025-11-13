#ifndef USER_SETUP_H
#define USER_SETUP_H

// Driver for ILI9488 480x320 TFT display
#define ILI9488_DRIVER

// Display resolution
#define TFT_WIDTH  320
#define TFT_HEIGHT 480

// ESP32-S3 SPI pins for TFT display - Configuración coche eléctrico
#define TFT_MISO 12  // MISO (Master In Slave Out)
#define TFT_MOSI 11  // MOSI (Master Out Slave In)
#define TFT_SCLK 10  // Clock
#define TFT_CS   8   // Chip select
#define TFT_DC   13  // Data/Command
#define TFT_RST  14  // Reset

// SPI frequency
#define SPI_FREQUENCY       27000000  // 27 MHz for ILI9488
#define SPI_READ_FREQUENCY  16000000  // 16 MHz for reading
#define SPI_TOUCH_FREQUENCY  2500000  // 2.5 MHz for touch controller

// Font support - include the fonts you want to use
#define LOAD_GLCD   // Font 1. Original Adafruit 8 pixel font
#define LOAD_FONT2  // Font 2. Small 16 pixel height font
#define LOAD_FONT4  // Font 4. Medium 26 pixel height font
#define LOAD_FONT6  // Font 6. Large 48 pixel height font
#define LOAD_FONT7  // Font 7. 7 segment 48 pixel height font
#define LOAD_FONT8  // Font 8. Large 75 pixel height font
#define LOAD_GFXFF  // FreeFonts. Include access to the 48 Adafruit_GFX free fonts

// Smooth fonts
#define SMOOTH_FONT

// Color depth (16-bit for ESP32)
#define TFT_RGB_ORDER TFT_BGR  // Color order for ILI9488

#endif // USER_SETUP_H
