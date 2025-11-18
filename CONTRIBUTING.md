# Guía de Contribución

¡Gracias por tu interés en contribuir a firmware-display-Marcos! Este documento proporciona directrices para contribuir al proyecto.

## Cómo Contribuir

### Reportar Bugs

Si encuentras un bug, por favor abre un issue con:
- Descripción clara del problema
- Pasos para reproducirlo
- Comportamiento esperado vs comportamiento actual
- Versión de hardware y software
- Logs o screenshots si es posible

### Sugerir Mejoras

Para sugerir una mejora:
1. Abre un issue describiendo la mejora
2. Explica por qué sería útil
3. Proporciona ejemplos de uso si es posible

### Pull Requests

1. Fork el repositorio
2. Crea una rama desde `main`:
   ```bash
   git checkout -b feature/mi-nueva-funcionalidad
   ```
3. Realiza tus cambios siguiendo las guías de estilo
4. Commit tus cambios con mensajes descriptivos
5. Push a tu fork
6. Abre un Pull Request

## Guías de Estilo

### Código C/C++

- **Indentación**: 4 espacios (no tabs)
- **Nombres de variables**: camelCase
- **Nombres de funciones**: camelCase
- **Nombres de clases**: PascalCase
- **Constantes**: UPPER_CASE
- **Comentarios**: En español, descriptivos

Ejemplo:
```cpp
class MiClase {
private:
    int miVariable;
    const int MI_CONSTANTE = 100;
    
public:
    void miFuncion() {
        // Comentario explicativo
        int resultado = miVariable + MI_CONSTANTE;
    }
};
```

### Documentación

- Documentación en español
- README actualizado con cambios significativos
- Comentarios de código donde sea necesario
- Actualizar CHANGELOG.md con cambios

### Commits

Formato de mensajes de commit:
```
<tipo>: <descripción breve>

<descripción detallada opcional>
```

Tipos:
- `feat`: Nueva funcionalidad
- `fix`: Corrección de bug
- `docs`: Cambios en documentación
- `style`: Formateo, punto y coma faltantes, etc
- `refactor`: Refactorización de código
- `test`: Añadir tests
- `chore`: Tareas de mantenimiento

Ejemplos:
```
feat: Agregar modo de ahorro de energía

Implementa apagado automático de backlight después de 5 minutos
de inactividad para reducir consumo de energía.
```

```
fix: Corregir parpadeo en actualización de pantalla

El buffer no se limpiaba correctamente causando artefactos visuales.
```

## Estructura del Proyecto

```
firmware-display-Marcos/
├── docs/              # Documentación técnica
├── examples/          # Ejemplos de código
├── include/           # Headers (.h)
├── src/              # Implementación (.cpp)
├── platformio.ini    # Configuración del proyecto
└── README.md         # Documentación principal
```

## Testing

Antes de enviar un PR:
1. Compilar el proyecto sin errores
2. Probar en hardware real si es posible
3. Verificar que no rompe funcionalidad existente
4. Documentar cambios en CHANGELOG.md

## Áreas de Mejora

Áreas donde las contribuciones son especialmente bienvenidas:

### Funcionalidades
- [ ] Persistencia de calibración en NVS/EEPROM
- [ ] Modo de ahorro de energía
- [ ] Calibración automática de pantalla táctil
- [ ] Soporte para más tipos de sensores
- [ ] Animaciones mejoradas
- [ ] Temas de colores personalizables

### Optimizaciones
- [ ] Renderizado diferencial (solo actualizar lo que cambió)
- [ ] Reducción de consumo de energía
- [ ] Mejora de precisión de encoder con filtrado
- [ ] Optimización de uso de memoria

### Documentación
- [ ] Traducción a otros idiomas
- [ ] Video tutoriales
- [ ] Más ejemplos de código
- [ ] Diagramas de circuitos detallados

### Hardware
- [ ] Soporte para otras pantallas (ILI9341, ST7789, etc)
- [ ] Soporte para ESP32 original (no solo S3)
- [ ] PCB personalizado (diseño opcional)

## Código de Conducta

### Nuestro Compromiso

Este proyecto se compromete a proporcionar un ambiente acogedor e inclusivo para todos.

### Comportamiento Esperado

- Usar lenguaje acogedor e inclusivo
- Ser respetuoso con diferentes puntos de vista
- Aceptar críticas constructivas de manera amable
- Enfocarse en lo mejor para la comunidad
- Mostrar empatía hacia otros miembros

### Comportamiento Inaceptable

- Comentarios ofensivos o discriminatorios
- Ataques personales
- Trolling o comentarios insultantes
- Acoso público o privado
- Publicar información privada de otros

## Licencia

Al contribuir, aceptas que tus contribuciones se licenciarán bajo la misma licencia MIT del proyecto.

## Preguntas

Si tienes preguntas sobre cómo contribuir, puedes:
- Abrir un issue con la etiqueta "question"
- Contactar al mantenedor del proyecto

## Reconocimientos

Todos los contribuidores serán reconocidos en el archivo CONTRIBUTORS.md (próximamente).

¡Gracias por contribuir! 🚗💨
