# Manual de Usuario

## Inicio del Sistema

Al encender el ESP32-S3, el sistema realiza la siguiente secuencia:

1. Inicializa la pantalla TFT
2. Inicializa el panel táctil
3. Configura el encoder rotatorio
4. Configura los pines de entrada analógicos
5. Muestra la pantalla principal con el coche

## Pantalla Principal

### Vista del Coche
La pantalla muestra un coche visto desde arriba (top-down) con los siguientes elementos:

#### 1. Cuerpo del Coche (Centro)
- Color azul con bordes redondeados
- Muestra parabrisas y ventana trasera
- Dimensiones: 120x180 píxeles

#### 2. Ruedas (4 esquinas)
- **Ruedas delanteras**: Giran según el ángulo de dirección
- **Ruedas traseras**: Siempre rectas
- **Animación de rotación**: Las ruedas rotan cuando hay aceleración
- Indicadores visuales en el centro de cada rueda

#### 3. Indicador de Volante (Superior Derecha)
- Círculo naranja con radios
- Muestra el ángulo de dirección en grados (-45° a +45°)
- Se actualiza en tiempo real según el sensor de dirección

#### 4. Indicador de Pedal (Inferior Izquierda)
- Barra vertical verde
- Muestra la posición del acelerador (0-100%)
- Se llena desde abajo según la presión del pedal

#### 5. Indicador de Batería (Superior Izquierda)
- Símbolo de batería con porcentaje
- **Códigos de color**:
  - Verde: >50% de carga
  - Naranja: 20-50% de carga
  - Rojo: <20% de carga (crítico)

## Acceso al Menú de Calibración

### Cómo Abrir el Menú
1. Toque y mantenga presionada la esquina superior izquierda de la pantalla
2. Mantenga presionado durante más de 1 segundo
3. El menú de calibración aparecerá

### Cómo Cerrar el Menú
1. Toque y mantenga presionada nuevamente la esquina superior izquierda
2. O navegue con el encoder y seleccione "Salir"

## Navegación en el Menú

### Usando el Encoder Rotatorio
1. **Girar a la derecha**: Mover selección hacia abajo
2. **Girar a la izquierda**: Mover selección hacia arriba
3. **Presionar botón**: Activar/desactivar sensor seleccionado

### Elementos del Menú
Cada sensor muestra:
- **Nombre**: Identificador del sensor
- **Estado**: ON (verde) o OFF (rojo)
- **Valor actual**: Lectura en tiempo real del ADC
- **Rango**: Valores mínimo y máximo calibrados

## Calibración de Sensores

### Sensores Disponibles
1. **Steering (Dirección)**: Rango 0-4095
2. **Accelerator (Acelerador)**: Rango 0-4095
3. **Brake (Freno)**: Rango 0-4095
4. **Battery (Batería)**: Rango 0-100%

### Proceso de Calibración
1. Acceder al menú de calibración
2. Seleccionar el sensor con el encoder
3. Presionar prolongadamente el botón para entrar en modo calibración
4. Mover el sensor a su posición mínima
5. Presionar el botón para guardar el valor mínimo
6. Mover el sensor a su posición máxima
7. Presionar el botón para guardar el valor máximo
8. El sensor quedará calibrado

### Activar/Desactivar Sensores
1. Navegar al sensor deseado
2. Presionar brevemente el botón del encoder
3. El estado cambiará entre ON y OFF

**Nota**: Los sensores desactivados no afectan la visualización del coche.

## Interpretación de Valores

### Sensor de Dirección
- **0-2047**: Giro a la izquierda (hasta -45°)
- **2048**: Centro (0°)
- **2049-4095**: Giro a la derecha (hasta +45°)

### Sensor de Acelerador
- **0**: Sin aceleración
- **4095**: Aceleración máxima

### Sensor de Batería
- **100%**: Batería completamente cargada (4.2V)
- **50%**: Batería media (3.6V)
- **0%**: Batería descargada (3.0V)

## Solución de Problemas

### La pantalla muestra valores incorrectos
- Verificar que los sensores estén correctamente calibrados
- Comprobar las conexiones de los potenciómetros
- Reiniciar el sistema y recalibrar

### El táctil no responde
- Verificar que el panel táctil esté correctamente conectado
- Comprobar que GPIO 33 y 25 estén bien conectados
- Intentar tocar con más firmeza

### El encoder no funciona
- Verificar conexiones de GPIO 26, 27 y 32
- Comprobar que el encoder tenga tierra común
- Verificar que las resistencias pull-up estén activas

### Los valores saltan o son inestables
- Verificar que los cables de los sensores no estén cerca de cables de potencia
- Agregar capacitores de filtrado (0.1µF) en las entradas analógicas
- Verificar que la alimentación sea estable

## Características Avanzadas

### Animación de Ruedas
- Las ruedas rotan proporcionalmente a la aceleración
- El ángulo de rotación se acumula mientras hay aceleración
- Proporciona feedback visual del movimiento

### Actualización de Pantalla
- Frecuencia: 10 Hz (cada 100ms)
- Optimizado para evitar parpadeo
- Actualización suave de todos los elementos

### Gestión de Energía
- Sin modo de ahorro de energía implementado
- Pantalla siempre activa
- Consumo aproximado: ~500mA @ 3.3V
