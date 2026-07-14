#ifndef REPORTES_H
#define REPORTES_H

#include "modelos.h"

// * Calcula los fondos totales del banco usando recursividad estricta (sin bucles)
float calcular_boveda_recursivo(const Cliente banco[], int total_clientes, int idx_cliente, int idx_cuenta);

// * Genera un ranking ordenado de mayor a menor saldo usando Quicksort y lo exporta
void generar_ranking_saldos(const Cliente banco[], int total_clientes);

#endif // REPORTES_H