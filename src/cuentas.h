#ifndef CUENTAS_H
#define CUENTAS_H

#include "modelos.h"

// * Define la cantidad de espacio inicial para no sobrecargar la RAM en el testing
#define CAPACIDAD_INICIAL_HISTORIAL 10

// * Construye la instancia del cliente y asigna memoria dinamica a su cuenta
void registrar_cliente(Cliente *nuevo_cliente, const char *cedula_validada);

#endif // CUENTAS_H