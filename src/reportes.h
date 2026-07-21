/* ====== reportes.h ====== */

#ifndef REPORTES_H
#define REPORTES_H

#include "modelos.h"

float calcular_boveda_recursivo(const Cliente banco[], int total_clientes, int idx_cliente, int idx_cuenta);
void generar_ranking_saldos(const Cliente banco[], int total_clientes);
void exportar_balances_csv(const Cliente banco[], int total_clientes); // Nueva exportacion CSV
void generar_log_boveda(float total_boveda, int total_clientes); // Nuevo Log TXT
float calcular_interes_recursivo(float capital, float tasa, int anios); // Nuevo Interes Recursivo
void proyectar_intereses_cliente(const Cliente *cliente); // Menu de proyeccion

#endif // REPORTES_H