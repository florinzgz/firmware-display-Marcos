# Advanced HUD - Visual Layout Reference

## Vista Completa del HUD (480x320 px)

```
┌──────────────────────────────────────────────────────────────────────────────┐
│ Temp: 23°C                                            Bat: 24.5V 95%   [0,0] │
├──────────────────────────────────────────────────────────────────────────────┤
│                                                                          [0,20]│
│   [20,40]                                                                     │
│   ┌──────────────────────────────────────────────┐                           │
│   │  VISTA CENITAL COCHE MERCEDES (240x200)     │         [340,40]          │
│   │                                              │    ┌────────────┐         │
│   │    [60,60]  ┌─────┐     ┌─────┐ [210,60]   │    │            │         │
│   │      FL     │░░85%│     │░░90%│     FR      │    │  Velocímetro│         │
│   │            ─│░░░░░│─────│░░░░░│─           │    │    30 km/h  │         │
│   │             │ 35°C│  ╱  │ 32°C│             │    │   ┌──●──┐  │         │
│   │             └──┬──┘ ║☆║ └──┬──┘             │    │  ╱   │   ╲ │         │
│   │                │    ╚═╝   │                 │    │ ╱    │    ╲│         │
│   │                └────┐ │ ┌─┘                 │    └────────────┘         │
│   │               [140,100]│                     │    R=60, aguja roja       │
│   │               LOGO ☆  │                     │                           │
│   │                       │                     │         [340,170]         │
│   │                 [140,140]                   │    ┌────────────┐         │
│   │                   ◯←45°                     │    │            │         │
│   │                 VOLANTE                     │    │  Tacómetro  │         │
│   │                                              │    │   220 RPM   │         │
│   │      RL     ┌─────┐     ┌─────┐     RR      │    │   ┌──●──┐  │         │
│   │    [60,180] │░░78%│     │░░82%│ [210,180]  │    │  ╱   │   ╲ │         │
│   │             │░░░░░│─────│░░░░░│             │    │ ╱    │    ╲│         │
│   │             │ 38°C│     │ 36°C│             │    └────────────┘         │
│   │             └─────┘     └─────┘             │    R=60, aguja verde      │
│   │                                              │                           │
│   └──────────────────────────────────────────────┘                           │
│                                                                          [0,240]│
│                                                                               │
│   [100,290]                        [280,290]                            [0,290]│
│   ┌────────────────────────┐       ┌───────────────────────────┐            │
│   │ P │ D2│ D1│ N │ R      │       │ Pedal: ████████░░ 80%     │            │
│   │   │   │   │   │        │       │                           │            │
│   │   │▓▓▓│   │   │        │       │ Azul→Verde→Rojo          │            │
│   └────────────────────────┘       └───────────────────────────┘            │
│   Shifter 160x25                   Barra Pedal 180x25                  [0,315]│
└──────────────────────────────────────────────────────────────────────────[480,320]
```

## Detalles de Componentes

### 1. Rueda Individual (50x40 px)

```
┌──────────────┐
│ FL          │  ← Etiqueta (cyan)
│ ░░░░░░░░░░  │  ← Barra esfuerzo 85%
│ ████████░░  │     (color dinámico)
│ 35°C        │  ← Temperatura (color según temp)
└──────────────┘
```

**Colores Esfuerzo/Temperatura:**
- 🟢 Verde: 0-25% / <35°C
- 🟡 Amarillo: 26-50% / 35-45°C
- 🟠 Naranja: 51-75% / 46-55°C
- 🔴 Rojo: 76-100% / >55°C

### 2. Logo Mercedes (R=15 px)

```
    ╱ ╲      Estrella de 3 puntas
   ╱   ╲     Color: Dorado (0xFEA0)
  ╱  ★  ╲    Centro: Golden
 ╱   │   ╲   Círculo: Plata Mercedes
◯────┼────◯  
     │        
```

### 3. Volante con Ángulo (R=20 px)

```
Posición Central (0°):    Girado Derecha (+45°):   Girado Izquierda (-45°):
       │                         ╱                        ╲
       │                        ╱                          ╲
    ───●───                  ───●───                    ───●───
       │                                                    
       │                                                    
     45°                                                    
```

### 4. Cardanes Ackermann

