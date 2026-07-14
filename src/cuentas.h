#ifndef CUENTAS_H
#define CUENTAS_H

#include "modelos.h"

// * Define la cantidad de espacio inicial para el historial
#define CAPACIDAD_INICIAL_HISTORIAL 10

// * Construye o anade una cuenta a la instancia del cliente y asigna memoria dinamica
// * Tambien solicita la creacion de credenciales si es un cliente nuevo.
void registrar_cliente(Cliente *cliente, const char *cedula_validada);

#endif // CUENTAS_H