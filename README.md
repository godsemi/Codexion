[README.md](https://github.com/user-attachments/files/31735919/README.md)
# Codexion

*Este proyecto ha sido creado como parte del currículo de 42 por jopaz-su*

## Descripción

**Codexion** es un proyecto de programación concurrente desarrollado en C como parte del currículo de 42.

El objetivo del proyecto es simular un grupo de personas programadoras que necesitan acceder a recursos compartidos, llamados **dongles**, para poder compilar su código.

Cada coder funciona como un hilo independiente (`pthread`) y debe realizar repetidamente las siguientes acciones:

1. Esperar hasta poder obtener los dos dongles que necesita.
2. Compilar utilizando ambos dongles.
3. Liberar los dongles.
4. Realizar otras tareas, como debugging y refactoring.
5. Volver a solicitar los dongles para realizar una nueva compilación.

La dificultad principal del proyecto reside en gestionar correctamente la concurrencia entre todos los threads. Varios coders pueden solicitar simultáneamente recursos que se solapan, por lo que es necesario coordinar el acceso mediante mutexes, variables de condición y un sistema de planificación basado en prioridades.

El proyecto implementa dos políticas de planificación:

* **FIFO (First In, First Out):** las solicitudes se atienden según el orden en el que fueron creadas.
* **EDF (Earliest Deadline First):** las solicitudes con la deadline más próxima tienen prioridad.

Para gestionar estas solicitudes se utiliza un **heap de prioridad**, que permite determinar qué coder debe tener prioridad para obtener sus recursos.

Además, cada dongle tiene un periodo de **cooldown** después de ser liberado. Durante este periodo no puede volver a utilizarse inmediatamente.

La simulación también dispone de un **monitor thread**, encargado de comprobar el estado de los coders y detectar situaciones de burnout. Un coder sufre burnout si pasa demasiado tiempo sin completar una nueva compilación.

El objetivo final de la simulación es que todos los coders completen el número de compilaciones requerido sin que se produzcan condiciones de carrera, deadlocks, starvation u otros problemas de sincronización.

## Características

* Implementación completamente realizada en C.
* Uso de POSIX threads mediante `pthread`.
* Gestión concurrente de múltiples coders.
* Gestión de recursos compartidos mediante mutexes.
* Comunicación entre threads mediante variables de condición.
* Sistema de solicitudes basado en un heap de prioridad.
* Planificación FIFO.
* Planificación EDF.
* Cooldown individual de los dongles.
* Detección de burnout mediante un monitor independiente.
* Control global del estado de la simulación.
* Serialización de los logs para evitar mensajes mezclados.
* Limpieza de recursos y finalización controlada de los threads.

## Estructura del proyecto

El código principal se encuentra en el directorio `coders/`.

### `main.c`

Punto de entrada del programa.

Se encarga de coordinar las diferentes fases:

* Validación de los argumentos.
* Inicialización de la simulación.
* Inicio de los threads.
* Espera de la finalización.
* Liberación de recursos.

### `parser.c`

Se encarga de analizar y validar los argumentos recibidos por línea de comandos.

Comprueba los valores numéricos y la política de planificación seleccionada.

### `init.c`

Inicializa las estructuras utilizadas por la simulación:

* Coders.
* Dongles.
* Heap.
* Mutexes.
* Variables de condición.
* Estado global de la simulación.

También asigna a cada coder los dos dongles que necesita para compilar.

### `simulation.c`

Gestiona el ciclo de vida de la simulación.

Crea los threads correspondientes a los coders y el monitor, y posteriormente espera su finalización mediante `pthread_join()`.

### `coder.c`

Contiene la lógica principal de los coders.

Gestiona:

* Solicitud de dongles.
* Espera por recursos.
* Compilación.
* Liberación de dongles.
* Cooldown.
* Debugging.
* Refactoring.
* Actualización del contador de compilaciones.

### `heap.c`, `heap2.c` y `heap3.c`

Implementan la gestión de las solicitudes de los coders.

El heap permite mantener las solicitudes ordenadas según la política seleccionada:

* FIFO: prioridad basada en el orden de solicitud.
* EDF: prioridad basada en la deadline.

También contienen la lógica necesaria para comprobar si una solicitud puede ejecutarse teniendo en cuenta la disponibilidad de los dongles y las solicitudes de los demás coders.

### `utils.c` y `utils2.c`

Contienen funciones auxiliares relacionadas con el tiempo, el estado de la simulación y otras operaciones comunes.

### `codexion.h`

Contiene las estructuras principales y las declaraciones compartidas por los diferentes módulos del proyecto.

Entre las estructuras principales se encuentran:

* `t_sim`
* `t_coder`
* `t_dongle`
* `t_request`
* `t_heap`

## Instrucciones

### Requisitos

El proyecto necesita:

* Un compilador compatible con C.
* POSIX threads (`pthread`).
* Un sistema compatible con las funciones POSIX utilizadas por el proyecto.

En Linux, no es necesario instalar ninguna librería externa adicional.

### Compilación

Desde la raíz del repositorio:

```bash
make
```

Esto genera el ejecutable:

```bash
./codexion
```

Para eliminar los archivos objeto:

```bash
make clean
```

Para eliminar los archivos objeto y el ejecutable:

```bash
make fclean
```

Para recompilar completamente:

```bash
make re
```

### Ejecución

El programa recibe los siguientes argumentos:

```text
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Donde:

| Argumento                     | Descripción                                                                      |
| ----------------------------- | -------------------------------------------------------------------------------- |
| `number_of_coders`            | Número de coders que participan en la simulación.                                |
| `time_to_burnout`             | Tiempo máximo que puede pasar un coder sin completar una compilación.            |
| `time_to_compile`             | Tiempo necesario para realizar una compilación.                                  |
| `time_to_debug`               | Tiempo dedicado a debugging.                                                     |
| `time_to_refactor`            | Tiempo dedicado a refactoring.                                                   |
| `number_of_compiles_required` | Número de compilaciones que debe completar cada coder.                           |
| `dongle_cooldown`             | Tiempo que debe pasar antes de que un dongle liberado pueda volver a utilizarse. |
| `scheduler`                   | Política de planificación: `fifo` o `edf`.                                       |

### Ejemplo

```bash
./codexion 5 1000 300 300 300 2 100 fifo
```

Este ejemplo crea una simulación con:

* 5 coders.
* 1000 ms antes del burnout.
* 300 ms de compilación.
* 300 ms de debugging.
* 300 ms de refactoring.
* 2 compilaciones requeridas por coder.
* 100 ms de cooldown para los dongles.
* Planificación FIFO.

Para utilizar EDF:

```bash
./codexion 5 1000 300 300 300 2 100 edf
```

## Blocking cases handled

La implementación tiene en cuenta los principales problemas de concurrencia derivados de la utilización de recursos compartidos.

### Deadlock e interbloqueos

Un coder necesita dos dongles para poder compilar. Una adquisición independiente de cada dongle podría provocar una situación en la que varios coders retengan un dongle mientras esperan otro.

La solución evita este problema mediante un sistema de solicitudes y una decisión de adquisición que comprueba que los dos recursos necesarios pueden obtenerse antes de permitir que el coder comience a compilar.

De esta forma, no se mantiene un dongle indefinidamente mientras se espera a obtener el segundo.

Esto evita las condiciones clásicas de Coffman asociadas a la espera circular de recursos.

### Mutual exclusion

Los dongles son recursos compartidos y su estado no puede ser modificado simultáneamente por varios threads.

El acceso a ellos está protegido mediante `pthread_mutex_t`, garantizando que las operaciones de comprobación, adquisición y liberación se realizan de forma sincronizada.

Por ejemplo, una comprobación del estado de los dongles y su posterior adquisición se realizan dentro de una sección crítica, evitando que dos coders observen simultáneamente un dongle como disponible y ambos intenten adquirirlo.

### Starvation

Una política de planificación incorrecta podría provocar que un coder espere indefinidamente mientras otros coders reciben los recursos repetidamente.

El sistema utiliza un heap de prioridad para mantener las solicitudes ordenadas.

Con `FIFO`, una solicitud conserva su prioridad según el momento en que fue creada.

Con `EDF`, la prioridad se determina según la deadline de cada coder, dando prioridad a los coders que están más cerca de alcanzar su límite de burnout.

Esto permite controlar la espera de los coders y evitar que las solicitudes más antiguas o urgentes queden indefinidamente relegadas.

### Cooldown de los dongles

Un dongle no vuelve a estar disponible inmediatamente después de ser liberado.

Al liberarse, se registra el momento a partir del cual podrá volver a utilizarse:

```text
available_at = current_time + dongle_cooldown
```

Durante este periodo, el dongle se considera no disponible.

Las solicitudes comprueban esta condición antes de poder adquirirlo, evitando que un coder utilice un recurso antes de que termine su cooldown.

### Detección precisa del burnout

Cada coder registra el momento de su última compilación.

El monitor compara periódicamente ese instante con el tiempo actual:

```text
current_time - last_compile_time
```

Si el resultado supera `time_to_burnout`, el coder sufre burnout y la simulación se detiene.

La utilización de timestamps basados en tiempo monotónico permite realizar estas comparaciones sin depender de cambios en el reloj del sistema.

Las deadlines utilizadas por EDF también se derivan de estos tiempos:

```text
deadline = last_compile_time + time_to_burnout
```

### Serialización del log

Varios coders pueden intentar escribir en la salida estándar simultáneamente.

Sin sincronización, los mensajes podrían aparecer mezclados o intercalados.

El acceso al logging se serializa mediante un mutex, garantizando que cada mensaje se imprime de forma completa antes de que otro thread pueda escribir.

Esto mantiene la salida legible y consistente incluso cuando múltiples coders cambian de estado al mismo tiempo.

### Finalización de la simulación

La simulación puede finalizar por dos motivos principales:

1. Todos los coders han completado el número requerido de compilaciones.
2. Un coder sufre burnout.

El estado global de ejecución se protege mediante sincronización para que todos los threads puedan detectar correctamente el final de la simulación.

Las variables de condición se utilizan para despertar a los threads que podrían estar esperando recursos cuando el estado de la simulación cambia.

## Thread synchronization mechanisms

La implementación utiliza diferentes primitivas POSIX para coordinar los threads.

### `pthread_mutex_t`

Los mutexes se utilizan para proteger los datos compartidos que pueden ser modificados por varios threads.

Entre los recursos protegidos se encuentran:

* Estado de los dongles.
* Heap de solicitudes.
* Estado global de la simulación.
* Logging.

La estructura general de una sección crítica es:

```c
pthread_mutex_lock(&mutex);

/* acceso/modificación del recurso compartido */

pthread_mutex_unlock(&mutex);
```

Esto garantiza que solamente un thread pueda ejecutar simultáneamente la sección crítica correspondiente.

### Protección de los dongles

Antes de comprobar o modificar el estado de los dongles, el coder adquiere el mutex correspondiente.

Por ejemplo, conceptualmente:

```text
Coder A                    Coder B
   │                          │
   ├── lock                  │
   │                          ├── espera
   │
   ├── comprueba dongles     │
   ├── adquiere dongles      │
   ├── unlock                │
   │                          ├── lock
   │                          └── comprueba nuevo estado
```

De esta manera se evita que dos threads modifiquen simultáneamente el mismo estado.

### `pthread_cond_t`

Las variables de condición se utilizan para evitar que los coders tengan que realizar polling continuamente mientras esperan un recurso.

Cuando un coder no puede continuar, puede esperar mediante una condición:

```c
pthread_cond_wait(&condition, &mutex);
```

o mediante una espera con timeout:

```c
pthread_cond_timedwait(&condition, &mutex, &timeout);
```

Cuando un thread libera recursos o se produce un cambio relevante, puede despertar a los threads que están esperando:

```c
pthread_cond_broadcast(&condition);
```

Los threads despertados vuelven a comprobar las condiciones antes de continuar.

Esto es importante porque despertarse de una condition variable **no implica automáticamente que el recurso esté disponible**. El estado siempre se vuelve a comprobar bajo el mutex.

### Comunicación entre coders

Los coders comunican indirectamente sus cambios de estado mediante los recursos compartidos y las variables de condición.

Por ejemplo:

```text
Coder A
  │
  ├── termina de compilar
  │
  ├── libera dongles
  │
  └── signal/broadcast
          │
          ▼
     Coder B / Coder C
          │
          └── vuelven a comprobar
              sus solicitudes
```

Esto permite que los coders reaccionen a la liberación de recursos sin necesidad de realizar una espera activa.

### Comunicación entre coders y monitor

El monitor observa el estado compartido de los coders, especialmente el momento de su última compilación.

El acceso a esta información se realiza de forma sincronizada para evitar que el monitor lea un valor mientras otro thread lo está modificando.

El flujo general es:

```text
             ┌──────────────────┐
             │      MONITOR     │
             └────────┬─────────┘
                      │
                 comprueba
                      │
                      ▼
              estado de coders
                      ▲
                      │
          ┌───────────┴───────────┐
          │                       │
       CODER 1                 CODER 2
          │                       │
       compile                  compile
          │                       │
     actualiza                 actualiza
     timestamp                 timestamp
```

Cuando el monitor detecta un burnout, modifica el estado global de la simulación y notifica a los threads que puedan estar esperando.

### Implementación personalizada de eventos

La comunicación de determinados cambios de estado se apoya en una abstracción propia de eventos construida sobre las primitivas de pthread.

Estos eventos permiten separar la lógica de sincronización de la lógica específica de la simulación.

El objetivo es proporcionar una forma thread-safe de notificar que ha ocurrido un cambio, manteniendo la coordinación entre los coders, el scheduler y el monitor.

## Decisiones técnicas

### Uso de un heap de prioridad

Se utiliza un heap porque el scheduler necesita seleccionar constantemente la solicitud con mayor prioridad.

La prioridad depende del scheduler seleccionado:

```text
FIFO
request más antigua
       ↓
mayor prioridad
```

o:

```text
EDF
deadline más próxima
       ↓
mayor prioridad
```

El heap permite insertar nuevas solicitudes y recuperar la solicitud prioritaria de manera eficiente.

### Separación entre solicitud y adquisición

Los coders no adquieren directamente los dongles al comenzar una compilación.

Primero crean una solicitud.

Esto permite que el scheduler tenga una visión global de las peticiones pendientes y pueda tomar decisiones teniendo en cuenta:

* Prioridad.
* Deadline.
* Disponibilidad de los dongles.
* Conflictos entre solicitudes.

### Monitor independiente

La detección de burnout se realiza desde un thread separado.

Esto permite que los coders se concentren en su ciclo de trabajo mientras el monitor supervisa continuamente las deadlines.

## Recursos

### Documentación

* `pthread_create` y creación de threads POSIX.
* `pthread_join` para esperar la finalización de threads.
* `pthread_mutex_t` y mutexes POSIX.
* `pthread_cond_t`, `pthread_cond_wait` y `pthread_cond_timedwait`.
* Documentación de `clock_gettime` y medición de tiempo monotónico.
* Documentación de POSIX Threads.

### Concurrencia

* The Open Group — POSIX Threads.
* *The Linux Programming Interface* — Michael Kerrisk.
* Documentación de `pthreads` de Linux.
* Material educativo de 42 relacionado con concurrencia y programación multihilo.

### Planificación

El diseño del scheduler está relacionado con algoritmos clásicos de planificación de procesos:

* **FIFO / FCFS (First Come, First Served)**.
* **EDF (Earliest Deadline First)**.
* Priority Queues.
* Binary Heaps.

### IA

Se ha utilizado inteligencia artificial como herramienta de apoyo durante el desarrollo del proyecto.

Su utilización ha estado orientada principalmente a:

* Comprender conceptos de programación concurrente y POSIX threads.
* Analizar problemas relacionados con mutexes y variables de condición.
* Revisar posibles condiciones de carrera y deadlocks.
* Ayudar a razonar sobre la implementación del scheduler FIFO/EDF.
* Revisar y explicar errores de compilación y comportamiento.
* Analizar la organización del código y proponer mejoras de estructura.
* Facilitar la comprensión y depuración de determinadas partes del proyecto.

La IA se ha utilizado como **herramienta de asistencia y aprendizaje**, mientras que la implementación, integración, pruebas y validación final del código corresponden al trabajo realizado en el proyecto.

## Licencia

Este proyecto ha sido desarrollado como parte del currículo de 42 y está destinado principalmente a fines educativos.

