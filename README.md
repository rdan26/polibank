# POLI BANK - Sistema de Cuentas y Transacciones Bancarias (Parte 2)

**Institución:** Escuela Politécnica Nacional (EPN)
**Asignatura:** Programación I
**Docente:** Ing. Thomás Borja, MSc.

## Descripción del Proyecto
POLI BANK es un sistema bancario digital desarrollado en C que simula la gestión transaccional y administrativa de una entidad financiera. Esta segunda etapa evoluciona la arquitectura inicial para incorporar seguridad basada en roles (RBAC), persistencia de datos y algoritmos de optimización, garantizando el aislamiento de la información y la eficiencia computacional.

## Características Técnicas Principales
El sistema cumple con los requerimientos avanzados de ingeniería de software estructurada:

* **Control de Acceso Basado en Roles (RBAC):** Arquitectura de seguridad con 4 niveles de privilegios (Administrador, Gerente, Cajero y Usuario), restringiendo las operaciones según el nivel de autorización.
* **Persistencia de Datos Segura:** Implementación de archivos binarios (`.bin` y `.dat`) para el almacenamiento permanente de clientes, cuentas y listas de historiales dinámicos.
* **Manejo Avanzado de Memoria (Heap):** Uso de punteros y funciones de asignación dinámica (`malloc`, `realloc`, `free`) para gestionar historiales de transacciones de tamaño variable sin fugas de memoria.
* **Algoritmos de Optimización:**
  * **Búsqueda Binaria $O(\log n)$:** Para la localización ultrarrápida de clientes mediante su número de cédula.
  * **Quicksort $O(n \log n)$:** Utilizado en el módulo de inteligencia de negocios para generar rankings precisos de saldos.
  * **Recursividad:** Algoritmos sin bucles iterativos para auditorías profundas de bóveda.
* **Exportación de Reportes:** Generación automática de comprobantes y estados de cuenta en formato de texto plano (`.txt`).
* **Experiencia de Usuario (UX):** Menús dinámicos que se adaptan al contexto del usuario y autocompletado de credenciales para garantizar el aislamiento de cuentas (un usuario no puede operar cuentas ajenas).

## Estructura del Repositorio
* `/src`: Contiene el código fuente modularizado (`.c` y `.h`).
  * `main.c`: Núcleo de la aplicación y enrutador de roles.
  * `acceso.c / .h`: Motor de autenticación y máscara de contraseñas.
  * `algoritmos.c / .h`: Lógica de ordenamiento y Búsqueda Binaria.
  * `cuentas.c / .h`: Registro y control de clientes.
  * `persistencia.c / .h`: Serialización binaria de las estructuras.
  * `reportes.c / .h`: Motor de recursividad y Quicksort.
  * `transacciones.c / .h`: Lógica de negocio (depósitos, retiros, transferencias).
  * `validaciones.c / .h`: Control de entradas y algoritmo Módulo 10.
* `/.vscode`: Archivos de configuración del compilador.

## Instrucciones de Compilación y Ejecución
Este proyecto utiliza el compilador GCC estándar. Para compilar desde la terminal en la raíz del proyecto:

```bash
gcc -g src/*.c -o polibank.exe
./polibank.exe