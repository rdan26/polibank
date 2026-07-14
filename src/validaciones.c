#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> 
#include "validaciones.h"
#include "modelos.h"

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
            destino[longitud - 1] = '\0'; // ! Sustituye el salto de linea por el fin de cadena
        }
        else
        {
            limpiarBuffer(); // ! Drena la basura si el usuario escribio mas de la cuenta
        }
    }
}

// * Fuerza la entrada de un valor monetario de formato rigido (ej. 15.00)
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
            // ! Cuenta la longitud de la cadena que existe despues del punto
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

// * Retorna 1 si la cedula cumple el estandar del Registro Civil, 0 si es invalida
int validar_cedula_ecuatoriana(const char *cedula)
{
    // ! Debe tener exactamente 10 caracteres
    if (strlen(cedula) != 10) return 0;

    // ! Rechaza letras o simbolos
    for (int i = 0; i < 10; i++)
    {
        if (!isdigit(cedula[i])) return 0;
    }

    // ! Validacion de provincia (01 a 24, o 30 para ecuatorianos en el exterior)
    int provincia = (cedula[0] - '0') * 10 + (cedula[1] - '0');
    if (provincia < 1 || (provincia > 24 && provincia != 30)) return 0;

    // ! Validacion de persona natural (el tercer digito debe ser menor a 6)
    int tercer_digito = cedula[2] - '0';
    if (tercer_digito >= 6) return 0;

    // * Algoritmo Modulo 10 (Multiplicacion intercalada por 2 y 1)
    int suma = 0;
    for (int i = 0; i < 9; i++)
    {
        int digito = cedula[i] - '0';
        
        // ! Las posiciones impares en la vida real son los indices pares en C (0, 2, 4...)
        if (i % 2 == 0)
        { 
            digito *= 2;
            if (digito > 9) digito -= 9; // ! Si el doble es 10 o mas, se le resta 9
        }
        suma += digito;
    }

    // * Calcula la diferencia contra la decena superior inmediata
    int decena_superior = ((suma + 9) / 10) * 10;
    int digito_calculado = decena_superior - suma;
    
    if (digito_calculado == 10)
    {
        digito_calculado = 0;
    }

    int digito_real = cedula[9] - '0';

    // ! Retorna 1 (verdadero) si el calculo matematico coincide con el ultimo digito impreso
    return (digito_calculado == digito_real);
}

// * Bucle que exige un identificador regional real
void leer_cedula_valida(char *destino)
{
    while (1)
    {
        // ! MAX_CEDULA es 15 (definido en modelos.h) para evitar desbordamientos si el usuario tipea de más
        leerCadenaSegura(destino, MAX_CEDULA);
        
        if (validar_cedula_ecuatoriana(destino))
        {
            break; // ! La estructura de datos es valida, salimos del bucle
        }
        else
        {
            printf("Error: Cedula ecuatoriana invalida o falsa. Ingrese 10 digitos reales: ");
        }
    }
}