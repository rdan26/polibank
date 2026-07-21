/* ====== persistencia.h ====== */

#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "modelos.h"

void guardar_sistema(const Cliente banco[], int total_clientes);
int cargar_sistema(Cliente banco[]);
void liberar_memoria_sistema(Cliente banco[], int total_clientes); // Nueva funcion extraida

#endif // PERSISTENCIA_H