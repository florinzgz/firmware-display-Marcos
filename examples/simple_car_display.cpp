/**
 * Ejemplo de uso simple de la clase CarDisplay
 * 
 * Este ejemplo muestra cómo usar la clase CarDisplay para
 * visualizar un coche con valores estáticos.
 */

#include <Arduino.h>
#include <TFT_eSPI.h>
#include "CarDisplay.h"

TFT_eSPI tft = TFT_eSPI();
CarDisplay car(&tft);

void setup() {
    Serial.begin(115200);
    
    // Inicializar TFT
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_BLACK);
    
    // Inicializar display del coche
    car.begin();
    
    // Configurar valores iniciales
    car.setWheelAngle(15.0);      // 15 grados a la derecha
    car.setWheelRotation(0.0);    // Sin rotación
    car.setPedalPosition(0.5);    // 50% de aceleración
    car.setBatteryLevel(0.75);    // 75% de batería
    
    // Dibujar
    car.update();
}

void loop() {
    // Animar el ángulo del volante
    static float angle = 0.0;
    static float direction = 1.0;
    
    angle += direction * 0.5;
    if (angle > 45.0 || angle < -45.0) {
        direction *= -1.0;
    }
    
    car.setWheelAngle(angle);
    
    // Animar la rotación de las ruedas
    static float rotation = 0.0;
    rotation += 0.1;
    if (rotation > 2 * PI) rotation -= 2 * PI;
    car.setWheelRotation(rotation);
    
    car.update();
    delay(50);
}
