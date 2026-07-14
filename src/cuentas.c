#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cuentas.h"
#include "validaciones.h"
#include "acceso.h"

void registrar_cliente(Cliente *cliente, const char *cedula_validada)
{
    // * 1. Inicializacion de datos personales si es el primer registro del cliente
    if (cliente->num_cuentas == 0)
    {
        strcpy(cliente->cedula, cedula_validada);

        printf("Ingrese el nombre completo (nombre/s apellido/s): ");
        
        // ! BARRERA ESTRUCTURAL Y DE CARACTERES
        while (1)
        {
            leerCadenaSegura(cliente->nombre_completo, MAX_NOMBRE);

            // * Validacion de pureza de caracteres (Whitelist)
            int caracteres_validos = 1;
            for (int i = 0; cliente->nombre_completo[i] != '\0'; i++)
            {
                char c = cliente->nombre_completo[i];
                
                // ! Si NO es letra, NO es espacio, NO es guion y NO es apostrofe, es invalido.
                if (!isalpha(c) && c != ' ' && c != '-' && c != '\'')
                {
                    caracteres_validos = 0;
                    break;
                }
            }

            if (!caracteres_validos)
            {
                printf("\n========================================\n");
                printf("[ERROR DE VALIDACION]\n");
                printf("Motivo: El nombre contiene caracteres invalidos.\n");
                printf("Causal: Se detectaron numeros, simbolos o caracteres especiales.\n");
                printf("Accion: Use unicamente letras, espacios, guiones (-) y apostrofes (').\n");
                printf("========================================\n");
                printf("Intente de nuevo: ");
                continue; 
            }

            // * Validacion de estructura (Al menos dos palabras)
            int contador_palabras = 0;
            int dentro_de_palabra = 0;

            for (int i = 0; cliente->nombre_completo[i] != '\0'; i++)
            {
                if (cliente->nombre_completo[i] != ' ')
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
                printf("\n========================================\n");
                printf("[ERROR DE VALIDACION]\n");
                printf("Motivo: Estructura nominal incompleta.\n");
                printf("Causal: No se proporcionaron nombres y apellidos completos.\n");
                printf("Accion: Debe ingresar al menos un nombre y un apellido.\n");
                printf("========================================\n");
                printf("Intente de nuevo: ");
            }
        }

        // ! NORMALIZACION DE DATOS: Formato Title Case
        int capitalizar_siguiente = 1; 
        
        for (int i = 0; cliente->nombre_completo[i] != '\0'; i++)
        {
            char c = cliente->nombre_completo[i];
            
            if (c == ' ' || c == '-' || c == '\'')
            {
                capitalizar_siguiente = 1; 
            }
            else if (capitalizar_siguiente && isalpha(c))
            {
                cliente->nombre_completo[i] = toupper(c);
                capitalizar_siguiente = 0; 
            }
            else if (isalpha(c))
            {
                cliente->nombre_completo[i] = tolower(c);
            }
        }

        // ! REGISTRO DE CREDENCIALES
        char nueva_password[50];
        printf("Cree una contrasena para el acceso digital del cliente: ");
        leerCadenaSegura(nueva_password, 50);
        
        // * Guardado automatico persistente (RBAC) con username igual a cedula y rol USUARIO
        registrar_nuevo_usuario(cliente->cedula, nueva_password);
        printf("=> Credenciales creadas exitosamente. Usuario de acceso: %s\n", cliente->cedula);
    }

    // * 2. Validacion de Limites de Cuenta
    if (cliente->num_cuentas >= MAX_CUENTAS_POR_CLIENTE)
    {
        printf("\n========================================\n");
        printf("[ERROR DE REGISTRO]\n");
        printf("Motivo: Limite de cuentas excedido.\n");
        printf("Causal: El titular de cedula %s ya posee %d cuentas (limite maximo).\n", cliente->cedula, MAX_CUENTAS_POR_CLIENTE);
        printf("Accion: Cierre o reestructure cuentas previas antes de intentar de nuevo.\n");
        printf("========================================\n");
        return;
    }

    // * 3. Inicializacion matematica y comercial de la nueva cuenta
    int indice_nueva_cuenta = cliente->num_cuentas;
    Cuenta *nueva_cuenta = &cliente->cuentas[indice_nueva_cuenta];

    nueva_cuenta->numero_cuenta = rand() % 900000 + 100000;
    
    printf("\nSeleccione el tipo de cuenta:\n");
    printf("1. Ahorros\n");
    printf("2. Corriente\n");
    printf("Ingrese una opcion: ");
    char op_tipo[10];
    while (1)
    {
        leerCadenaSegura(op_tipo, 10);
        int op_num = atoi(op_tipo);
        if (op_num == 1)
        {
            strcpy(nueva_cuenta->tipo_cuenta, "Ahorros");
            break;
        }
        else if (op_num == 2)
        {
            strcpy(nueva_cuenta->tipo_cuenta, "Corriente");
            break;
        }
        else
        {
            printf("Error: Seleccion invalida. Ingrese 1 para Ahorros o 2 para Corriente: ");
        }
    }

    nueva_cuenta->saldo_actual = 0.0f;
    nueva_cuenta->num_transacciones = 0;
    nueva_cuenta->capacidad_historial = CAPACIDAD_INICIAL_HISTORIAL;

    // * Asignacion dinamica de memoria en el Heap
    nueva_cuenta->historial = (Transaccion *)malloc(CAPACIDAD_INICIAL_HISTORIAL * sizeof(Transaccion));

    if (nueva_cuenta->historial == NULL)
    {
        printf("\nError CRITICO: Fallo al asignar memoria dinamica (malloc).\n");
        exit(1); 
    }

    cliente->num_cuentas++;

    printf("\n========================================\n");
    printf("[REGISTRO EXITOSO]\n");
    printf("Cliente:            %s\n", cliente->nombre_completo);
    printf("Cedula:             %s\n", cliente->cedula);
    printf("Cuenta Aperturada:  #%d (%s)\n", nueva_cuenta->numero_cuenta, nueva_cuenta->tipo_cuenta);
    printf("Cuentas Totales:    %d / %d\n", cliente->num_cuentas, MAX_CUENTAS_POR_CLIENTE);
    printf("========================================\n");
}