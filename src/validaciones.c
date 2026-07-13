#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "validaciones.h"

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
        monto = strtof(entrada, &fin_ptr);
        if (fin_ptr == entrada || *fin_ptr != '\0')
        {
            printf("Error: Entrada inválida. Por favor, ingrese un valor numérico: ");
        }
        else if (monto <= 0)
        {
            printf("Error: El monto debe ser mayor que cero. Intente de nuevo: ");
        }
        else
        {
            return monto;
        }
    }
}