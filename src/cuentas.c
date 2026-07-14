#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cuentas.h"
#include "validaciones.h"

void registrar_cliente(Cliente *nuevo_cliente, const char *cedula_validada)
{
    // * Asignacion de identidad
    strcpy(nuevo_cliente->cedula, cedula_validada);

    printf("Ingrese el nombre completo (nombre/s apellido/s): ");
    
    // ! BARRERA ESTRUCTURAL Y DE CARACTERES
    while (1)
    {
        leerCadenaSegura(nuevo_cliente->nombre_completo, MAX_NOMBRE);

        // * 1. Validacion de pureza de caracteres (Whitelist)
        int caracteres_validos = 1;
        for (int i = 0; nuevo_cliente->nombre_completo[i] != '\0'; i++)
        {
            char c = nuevo_cliente->nombre_completo[i];
            
            // ! Si NO es letra, NO es espacio, NO es guion y NO es apostrofe, es invalido.
            if (!isalpha(c) && c != ' ' && c != '-' && c != '\'')
            {
                caracteres_validos = 0;
                break;
            }
        }

        if (!caracteres_validos)
        {
            printf("Error: El nombre contiene numeros o simbolos no permitidos.\n");
            printf("Solo se permiten letras, espacios, guiones (-) y apostrofes (').\n");
            printf("Intente de nuevo: ");
            continue; // ! Interrumpe el ciclo actual y vuelve a pedir la cadena
        }

        // * 2. Validacion de estructura (Al menos dos palabras)
        int contador_palabras = 0;
        int dentro_de_palabra = 0;

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

        if (contador_palabras >= 2)
        {
            break; // * La cadena sobrevivio a todas las validaciones
        }
        else
        {
            printf("Error: Registro incompleto. Debe ingresar al menos un nombre y un apellido.\n");
            printf("Intente de nuevo: ");
        }
    }

    // ! NORMALIZACION DE DATOS: Formato Title Case (Sensible a guiones y apostrofes)
    int capitalizar_siguiente = 1; 
    
    for (int i = 0; nuevo_cliente->nombre_completo[i] != '\0'; i++)
    {
        char c = nuevo_cliente->nombre_completo[i];
        
        // * El siguiente caracter util se capitaliza si venimos de un espacio, guion o apostrofe
        if (c == ' ' || c == '-' || c == '\'')
        {
            capitalizar_siguiente = 1; 
        }
        else if (capitalizar_siguiente && isalpha(c))
        {
            nuevo_cliente->nombre_completo[i] = toupper(c);
            capitalizar_siguiente = 0; 
        }
        else if (isalpha(c))
        {
            nuevo_cliente->nombre_completo[i] = tolower(c);
        }
    }

    // * Inicializacion matematica y comercial de la cuenta
    nuevo_cliente->cuenta_bancaria.numero_cuenta = rand() % 900000 + 100000;
    strcpy(nuevo_cliente->cuenta_bancaria.tipo_cuenta, "Ahorros");
    nuevo_cliente->cuenta_bancaria.saldo_actual = 0.0f;
    nuevo_cliente->cuenta_bancaria.num_transacciones = 0;
    nuevo_cliente->cuenta_bancaria.capacidad_historial = CAPACIDAD_INICIAL_HISTORIAL;

    // * Asignacion dinamica de memoria en el Heap
    nuevo_cliente->cuenta_bancaria.historial = (Transaccion *)malloc(CAPACIDAD_INICIAL_HISTORIAL * sizeof(Transaccion));

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