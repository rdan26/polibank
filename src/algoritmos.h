/* ====== algoritmos.h ====== */

#ifndef ALGORITMOS_H
#define ALGORITMOS_H

#include "modelos.h"

int buscar_cliente_por_cedula(const Cliente banco[], int total_clientes, const char *cedula);
int obtener_indice_insercion(const Cliente banco[], int total_clientes, const char *cedula);
void hacer_espacio_cliente(Cliente banco[], int *total_clientes, int indice_insercion);

#endif // ALGORITMOS_H