#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "reportes.h"

// * Recursividad profunda para recorrer clientes y cuentas sin usar bucles for/while
float calcular_boveda_recursivo(const Cliente banco[], int total_clientes, int idx_cliente, int idx_cuenta)
{
    // Caso base 1: Hemos procesado todos los clientes
    if (idx_cliente >= total_clientes)
    {
        return 0.0f;
    }
    
    // Caso base 2: Hemos procesado todas las cuentas del cliente actual, pasamos al siguiente
    if (idx_cuenta >= banco[idx_cliente].num_cuentas)
    {
        return calcular_boveda_recursivo(banco, total_clientes, idx_cliente + 1, 0);
    }
    
    // Suma recursiva: Saldo actual + el resto del arbol de cuentas
    return banco[idx_cliente].cuentas[idx_cuenta].saldo_actual + 
           calcular_boveda_recursivo(banco, total_clientes, idx_cliente, idx_cuenta + 1);
}

// * Estructura temporal plana para facilitar el ordenamiento Quicksort
typedef struct {
    char cedula[MAX_CEDULA];
    char nombre[MAX_NOMBRE];
    int numero_cuenta;
    float saldo;
} FilaRanking;

void intercambiar(FilaRanking *a, FilaRanking *b)
{
    FilaRanking temp = *a;
    *a = *b;
    *b = temp;
}

int particion(FilaRanking arr[], int bajo, int alto)
{
    float pivote = arr[alto].saldo;
    int i = (bajo - 1);
    
    for (int j = bajo; j <= alto - 1; j++)
    {
        // ! Orden descendente (Mayor a Menor)
        if (arr[j].saldo > pivote)
        {
            i++;
            intercambiar(&arr[i], &arr[j]);
        }
    }
    intercambiar(&arr[i + 1], &arr[alto]);
    return (i + 1);
}

void quicksort(FilaRanking arr[], int bajo, int alto)
{
    if (bajo < alto)
    {
        int indice_particion = particion(arr, bajo, alto);
        quicksort(arr, bajo, indice_particion - 1);
        quicksort(arr, indice_particion + 1, alto);
    }
}

void generar_ranking_saldos(const Cliente banco[], int total_clientes)
{
    int total_cuentas = 0;
    
    // * 1. Conteo total de elementos a ordenar
    for (int i = 0; i < total_clientes; i++)
    {
        total_cuentas += banco[i].num_cuentas;
    }

    if (total_cuentas == 0)
    {
        printf("\nError: No existen cuentas activas para generar un ranking.\n");
        return;
    }

    // * 2. Asignacion dinamica para aplanar la estructura bidimensional
    FilaRanking *arreglo = (FilaRanking *)malloc(total_cuentas * sizeof(FilaRanking));
    if (arreglo == NULL)
    {
        printf("\nError critico: Memoria insuficiente para procesar el Quicksort.\n");
        return;
    }

    int indice = 0;
    for (int i = 0; i < total_clientes; i++)
    {
        for (int j = 0; j < banco[i].num_cuentas; j++)
        {
            strcpy(arreglo[indice].cedula, banco[i].cedula);
            strcpy(arreglo[indice].nombre, banco[i].nombre_completo);
            arreglo[indice].numero_cuenta = banco[i].cuentas[j].numero_cuenta;
            arreglo[indice].saldo = banco[i].cuentas[j].saldo_actual;
            indice++;
        }
    }

    // * 3. Ejecucion del Algoritmo Quicksort O(n log n)
    quicksort(arreglo, 0, total_cuentas - 1);

    // * 4. Exportacion y despliegue
    FILE *archivo = fopen("ranking_saldos.txt", "w");
    
    printf("\n======================================================\n");
    printf("              RANKING DE CLIENTES (MAYOR SALDO)         \n");
    printf("======================================================\n");
    
    if (archivo != NULL)
    {
        fprintf(archivo, "======================================================\n");
        fprintf(archivo, "              RANKING DE CLIENTES (MAYOR SALDO)         \n");
        fprintf(archivo, "======================================================\n");
    }

    for (int i = 0; i < total_cuentas; i++)
    {
        printf("%d. [%s] %s | Cta: #%d | Saldo: $%.2f\n", 
               i + 1, arreglo[i].cedula, arreglo[i].nombre, 
               arreglo[i].numero_cuenta, arreglo[i].saldo);
               
        if (archivo != NULL)
        {
            fprintf(archivo, "%d. [%s] %s | Cta: #%d | Saldo: $%.2f\n", 
                   i + 1, arreglo[i].cedula, arreglo[i].nombre, 
                   arreglo[i].numero_cuenta, arreglo[i].saldo);
        }
    }
    printf("======================================================\n");
    
    if (archivo != NULL)
    {
        fprintf(archivo, "======================================================\n");
        fclose(archivo);
        printf("=> Ranking exportado exitosamente a 'ranking_saldos.txt'\n");
    }

    free(arreglo);
}