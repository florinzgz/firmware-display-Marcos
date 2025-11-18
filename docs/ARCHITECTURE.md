# Arquitectura del Sistema

## Diagrama de Componentes

```
┌─────────────────────────────────────────────────────────────┐
│                      ESP32-S3-DevKitC-1                     │
│                                                             │
│  ┌──────────────┐         ┌──────────────┐                │
│  │   main.cpp   │────────▶│  CarDisplay  │                │
│  │              │         │              │                │
│  │   - Setup    │         │ - Coche      │                │
│  │   - Loop     │         │ - Ruedas     │                │
│  │   - Sensores │         │ - Volante    │                │
│  │   - Touch    │         │ - Pedal      │                │
│  │              │         │ - Batería    │                │
│  └──────┬───────┘         └──────────────┘                │
│         │                                                  │
│         │                 ┌──────────────┐                │
│         └────────────────▶│ CalibMenu    │                │
│                           │              │                │
│                           │ - Sensores   │                │
│                           │ - Calibrar   │                │
│                           │ - On/Off     │                │
│                           │ - Navegación │                │
│                           └──────────────┘                │
│                                                             │
└─────────────────────────────────────────────────────────────┘
                           │
            ┌──────────────┼──────────────┐
            ▼              ▼              ▼
    ┌─────────────┐  ┌──────────┐  ┌──────────┐
    │  TFT        │  │  Touch   │  │ Encoder  │
    │  ILI9488    │  │ XPT2046  │  │          │
    │             │  │          │  │  A B SW  │
    │ 320x480     │  │  Gestos  │  │          │
    └─────────────┘  └──────────┘  └──────────┘
                           │
            ┌──────────────┼──────────────┐
            ▼              ▼              ▼
    ┌─────────────┐  ┌──────────┐  ┌──────────┐
    │  ADC 34     │  │  ADC 35  │  │  ADC 36  │
    │  Steering   │  │  Accel   │  │  Brake   │
    └─────────────┘  └──────────┘  └──────────┘
```

## Flujo de Ejecución

```
Setup:
┌─────────────────────────────────────────────┐
│ 1. Inicializar TFT (320x480, Portrait)     │
│ 2. Inicializar Touch (XPT2046)             │
│ 3. Configurar Encoder (GPIOs + INT)        │
│ 4. Configurar ADC (12-bit)                 │
│ 5. Inicializar CarDisplay                  │
│ 6. Inicializar CalibrationMenu             │
│ 7. Mostrar pantalla inicial                │
└─────────────────────────────────────────────┘

Loop (cada ~10ms):
┌─────────────────────────────────────────────┐
│ 1. Leer encoder (si cambió)                │
│    └─▶ Actualizar menú si visible          │
│                                             │
│ 2. Leer botón encoder (si presionado)      │
│    └─▶ Toggle sensor o acción menú         │
│                                             │
│ 3. Leer touch (polling)                    │
│    └─▶ Detectar long-press esquina         │
│        └─▶ Toggle menú calibración         │
│                                             │
│ 4. Leer sensores ADC (4 canales)           │
│    ├─▶ Actualizar CalibrationMenu          │
│    └─▶ Mapear y actualizar CarDisplay      │
│                                             │
│ 5. Animar rotación ruedas                  │
│    └─▶ Basado en acelerador                │
│                                             │
│ 6. Actualizar pantalla (cada 100ms)        │
│    ├─▶ Si menú visible: CalibMenu.update() │
│    └─▶ Si no: CarDisplay.update()          │
└─────────────────────────────────────────────┘
```

## Estados de la Aplicación

```
     ┌──────────────┐
     │   INICIO     │
     │  (Splash)    │
     └──────┬───────┘
            │
            ▼
     ┌──────────────┐         Long Press
     │   DISPLAY    │◀───────┐  (>1s)
     │     COCHE    │         │  Esquina
     │              │         │
     │  - Ruedas    │         │
     │  - Volante   │         │
     │  - Pedal     │         │
     │  - Batería   │         │
     └──────┬───────┘         │
            │                 │
            │ Long Press      │
            │   (>1s)         │
            │  Esquina        │
            ▼                 │
     ┌──────────────┐         │
     │     MENÚ     │─────────┘
     │ CALIBRACIÓN  │
     │              │
     │  - Sensores  │
     │  - On/Off    │
     │  - Valores   │
     │  - Rangos    │
     └──────────────┘
```

## Mapeo de Memoria

```
Flash (4MB):
┌─────────────────────────────┐ 0x00000000
│  Bootloader                 │
├─────────────────────────────┤
│  Partition Table            │
├─────────────────────────────┤
│  NVS (no usado aún)         │
├─────────────────────────────┤
│  OTA Data                   │
├─────────────────────────────┤
│  App0 (Firmware)            │
│  - main.cpp                 │
│  - CarDisplay.cpp           │
│  - CalibrationMenu.cpp      │
│  - TFT_eSPI lib            │
│  - XPT2046 lib             │
├─────────────────────────────┤
│  SPIFFS/LittleFS (opcional) │
└─────────────────────────────┘

RAM (512KB):
┌─────────────────────────────┐
│  Arduino Core               │
├─────────────────────────────┤
│  TFT Buffer (~307KB)        │
├─────────────────────────────┤
│  Stack (~50KB)              │
├─────────────────────────────┤
│  Heap (~150KB libre)        │
│  - Variables globales       │
│  - Objetos dinámicos        │
└─────────────────────────────┘
```

## Cronograma de Señales

```
Tiempo (ms):  0    10   20   30   40   50   60   70   80   90   100
              │    │    │    │    │    │    │    │    │    │    │
SPI TFT:      ████████████████████████████████████████████████████ (continuo)
              │    │    │    │    │    │    │    │    │    │    │
ADC:          ▓    │    ▓    │    ▓    │    ▓    │    ▓    │    ▓
              │    │    │    │    │    │    │    │    │    │    │
Touch:        ░    ░    ░    ░    ░    ░    ░    ░    ░    ░    ░ (polling)
              │    │    │    │    │    │    │    │    │    │    │
Encoder:      INT──INT────────INT──────────────────INT──────INT─── (eventos)
              │    │    │    │    │    │    │    │    │    │    │
Display:      │    │    │    │    │    │    │    │    │    │    █ (update)
              └────┴────┴────┴────┴────┴────┴────┴────┴────┴────┘

Leyenda:
█ = Activo continuo
▓ = Pulso activo
░ = Polling periódico
INT = Interrupción
```

## Prioridades de Tareas

```
Prioridad Alta (Interrupciones):
┌────────────────────────────┐
│ Encoder A/B (GPIO 26/27)   │ ◀── Crítico (pérdida de pasos)
│ Button (GPIO 32)           │ ◀── Alta (UX)
│ Touch IRQ (GPIO 25)        │ ◀── Media (opcional)
└────────────────────────────┘

Prioridad Media (Loop):
┌────────────────────────────┐
│ Lectura ADC (sensores)     │ ◀── 10 Hz
│ Procesamiento encoder      │ ◀── Inmediato
│ Detección gestos táctiles  │ ◀── Continuo
└────────────────────────────┘

Prioridad Baja (Loop):
┌────────────────────────────┐
│ Actualización display      │ ◀── 10 Hz (limitado)
│ Animaciones                │ ◀── 10 Hz
│ Procesamiento de menú      │ ◀── On demand
└────────────────────────────┘
```
