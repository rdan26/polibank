#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> 
#include "validaciones.h"

// * Vacia el buffer de entrada para evitar lecturas residuales en el sistema
void limpiarBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// * Extrae la cadena de la consola respetando estrictamente el limite de memoria
void leerCadenaSegura(char* destino, int longitud_max)
{
    if (fgets(destino, longitud_max, stdin) != NULL)
    {
        size_t longitud = strlen(destino);
        if (longitud > 0 && destino[longitud - 1] == '\n')
        {
            destino[longitud - 1] = '\0'; // * Sustituye el salto de linea por el fin de cadena
        }
        else
        {
            limpiarBuffer(); // * Drena la basura si el usuario escribio mas de la cuenta
        }
    }
}

// * Fuerza la entrada de un valor monetario matematico de formato rigido (ej. 15.00)
float leerMontoValido(void)
{
    char entrada[MAX_BUFFER_ENTRADA];
    float monto;
    char* fin_ptr;

    while (1)
    {
        leerCadenaSegura(entrada, MAX_BUFFER_ENTRADA);

        // ! Evita espacios iniciales, signos o puntos huerfanos (ej. " .45")
        if (!isdigit(entrada[0]))
        {
            printf("Error: El monto debe iniciar con un numero (ej. 0.45). Intente de nuevo: ");
            continue;
        }

        // ! Bloquea la conversion automatica de notacion cientifica (ej. "1e3")
        if (strchr(entrada, 'e') != NULL || strchr(entrada, 'E') != NULL)
        {
            printf("Error: Notacion cientifica no permitida. Ingrese moneda estandar: ");
            continue;
        }

        // * Convierte el texto puro a flotante en memoria
        monto = strtof(entrada, &fin_ptr);

        // ! Detecta caracteres alfabeticos mezclados despues del numero (ej. "150a")
        if (fin_ptr == entrada || *fin_ptr != '\0')
        {
            printf("Error: Entrada invalida. Por favor, ingrese un valor numerico: ");
            continue;
        }

        // ! Asegura transacciones positivas (ni cero ni negativas)
        if (monto <= 0)
        {
            printf("Error: El monto debe ser mayor que cero. Intente de nuevo: ");
            continue;
        }

        // ! Validacion de formato monetario estricto (x.xx)
        char* punto = strchr(entrada, '.'); 
        
        if (punto == NULL)
        {
            // ! Rechaza numeros enteros puros obligando el uso de ".00"
            printf("Error: Debe incluir los centavos. Ingrese exactamente dos decimales (ej. %.2f): ", monto);
            continue;
        }
        else
        {
            // * Cuenta la longitud de la cadena que existe despues del punto
            int decimales = strlen(punto + 1); 
            
            if (decimales != 2)
            {
                // ! Rechaza montos con 1 o más de 2 decimales (ej. "10.9" o "10.999")
                printf("Error: Formato estricto. Ingrese exactamente dos decimales (ej. 10.90): ");
                continue;
            }
        }

        return monto;
    }
}