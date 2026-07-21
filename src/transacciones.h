/* ====== transacciones.h ====== */

#ifndef TRANSACCIONES_H
#define TRANSACCIONES_H

#include "modelos.h"

void procesar_deposito(Cliente *cliente, int numero_cuenta, float monto);
void procesar_transferencia(Cliente *origen, int num_cuenta_origen, Cliente *destino, int num_cuenta_destino, float monto);
void procesar_retiro(Cliente *cliente, int numero_cuenta, float monto);
void imprimir_estado_cuenta(const Cliente *cliente);

#endif // TRANSACCIONES_H