#ifndef TRANSACCIONES_H
#define TRANSACCIONES_H

#include "modelos.h"

// * Inyecta capital a la cuenta especificada si pertenece al cliente
void procesar_deposito(Cliente *cliente, int numero_cuenta, float monto);

// * Realiza transferencias simultaneas verificando pertenencia de cuentas
void procesar_transferencia(Cliente *origen, int num_cuenta_origen, Cliente *destino, int num_cuenta_destino, float monto);

// * Procesa un retiro verificando fondos y pertenencia
void procesar_retiro(Cliente *cliente, int numero_cuenta, float monto);

// * Imprime el estado total de la cuenta en consola y ofrece exportacion a TXT
void imprimir_estado_cuenta(const Cliente *cliente);

#endif // TRANSACCIONES_H