#ifndef CUENTAS_H
#define CUENTAS_H

#include "modelos.h"

// * Define la cantidad de espacio inicial para el historial
#define CAPACIDAD_INICIAL_HISTORIAL 10

// * Construye o añade una cuenta a la instancia del cliente y asigna memoria dinámica
void registrar_cliente(Cliente *cliente, const char *cedula_validada);

// * Escanea el arreglo de clientes. Retorna el índice si lo encuentra, o -1 si no existe.
int buscar_cliente_por_cedula(Cliente banco[], int total_clientes, const char *cedula);

#endif // CUENTAS_H