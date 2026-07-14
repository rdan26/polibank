#ifndef CUENTAS_H
#define CUENTAS_H

#include "modelos.h"

// * Define la cantidad de espacio inicial para el historial
#define CAPACIDAD_INICIAL_HISTORIAL 10

// * Construye la instancia del cliente y asigna memoria dinamica a su cuenta
void registrar_cliente(Cliente *nuevo_cliente, const char *cedula_validada);

// * Escanea el arreglo de clientes. Retorna el indice si lo encuentra, o -1 si no existe.
int buscar_cliente_por_cedula(Cliente banco[], int total_clientes, const char *cedula);

#endif // CUENTAS_H