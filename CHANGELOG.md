# Changelog

## [1.0.0] - 2025-11-18

### Implementado
- Vista dinámica del coche desde arriba (top-down view)
- Ruedas con animación de rotación basada en aceleración
- Volante dinámico que refleja el ángulo de dirección (-45° a +45°)
- Indicador de pedal de acelerador con visualización porcentual
- Indicador de batería con código de colores (verde/naranja/rojo)
- Menú de calibración oculto accesible por gesto táctil (long-press)
- Calibración individual de sensores
- Activación/desactivación de sensores desde el menú
- Soporte para encoder rotatorio con navegación de menú
- Lectura de 4 sensores analógicos (dirección, acelerador, freno, batería)
- Gestión de interrupciones para encoder y botón
- Detección de gestos táctiles (long-press en esquina superior izquierda)
- Documentación completa en español:
  - Manual de usuario
  - Diagrama de conexiones
  - Especificación técnica
  - Ejemplos de código

### Hardware Soportado
- ESP32-S3-DevKitC-1
- Pantalla TFT ILI9488 (320x480)
- Panel táctil XPT2046
- Encoder rotatorio con botón
- Sensores analógicos (potenciómetros o similar)

### Bibliotecas
- TFT_eSPI v2.5.43
- XPT2046_Touchscreen v1.4

### Características
- Actualización de pantalla a 10 Hz
- ADC de 12-bit para sensores analógicos
- Mapeo automático de valores de sensores
- Visualización en tiempo real
- Interfaz intuitiva y responsive

### Estructura del Proyecto
```
firmware-display-Marcos/
├── docs/                      # Documentación
│   ├── CONNECTIONS.md         # Diagrama de conexiones
│   ├── USER_MANUAL.md         # Manual de usuario
│   └── TECHNICAL_SPEC.md      # Especificación técnica
├── examples/                  # Ejemplos de código
│   ├── simple_car_display.cpp
│   └── calibration_menu.cpp
├── include/                   # Headers
│   ├── CarDisplay.h
│   └── CalibrationMenu.h
├── src/                       # Implementación
│   ├── main.cpp
│   ├── CarDisplay.cpp
│   └── CalibrationMenu.cpp
├── platformio.ini             # Configuración PlatformIO
├── .gitignore
└── README.md
```

### Notas
- Primera versión funcional completa
- Calibración no persistente (se pierde al reiniciar)
- Sin modo de ahorro de energía