```
         Volante
            ●
           ╱ ╲        Líneas conectando
          ╱   ╲       volante a ruedas
         ╱     ╲      delanteras
     FL ●       ● FR  
     
     Ángulo FL ≠ FR  (geometría Ackermann)
     Rueda interior gira más
```

### 5. Velocímetro/Tacómetro (R=60 px)

```
        Marcas
    ╱     │     ╲
   ╱      │      ╲     270° de recorrido
  │       │       │    -135° a +135°
  │       25      │    Aguja con sombra
  │     ╱    ╲   │    Centro: valor digital
  │    ╱  ●   ╲  │    
   ╲  ╱   │    ╲╱     
    ╲╱   km/h  ╲      Label inferior
```

**Velocímetro**: Aguja roja, 0-30 km/h
**Tacómetro**: Aguja verde, 0-220 RPM

### 6. Shifter (160x25 px)

```
┌────┬────┬────┬────┬────┐
│ P  │ D2 │ D1 │ N  │ R  │
│    │▓▓▓▓│    │    │    │  ← D1 activo (fondo azul, texto cyan)
└────┴────┴────┴────┴────┘

Ancho por posición: 32 px
Separadores verticales
```

### 7. Barra de Pedal (180x25 px)

```
Pedal: ████████████████░░░░░░ 80%
       ├─────────┬──────────┤
       Azul     Verde      Rojo
       0-33%    34-66%   67-100%
```

## Paleta de Colores Completa

### Tema Mercedes

| Color | Hex | RGB565 | Uso |
|-------|-----|--------|-----|
| Mercedes Silver | #BDBDBD | 0xC618 | Bordes coche, círculo logo |
| Mercedes Blue | #003366 | 0x0356 | Fondo shifter activo |
| Star Gold | #FFD700 | 0xFEA0 | Estrella Mercedes |
| Gauge Red | #FF0000 | 0xF800 | Aguja velocímetro |
| Gauge Green | #00FF00 | 0x07E0 | Aguja tacómetro |
| HUD Background | #000000 | 0x0000 | Fondo negro |
| HUD Panel | #1A1A1A | 0x18C3 | Paneles gris oscuro |
| HUD Accent | #00FFFF | 0x07FF | Cyan elementos activos |

### Colores Dinámicos

| Estado | Temp °C | Esfuerzo % | Hex | RGB565 |
|--------|---------|------------|-----|--------|
| OK | <35 | 0-25 | #00FF00 | 0x07E0 |
| Warning | 35-45 | 26-50 | #FFFF00 | 0xFFE0 |
| Caution | 46-55 | 51-75 | #FFA500 | 0xFD20 |
| Critical | >55 | 76-100 | #FF0000 | 0xF800 |

### Colores Pedal

| Rango | Color | Hex | RGB565 | Descripción |
|-------|-------|-----|--------|-------------|
| 0-33% | Azul Mercedes | #003366 | 0x0356 | Aceleración suave |
| 34-66% | Verde | #00FF00 | 0x07E0 | Aceleración media |
| 67-100% | Rojo | #FF0000 | 0xF800 | Aceleración máxima |

## Dimensiones Exactas

### Componentes Principales

| Elemento | X | Y | W | H | R | Notas |
|----------|---|---|---|---|---|-------|
| Temp Ambiente | 10 | 5 | 100 | 15 | - | Texto size 1 |
| Batería | 360 | 5 | 110 | 15 | - | Texto size 1 |
| Vista Coche | 20 | 40 | 240 | 200 | - | Panel principal |
| Rueda FL | 60 | 60 | 50 | 40 | 4 | Roundrect |
| Rueda FR | 210 | 60 | 50 | 40 | 4 | Roundrect |
| Rueda RL | 60 | 180 | 50 | 40 | 4 | Roundrect |
| Rueda RR | 210 | 180 | 50 | 40 | 4 | Roundrect |
| Logo Mercedes | 140 | 100 | - | - | 15 | Círculo + estrella |
| Volante | 140 | 140 | - | - | 20 | Círculo |
| Velocímetro | 400 | 100 | - | - | 60 | Gauge circular |
| Tacómetro | 400 | 230 | - | - | 60 | Gauge circular |
| Shifter | 100 | 290 | 160 | 25 | 4 | Roundrect |
| Pedal | 280 | 290 | 180 | 25 | - | Barra + texto |

### Sub-componentes Rueda

