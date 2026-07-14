#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "modelos.h"

// * Guarda de forma persistente y binaria la base de clientes y sus historiales dinamicos
void guardar_sistema(const Cliente banco[], int total_clientes);

// * Carga y reconstruye en memoria Heap la base de datos de clientes e historiales
int cargar_sistema(Cliente banco[]);

#endif // PERSISTENCIA_H