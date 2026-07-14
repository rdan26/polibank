#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "modelos.h"

// * Motor de Busqueda Binaria (O(log n)). El arreglo debe estar ordenado por cedula.
// * Retorna el indice del cliente si existe, o -1 si no fue encontrado.
int buscar_cliente_por_cedula(const Cliente banco[], int total_clientes, const char *cedula);

// * Encuentra el indice exacto donde debe insertarse un nuevo registro para mantener el orden.
int obtener_indice_insercion(const Cliente banco[], int total_clientes, const char *cedula);

// * Desplaza el arreglo a la derecha para hacer espacio seguro sin corromper la memoria Heap.
void hacer_espacio_cliente(Cliente banco[], int *total_clientes, int indice_insercion);

#endif // ALGORITMOS_H