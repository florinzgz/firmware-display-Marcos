# Diagrama de Conexiones

## Pantalla TFT ILI9488

```
ESP32-S3          ILI9488
-------          --------
GPIO 13 -------> SDI (MOSI)
GPIO 12 <------- SDO (MISO)
GPIO 14 -------> SCK
GPIO 15 -------> CS
GPIO 2  -------> DC
GPIO 4  -------> RST
3.3V    -------> VCC
GND     -------> GND
```

## Panel Táctil XPT2046

```
ESP32-S3          XPT2046
-------          ---------
GPIO 13 -------> T_DIN (compartido con TFT MOSI)
GPIO 12 <------- T_DO  (compartido con TFT MISO)
GPIO 14 -------> T_CLK (compartido con TFT SCK)
GPIO 33 -------> T_CS
GPIO 25 <------- T_IRQ
3.3V    -------> VCC
GND     -------> GND
```

## Encoder Rotatorio

```
ESP32-S3          Encoder
-------          --------
GPIO 26 <------- A
GPIO 27 <------- B
GPIO 32 <------- SW (botón)
GND     -------> GND
```

## Sensores Analógicos

```
ESP32-S3          Sensor
-------          --------
GPIO 34 <------- Potenciómetro Dirección
GPIO 35 <------- Potenciómetro Acelerador
GPIO 36 <------- Potenciómetro Freno
GPIO 39 <------- Divisor de Voltaje Batería
```

### Nota sobre Sensores de Batería
Para medir el voltaje de batería, usar un divisor de voltaje:

```
Batería+ ---[R1=10kΩ]---+--- GPIO 39
                         |
                      [R2=10kΩ]
                         |
                        GND
```

Con esta configuración:
- Batería 4.2V -> GPIO 2.1V (dentro del rango del ADC)
- Batería 3.0V -> GPIO 1.5V
