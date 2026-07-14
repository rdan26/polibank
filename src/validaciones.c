#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> 
#include "validaciones.h"
#include "modelos.h"

// * Vacía el buffer de entrada para evitar lecturas residuales en el sistema
void limpiarBuffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// * Extrae la cadena de la consola respetando estrictamente el límite de memoria
void leerCadenaSegura(char* destino, int longitud_max)
{
    if (fgets(destino, longitud_max, stdin) != NULL)
    {
        size_t longitud = strlen(destino);
        if (longitud > 0 && destino[longitud - 1] == '\n')
        {
            destino[longitud - 1] = '\0'; // ! Sustituye el salto de línea por el fin de cadena
        }
        else
        {
            limpiarBuffer(); // ! Drena la basura si el usuario escribió más de la cuenta
        }
    }
}

// * Fuerza la entrada de un valor monetario de formato flexible (ej. 12, 12.5, 12.50)
float leerMontoValido(void)
{
    char entrada[MAX_BUFFER_ENTRADA];
    float monto;
    char* fin_ptr;

    while (1)
    {
        leerCadenaSegura(entrada, MAX_BUFFER_ENTRADA);

        // ! Intercepción de signos negativos
        if (entrada[0] == '-')
        {
            printf("Error: No se aceptan transacciones con valores negativos. Intente de nuevo: $");
            continue;
        }

        // ! Evita espacios iniciales, símbolos extra o puntos huérfanos
        if (!isdigit(entrada[0]))
        {
            printf("Error: El monto debe iniciar con un digito valido (ej. 12, 12.5 o 12.50). Intente de nuevo: $");
            continue;
        }

        // ! Bloquea la conversión automática de notación científica (ej. "1e3")
        if (strchr(entrada, 'e') != NULL || strchr(entrada, 'E') != NULL)
        {
            printf("Error: Notacion cientifica no permitida. Ingrese moneda estandar: $");
            continue;
        }

        // * Convierte el texto a flotante en memoria
        monto = strtof(entrada, &fin_ptr);

        // ! Detecta caracteres alfabéticos mezclados después del número
        if (fin_ptr == entrada || (*fin_ptr != '\0' && *fin_ptr != '\n' && *fin_ptr != '\r'))
        {
            printf("Error: Entrada invalida. Por favor, ingrese un valor numerico valido: $");
            continue;
        }

        // ! Asegura que no se ingresen transacciones de valor cero o menores
        if (monto <= 0.0f)
        {
            printf("Error: El monto debe ser mayor que cero. Intente de nuevo: $");
            continue;
        }

        // ! Protección contra desbordamiento
        if (monto > 9999999.99f)
        {
            printf("Error: Limite estructural excedido. El sistema soporta un maximo de $9999999.99 por transaccion.\n");
            printf("Por favor, ingrese un monto menor: $");
            continue;
        }

        // ! Validación de formato monetario flexible (.x o .xx permitidos, o sin decimales)
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
                printf("Error: Formato decimal incorrecto. Use un entero o entre 1 y 2 decimales (ej. 15, 15.5, 15.50): $");
                continue;
            }
        }

        return monto;
    }
}

// * Retorna 1 si la cédula cumple el estándar del Registro Civil, 0 si es inválida
int validar_cedula_ecuatoriana(const char *cedula)
{
    // ! Debe tener exactamente 10 caracteres
    if (strlen(cedula) != 10) return 0;

    // ! Rechaza letras o símbolos
    for (int i = 0; i < 10; i++)
    {
        if (!isdigit(cedula[i])) return 0;
    }

    // ! Validación de provincia (01 a 24, o 30 para ecuatorianos en el exterior)
    int provincia = (cedula[0] - '0') * 10 + (cedula[1] - '0');
    if (provincia < 1 || (provincia > 24 && provincia != 30)) return 0;

    // ! Validación de persona natural (el tercer dígito debe ser menor a 6)
    int tercer_digito = cedula[2] - '0';
    if (tercer_digito >= 6) return 0;

    // * Algoritmo Módulo 10 (Multiplicación intercalada por 2 y 1)
    int suma = 0;
    for (int i = 0; i < 9; i++)
    {
        int digito = cedula[i] - '0';
        
        // ! Las posiciones impares en la vida real son los índices pares en C (0, 2, 4...)
        if (i % 2 == 0)
        { 
            digito *= 2;
            if (digito > 9) digito -= 9; // ! Si el doble es 10 o más, se le resta 9
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

    // ! Retorna 1 (verdadero) si el cálculo matemático coincide con el último dígito impreso
    return (digito_calculado == digito_real);
}

// * Bucle que exige un identificador regional real o permite cancelar la operación
int leer_cedula_valida(char *destino)
{
    while (1)
    {
        printf("Ingrese su cédula ecuatoriana (10 dígitos, o 'X' para cancelar): ");
        leerCadenaSegura(destino, MAX_CEDULA);
        
        // ! MECANISMO DE ESCAPE: Si el usuario teclea 'X' o 'x' y da Enter
        if ((destino[0] == 'X' || destino[0] == 'x') && destino[1] == '\0')
        {
            return 0; // * Retorna 0 (Falso/Cancelado) al main
        }

        if (validar_cedula_ecuatoriana(destino))
        {
            return 1; // ! La estructura de datos es válida, retorna 1 (Verdadero/Éxito)
        }
        else
        {
            printf("\n========================================\n");
            printf("[ERROR DE VALIDACION]\n");
            printf("Motivo: Identificacion invalida.\n");
            printf("Causal: Los digitos provistos no cumplen el algoritmo Modulo 10 del Registro Civil.\n");
            printf("Accion: Verifique que este ingresando su identificador real de 10 digitos.\n");
            printf("========================================\n");
        }
    }
}