| Elemento | Offset X | Offset Y | W | H | Notas |
|----------|----------|----------|---|---|-------|
| Label | +2 | +2 | - | - | Font size 1 |
| Barra esfuerzo | +2 | +14 | 46 | 6 | Progreso horizontal |
| % Esfuerzo | +2 | +22 | - | - | Font size 1 |
| Temperatura | +2 | +31 | - | - | Font size 1 |

## Flujo de Actualización

```
Loop Principal (cada 10ms)
    │
    ├─→ [50ms] Pedal, RPM, Encoder (20 Hz)
    │     │
    │     └─→ Actualizar hudData.pedalPosition
    │          Actualizar hudData.rpm
    │          Actualizar hudData.encoderAngle
    │
    ├─→ [100ms] Velocidad, INA226 x4 (10 Hz)
    │     │
    │     └─→ Actualizar hudData.speed
    │          Leer 4 canales INA226
    │          Calcular esfuerzos
    │          Calcular ángulos Ackermann
    │
    ├─→ [500ms] Batería, Temperatura (2 Hz)
    │     │
    │     └─→ Actualizar hudData.batteryVoltage
    │          Actualizar hudData.batteryPercent
    │          Actualizar hudData.ambientTemp
    │
    └─→ [50ms] Update HUD (20 FPS)
          │
          ├─→ Detectar cambios (threshold)
          │     │
          │     ├─→ Temp >0.5°C → Redibujar
          │     ├─→ Batería >0.1V → Redibujar
          │     ├─→ Speed >0.5 km/h → Redibujar
          │     ├─→ RPM >5 → Redibujar
          │     ├─→ Gear cambió → Redibujar
          │     ├─→ Pedal → Siempre redibujar
          │     └─→ Encoder >2° → Redibujar vista coche
          │
          └─→ Dibujar solo elementos cambiados
```

## Ejemplo de Renderizado

### Frame Típico en Movimiento

```
Estado:
- Velocidad: 18.5 km/h
- RPM: 145
- Pedal: 65% (verde)
- Encoder: +30° (giro derecha)
- Marcha: D1
- Batería: 24.2V, 87%
- Temp Ambiente: 23°C

Ruedas:
- FL: 72% esfuerzo, 42°C (naranja), +33° dirección
- FR: 68% esfuerzo, 39°C (amarillo), +27° dirección (Ackermann)
- RL: 65% esfuerzo, 41°C (naranja)
- RR: 63% esfuerzo, 38°C (amarillo)

Actualizaciones este frame:
✓ Pedal (siempre)
✓ Velocímetro (cambió 0.8 km/h)
✓ Vista coche (encoder cambió 3°)
✗ Tacómetro (cambio <5 RPM)
✗ Batería (cambio <0.1V)
✗ Temperatura (cambio <0.5°C)
✗ Shifter (sin cambio)
```

## Mapa de Memoria Visual

```
Áreas de Pantalla por Prioridad de Actualización:

Alta (cada frame):
  [████] Pedal bar (280,290 - 460,315)

Media-Alta (cambios frecuentes):
  [▓▓▓▓] Vista coche (20,40 - 260,240)
  [▓▓▓▓] Velocímetro (340,40 - 460,160)
  [▓▓▓▓] Tacómetro (340,170 - 460,290)

Baja (cambios raros):
  [░░░░] Batería (360,5 - 470,20)
  [░░░░] Temperatura (10,5 - 110,20)
  [░░░░] Shifter (100,290 - 260,315)

Total píxeles: 480 x 320 = 153,600
Actualización típica: ~30,000 píxeles/frame (20%)
```

## Leyenda de Símbolos

```
● = Punto/Centro
◯ = Círculo vacío
☆ = Estrella Mercedes
░ = Barra vacía
█ = Barra llena
▓ = Elemento activo
─ = Línea horizontal
│ = Línea vertical
╱╲ = Líneas diagonales
┌┐└┘ = Esquinas
├┤┬┴┼ = Conectores
```

---

**Notas de Implementación:**

1. Todos los elementos usan coordenadas absolutas desde (0,0) = esquina superior izquierda
2. Los gauges circulares tienen 270° de recorrido (-135° a +135°)
3. El ángulo 0° del encoder representa volante centrado
4. Los colores dinámicos se calculan en tiempo de ejecución según valores
5. La geometría Ackermann hace que la rueda interior gire 10% más que la exterior
6. Las sombras de las agujas usan offset de 1 pixel con color gris oscuro
