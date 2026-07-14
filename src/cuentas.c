#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cuentas.h"
#include "validaciones.h"

void registrar_cliente(Cliente *nuevo_cliente, const char *cedula_validada)
{
    // * Asignacion de identidad
    strcpy(nuevo_cliente->cedula, cedula_validada);

    // * Instruccion inicial clara y precisa
    printf("Ingrese el nombre completo (nombre/s apellido/s): ");
    
    // ! Exige al menos dos palabras separadas por espacio
    while (1)
    {
        leerCadenaSegura(nuevo_cliente->nombre_completo, MAX_NOMBRE);

        int contador_palabras = 0;
        int dentro_de_palabra = 0;

        // * Escanea el arreglo caracter por caracter
        for (int i = 0; nuevo_cliente->nombre_completo[i] != '\0'; i++)
        {
            if (nuevo_cliente->nombre_completo[i] != ' ')
            {
                if (!dentro_de_palabra)
                {
                    contador_palabras++;
                    dentro_de_palabra = 1; 
                }
            }
            else
            {
                dentro_de_palabra = 0; 
            }
        }

        // ! Condicion de salida: 2 palabras (ej. "Daniel Perez") son suficientes
        if (contador_palabras >= 2)
        {
            break; 
        }
        else
        {
            printf("Error: Registro incompleto. Debe ingresar al menos un nombre y un apellido.\n");
            printf("Intente de nuevo (nombre/s apellido/s): ");
        }
    }

    // * Inicializacion matematica y comercial de la cuenta
    nuevo_cliente->cuenta_bancaria.numero_cuenta = rand() % 900000 + 100000; // Genera un ID de 6 digitos
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

    printf("\n=> Memoria inicializada. Cliente '%s' registrado (Cuenta: %d).\n", 
           nuevo_cliente->nombre_completo, 
           nuevo_cliente->cuenta_bancaria.numero_cuenta);
}

int buscar_cliente_por_cedula(Cliente banco[], int total_clientes, const char *cedula)
{
    // * Escanea la memoria activa desde el primer cliente hasta el total registrado
    for (int i = 0; i < total_clientes; i++)
    {
        // * strcmp devuelve 0 si ambas cadenas de texto son exactas
        if (strcmp(banco[i].cedula, cedula) == 0)
        {
            return i; // * Retorna la posicion exacta (indice) del cliente en el arreglo
        }
    }
    
    return -1; // ! -1 actua como bandera de que la cedula no fue encontrada
}