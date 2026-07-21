/* ====== algoritmos.c ====== */

#include <string.h>
#include "algoritmos.h"
#include "modelos.h"

int buscar_cliente_por_cedula(const Cliente banco[], int total_clientes, const char *cedula)
{
    int inicio = 0;
    int fin = total_clientes - 1;

    // Busqueda Binaria estricta
    while (inicio <= fin)
    {
        int medio = inicio + (fin - inicio) / 2;
        int comparacion = strcmp(banco[medio].cedula, cedula);

        if (comparacion == 0)
        {
            return medio; 
        }
        else if (comparacion < 0)
        {
            inicio = medio + 1; 
        }
        else
        {
            fin = medio - 1; 
        }
    }
    
    return -1; 
}

int obtener_indice_insercion(const Cliente banco[], int total_clientes, const char *cedula)
{
    int inicio = 0;
    int fin = total_clientes - 1;
    
    // Busqueda binaria para encontrar la posicion de insercion alfabetica (numerica)
    while (inicio <= fin)
    {
        int medio = inicio + (fin - inicio) / 2;
        if (strcmp(banco[medio].cedula, cedula) < 0)
        {
            inicio = medio + 1;
        }
        else
        {
            fin = medio - 1;
        }
    }
    
    return inicio; 
}

void hacer_espacio_cliente(Cliente banco[], int *total_clientes, int indice_insercion)
{
    // Desplazamiento a la derecha
    for (int i = *total_clientes; i > indice_insercion; i--)
    {
        banco[i] = banco[i - 1]; 
    }
    
    memset(&banco[indice_insercion], 0, sizeof(Cliente));
    (*total_clientes)++;
}