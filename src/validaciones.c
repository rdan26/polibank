#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h> 
#include <stdbool.h>
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

// * Validacion de cedula ecuatoriana
int validarCedula(const char *cedula) {

    // Verificar que la cédula tenga exactamente 10 dígitos
    if (strlen(cedula) != 10) {
        printf("Cédula inválida: Debe tener exactamente 10 dígitos.\n");
        return 0; 
    }

    // Verificar que todos los caracteres sean dígitos
    for (int i = 0; i < 10; i++) {
        if (!isdigit(cedula[i])) {
            printf ("Cédula inválida: Ingrese solo dígitos.\n");
            return 0;
        }
    }

    // Validar el dígito verificador
    int suma = 0;
    for (int i = 0; i < 9; i++) {
        int digito = cedula[i] - '0';
        if (i % 2 == 0) { // Posiciones pares (0, 2, 4, 6, 8)
            digito *= 2;
            if (digito > 9) {
                digito -= 9;
            }
        }
        suma += digito;
    }

    int digitoVerificadorCalculado = (10 - (suma % 10)) % 10;
    int digitoVerificadorCedula = cedula[9] - '0';

    if (digitoVerificadorCalculado == digitoVerificadorCedula) {
        printf("Cédula válida.\n");
        return 1;
    } else {
        printf("Cédula inválida: El dígito verificador no coincide.\n");
        return 0;
    }
}

// * Validacion de fecha ingresada por el usuario
int validacionFecha (int dia, int mes, int year)
{
    // Validar el año
    if (year < 1900 || year > 2025)
    {
        printf("Error: Año inválido. Ingrese un año perteneciente al intervalo de 1900 a 2025.\n");
        return 0;
    }

    // Validar el mes
    if (mes < 1 || mes > 12)
    {
        printf("Error: Mes inválido. Ingrese su número perteneciente al intervalo de 1 a 12.\n");
        return 0;
    }

    int diasEnMes[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Variable para validar si es el año bisiesto o no
    bool esBisiesto = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

    if (esBisiesto) diasEnMes[1] = 29;

    if (dia < 1 || dia > diasEnMes[mes - 1])
    {
        printf("Error: Día inválido para el mes ingresado. Ingrese un día entre 1 y %d.\n", diasEnMes[mes - 1]);
        return 0;
    }
    return true;
}

//* Validacion de cuenta de usuario y contraseña
int validarCuenta(const char* usuario, const char* password) {

    // Validar usuario y contraseña no vacíos
    if (usuario == NULL || strlen(usuario) == 0) 
    {
        printf("Error: El usuario no puede estar vacío.\n");
        return 0;
    }

    if (password == NULL || strlen(password) == 0) 
    {
        printf("Error: La contraseña no puede estar vacía.\n");
        return 0;
    }

    // TODO: Verificar el usuario y la contraseña contra una base de datos binaria o un archivo de texto seguro.
}
