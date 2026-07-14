#ifndef TRANSACCIONES_H
#define TRANSACCIONES_H

#include "modelos.h"

// * Inyecta capital a la cuenta y registra el movimiento en la memoria dinamica
void procesar_deposito(Cliente *cliente, float monto);

#endif // TRANSACCIONES_H