#ifndef TRANSACCIONES_H
#define TRANSACCIONES_H

#include "modelos.h"

// * Inyecta capital a la cuenta y registra el movimiento en la memoria dinamica
void procesar_deposito(Cliente *cliente, float monto);

// * Inyeccion simultanea en dos cuentas distintas
void procesar_transferencia(Cliente *origen, Cliente *destino, float monto);

// * Proceso de retiro
void procesar_retiro(Cliente *cliente, float monto);

// * Lectura de estado (El modificador const blinda la memoria contra alteraciones)
void imprimir_estado_cuenta(const Cliente *cliente);

#endif // TRANSACCIONES_H