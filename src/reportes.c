/* ====== reportes.c ====== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "reportes.h"

float calcular_boveda_recursivo(const Cliente banco[], int total_clientes, int idx_cliente, int idx_cuenta)
{
    if (idx_cliente >= total_clientes) return 0.0f;
    
    if (idx_cuenta >= banco[idx_cliente].num_cuentas)
    {
        return calcular_boveda_recursivo(banco, total_clientes, idx_cliente + 1, 0);
    }
    
    return banco[idx_cliente].cuentas[idx_cuenta].saldo_actual + 
           calcular_boveda_recursivo(banco, total_clientes, idx_cliente, idx_cuenta + 1);
}

void generar_log_boveda(float total_boveda, int total_clientes)
{
    FILE *archivo = fopen("log_boveda.txt", "a");
    if (archivo != NULL)
    {
        time_t t = time(NULL);
        struct tm tm = *localtime(&t);
        
        fprintf(archivo, "[%02d/%02d/%d %02d:%02d:%02d] AUDITORIA BOVEDA | Clientes: %d | Liquidez Total: $%.2f\n",
                tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec,
                total_clientes, total_boveda);
        fclose(archivo);
        printf("=> Log de auditoria anexado exitosamente en 'log_boveda.txt'\n");
    }
    else
    {
        printf("Error al generar el log de boveda en archivo de texto.\n");
    }
}

float calcular_interes_recursivo(float capital, float tasa, int anios)
{
    if (anios == 0) return capital;
    return (1.0f + tasa) * calcular_interes_recursivo(capital, tasa, anios - 1);
}

void proyectar_intereses_cliente(const Cliente *cliente)
{
    printf("\n--- PROYECCION DE INTERESES (CUENTAS DE AHORRO) ---\n");
    int tiene_ahorros = 0;
    
    for (int i = 0; i < cliente->num_cuentas; i++)
    {
        if (strcmp(cliente->cuentas[i].tipo_cuenta, "Ahorros") == 0)
        {
            tiene_ahorros = 1;
            float saldo_actual = cliente->cuentas[i].saldo_actual;
            
            printf("Cuenta #%d | Saldo Base: $%.2f\n", cliente->cuentas[i].numero_cuenta, saldo_actual);
            
            // Mejora UX: Lenguaje profesional en lugar de "anos"
            printf("  -> Proyeccion a 1 periodo anual:    $%.2f\n", calcular_interes_recursivo(saldo_actual, 0.04f, 1));
            printf("  -> Proyeccion a 3 periodos anuales: $%.2f\n", calcular_interes_recursivo(saldo_actual, 0.04f, 3));
            printf("  -> Proyeccion a 5 periodos anuales: $%.2f\n", calcular_interes_recursivo(saldo_actual, 0.04f, 5));
        }
    }
    
    if (!tiene_ahorros)
    {
        printf("Aviso: El cliente no posee cuentas de Ahorros para proyectar intereses.\n");
    }
}

void exportar_balances_csv(const Cliente banco[], int total_clientes)
{
    FILE *archivo = fopen("balances_cuentas.csv", "w");
    if (archivo != NULL)
    {
        fprintf(archivo, "Cedula,Nombre Completo,Numero de Cuenta,Tipo,Saldo Actual\n");
        for (int i = 0; i < total_clientes; i++)
        {
            for (int j = 0; j < banco[i].num_cuentas; j++)
            {
                fprintf(archivo, "%s,%s,%d,%s,%.2f\n", 
                        banco[i].cedula, banco[i].nombre_completo, 
                        banco[i].cuentas[j].numero_cuenta, 
                        banco[i].cuentas[j].tipo_cuenta, 
                        banco[i].cuentas[j].saldo_actual);
            }
        }
        fclose(archivo);
        printf("\n=> Balances exportados exitosamente a 'balances_cuentas.csv'\n");
    }
    else
    {
        printf("\nError: No se pudo generar el archivo CSV.\n");
    }
}

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
    
    for (int i = 0; i < total_clientes; i++)
    {
        total_cuentas += banco[i].num_cuentas;
    }

    if (total_cuentas == 0) return;

    FilaRanking *arreglo = (FilaRanking *)malloc(total_cuentas * sizeof(FilaRanking));
    if (arreglo == NULL) return;

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

    quicksort(arreglo, 0, total_cuentas - 1);

    printf("\n--- RANKING DE CLIENTES (MAYOR SALDO) ---\n");
    for (int i = 0; i < total_cuentas; i++)
    {
        printf("%d. [%s] %s | Cta: #%d | Saldo: $%.2f\n", 
               i + 1, arreglo[i].cedula, arreglo[i].nombre, 
               arreglo[i].numero_cuenta, arreglo[i].saldo);
    }
    
    free(arreglo);
}