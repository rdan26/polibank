#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> 
#include "validaciones.h"
#include "modelos.h"

void limpiarBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void leerCadenaSegura(char* destino, int longitud_max)
{
    if (fgets(destino, longitud_max, stdin) != NULL)
    {
        size_t longitud = strlen(destino);
        if (longitud > 0 && destino[longitud - 1] == '\n')
        {
            destino[longitud - 1] = '\0'; 
        }
        else
        {
            limpiarBuffer(); 
        }
    }
}

float leerMontoValido(void)
{
    char entrada[MAX_BUFFER_ENTRADA];
    float monto;
    char* fin_ptr;

    while (1)
    {
        leerCadenaSegura(entrada, MAX_BUFFER_ENTRADA);

        if (entrada[0] == '-')
        {
            printf("Error: No se aceptan transacciones con valores negativos. Intente de nuevo: $");
            continue;
        }

        if (!isdigit(entrada[0]))
        {
            printf("Error: El monto debe iniciar con un digito valido (ej. 12, 12.5 o 12.50). Intente de nuevo: $");
            continue;
        }

        if (strchr(entrada, 'e') != NULL || strchr(entrada, 'E') != NULL)
        {
            printf("Error: Notacion cientifica no permitida. Ingrese moneda estandar: $");
            continue;
        }

        monto = strtof(entrada, &fin_ptr);

        if (fin_ptr == entrada || (*fin_ptr != '\0' && *fin_ptr != '\n' && *fin_ptr != '\r'))
        {
            printf("Error: Entrada invalida. Por favor, ingrese un valor numerico valido: $");
            continue;
        }

        if (monto <= 0.0f)
        {
            printf("Error: El monto debe ser mayor que cero. Intente de nuevo: $");
            continue;
        }

        if (monto > 9999999.99f)
        {
            printf("Error: Limite estructural excedido. Maximo permitido $9999999.99.\n");
            printf("Por favor, ingrese un monto menor: $");
            continue;
        }

        char* punto = strchr(entrada, '.'); 
        if (punto != NULL)
        {
            int decimales = 0;
            int caracter_invalido_decimal = 0;
            for (int i = 1; punto[i] != '\0' && punto[i] != '\n' && punto[i] != '\r'; i++)
            {
                if (!isdigit(punto[i]))
                {
                    caracter_invalido_decimal = 1;
                    break;
                }
                decimales++;
            }
            
            if (caracter_invalido_decimal || decimales < 1 || decimales > 2)
            {
                printf("Error: Formato decimal incorrecto. Use un entero o hasta 2 decimales (ej. 15, 15.50): $");
                continue;
            }
        }

        return monto;
    }
}

int validar_cedula_ecuatoriana(const char *cedula)
{
    if (strlen(cedula) != 10) return 0;

    for (int i = 0; i < 10; i++)
    {
        if (!isdigit(cedula[i])) return 0;
    }

    int provincia = (cedula[0] - '0') * 10 + (cedula[1] - '0');
    if (provincia < 1 || (provincia > 24 && provincia != 30)) return 0;

    int tercer_digito = cedula[2] - '0';
    if (tercer_digito >= 6) return 0;

    int suma = 0;
    for (int i = 0; i < 9; i++)
    {
        int digito = cedula[i] - '0';
        
        if (i % 2 == 0)
        { 
            digito *= 2;
            if (digito > 9) digito -= 9; 
        }
        suma += digito;
    }

    int decena_superior = ((suma + 9) / 10) * 10;
    int digito_calculado = decena_superior - suma;
    
    if (digito_calculado == 10)
    {
        digito_calculado = 0;
    }

    int digito_real = cedula[9] - '0';

    return (digito_calculado == digito_real);
}

int leer_cedula_valida(char *destino)
{
    while (1)
    {
        printf("Ingrese su cedula ecuatoriana (10 digitos, o 'X' para cancelar): ");
        leerCadenaSegura(destino, MAX_CEDULA);
        
        if ((destino[0] == 'X' || destino[0] == 'x') && destino[1] == '\0')
        {
            return 0; 
        }

        if (validar_cedula_ecuatoriana(destino))
        {
            return 1; 
        }
        else
        {
            printf("\n========================================\n");
            printf("[ERROR DE VALIDACION]\n");
            printf("Motivo: Identificacion invalida.\n");
            printf("Causal: Los digitos no cumplen el algoritmo Modulo 10.\n");
            printf("Accion: Verifique su identificador.\n");
            printf("========================================\n");
        }
    }
}