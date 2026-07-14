#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cuentas.h"
#include "validaciones.h"

void registrar_cliente(Cliente *nuevo_cliente, const char *cedula_validada)
{
    // * Asignacion de identidad
    strcpy(nuevo_cliente->cedula, cedula_validada);

    printf("Ingrese el nombre completo del cliente: ");

    leerCadenaSegura(nuevo_cliente->nombre_completo, MAX_NOMBRE);

    // * Inicializacion del estado de la cuenta
    nuevo_cliente->cuenta_bancaria.numero_cuenta = rand() % 900000 + 100000;
    strcpy(nuevo_cliente->cuenta_bancaria.tipo_cuenta, "Ahorros");
    nuevo_cliente->cuenta_bancaria.saldo_actual = 0.0f;
    nuevo_cliente->cuenta_bancaria.num_transacciones = 0;
    nuevo_cliente->cuenta_bancaria.capacidad_historial = CAPACIDAD_INICIAL_HISTORIAL;

    // * Asignacion dinamica de memoria en el Heap
    nuevo_cliente->cuenta_bancaria.historial = (Transaccion *)malloc(CAPACIDAD_INICIAL_HISTORIAL * sizeof(Transaccion));

    // ! Verificacion memoria
    if (nuevo_cliente->cuenta_bancaria.historial == NULL)
    {
        printf("\nError CRITICO: Fallo al asignar memoria dinamica (malloc).\n");
        exit(1); 
    }

    printf("\n=> Memoria de transacciones inicializada. Cliente '%s' registrado con exito.\n", nuevo_cliente->nombre_completo);
}