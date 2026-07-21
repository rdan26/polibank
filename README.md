# POLI BANK - Sistema de Cuentas y Transacciones Bancarias (Parte 2)

**Institución:** Escuela Politécnica Nacional (EPN)
**Asignatura:** Programación I
**Docente:** Ing. Thomás Borja, MSc.

## Descripción del Proyecto
POLI BANK es un sistema bancario digital desarrollado en C que simula la gestión transaccional y administrativa de una entidad financiera. Esta segunda etapa evoluciona la arquitectura inicial para incorporar seguridad basada en roles, persistencia de datos y algoritmos de optimización, garantizando el aislamiento de la información, la eficiencia computacional y la seguridad estricta en el manejo de memoria.

## Características Técnicas Principales
El sistema cumple con los requerimientos avanzados de ingeniería de software estructurada:

* **Control de Acceso Basado en Roles (RBAC):** Arquitectura de seguridad con 4 niveles de privilegios (Administrador, Gerente, Cajero y Usuario), gestionando operaciones de acceso y jornada bancaria según el perfil.
* **Persistencia de Datos Ininterrumpida:** Implementación de archivos binarios (`.bin` y `.dat`) para el registro, actualización y eliminación persistente de titulares y credenciales. Incluye un motor de auto-guardado transaccional en el ciclo principal para evitar pérdida de datos.
* **Manejo Dinámico de Memoria (Heap):** Uso estricto de punteros y paso por referencia para manipular listas de transacciones de tamaño variable (`malloc`, `realloc`, `free`), garantizando cero fugas de memoria.
* **Algoritmos y Optimización Matemática:**
  * **Búsqueda Binaria $O(\log n)$:** Utilizada para la localización eficiente de clientes mediante cédula en operaciones de registro y transferencias.
  * **Quicksort $O(n \log n)$:** Aplicado en el módulo de reportes para ordenar saldos de forma descendente y generar rankings gerenciales.
  * **Recursividad Aplicada:** Subprogramas puramente recursivos para la auditoría y cálculo de balance de la bóveda, y para la proyección de intereses compuestos en cuentas de ahorro.
* **Exportación de Reportes Estructurados:** Generación automática de comprobantes y estados de cuenta en texto plano (`.txt`), creación de logs de auditoría al cierre de caja (`.txt`), y exportación del balance general al formato separado por comas (`.csv`).
* **Experiencia de Usuario (UX) y Control de Estado:** Menús dinámicos con un mecanismo de escape universal ('X') estrictamente validado para abortar operaciones de forma segura, garantizando reversiones (rollbacks) en RAM sin corromper la persistencia.

## Estructura del Repositorio
* `/src`: Contiene el código fuente completamente modularizado (`.c` y `.h`).
  * `main.c`: Núcleo de la aplicación, enrutador de roles y ejecutor del auto-guardado.
  * `acceso.c / .h`: Motor de autenticación y máscara de contraseñas.
  * `algoritmos.c / .h`: Lógica de Búsqueda Binaria e inserción ordenada.
  * `cuentas.c / .h`: Gestión persistente de clientes (CRUD) y prevención de duplicados.
  * `menus.c / .h`: Desacoplamiento de interfaces de usuario y validación de abortos.
  * `persistencia.c / .h`: Serialización binaria de las estructuras y barrido seguro del Heap.
  * `reportes.c / .h`: Motor de algoritmos recursivos, ordenamiento Quicksort y exportación de archivos.
  * `transacciones.c / .h`: Lógica de negocio (depósitos, retiros y transferencias directas en memoria).
  * `validaciones.c / .h`: Control de entradas de cadena/monto y algoritmo de validación Módulo 10.
* `/.vscode`: Archivos de configuración del compilador y ejecución de tareas.

## Instrucciones de Compilación y Ejecución
Este proyecto utiliza el compilador GCC estándar. Para enlazar todos los módulos y compilar desde la terminal en la raíz del proyecto:

```bash
gcc -g src/*.c -o polibank.exe
./polibank.exe