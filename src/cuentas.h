#ifndef CUENTAS_H
#define CUENTAS_H

#include "modelos.h"

#define CAPACIDAD_INICIAL_HISTORIAL 10

int registrar_cliente(Cliente *cliente, const char *cedula_validada);
void actualizar_datos_cliente(Cliente *cliente);
void cerrar_cuenta(Cliente banco[], int *total_clientes, int indice_cliente, int numero_cuenta);

#endif // CUENTAS_H