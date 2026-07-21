/* ====== persistencia.c ====== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "persistencia.h"
#include "cuentas.h"

#define ARCHIVO_DATOS "banco_datos.bin"

void guardar_sistema(const Cliente banco[], int total_clientes)
{
    FILE *archivo = fopen(ARCHIVO_DATOS, "wb");
    if (archivo == NULL)
    {
        printf("\nError de persistencia: No se pudo abrir el archivo para guardar datos.\n");
        return;
    }

    fwrite(&total_clientes, sizeof(int), 1, archivo);

    for (int i = 0; i < total_clientes; i++)
    {
        const Cliente *cli = &banco[i];
        
        fwrite(cli->cedula, sizeof(char), MAX_CEDULA, archivo);
        fwrite(cli->nombre_completo, sizeof(char), MAX_NOMBRE, archivo);
        fwrite(&cli->num_cuentas, sizeof(int), 1, archivo);

        for (int j = 0; j < cli->num_cuentas; j++)
        {
            const Cuenta *cta = &cli->cuentas[j];
            
            fwrite(&cta->numero_cuenta, sizeof(int), 1, archivo);
            fwrite(cta->tipo_cuenta, sizeof(char), MAX_TIPO_CUENTA, archivo);
            fwrite(&cta->saldo_actual, sizeof(float), 1, archivo);
            fwrite(&cta->num_transacciones, sizeof(int), 1, archivo);
            fwrite(&cta->capacidad_historial, sizeof(int), 1, archivo);

            if (cta->num_transacciones > 0 && cta->historial != NULL)
            {
                fwrite(cta->historial, sizeof(Transaccion), cta->num_transacciones, archivo);
            }
        }
    }

    fclose(archivo);
}

int cargar_sistema(Cliente banco[])
{
    FILE *archivo = fopen(ARCHIVO_DATOS, "rb");
    if (archivo == NULL) return 0;

    int total_clientes = 0;
    if (fread(&total_clientes, sizeof(int), 1, archivo) != 1)
    {
        fclose(archivo);
        return 0;
    }

    for (int i = 0; i < total_clientes; i++)
    {
        Cliente *cli = &banco[i];

        fread(cli->cedula, sizeof(char), MAX_CEDULA, archivo);
        fread(cli->nombre_completo, sizeof(char), MAX_NOMBRE, archivo);
        fread(&cli->num_cuentas, sizeof(int), 1, archivo);

        for (int j = 0; j < cli->num_cuentas; j++)
        {
            Cuenta *cta = &cli->cuentas[j];

            fread(&cta->numero_cuenta, sizeof(int), 1, archivo);
            fread(cta->tipo_cuenta, sizeof(char), MAX_TIPO_CUENTA, archivo);
            fread(&cta->saldo_actual, sizeof(float), 1, archivo);
            fread(&cta->num_transacciones, sizeof(int), 1, archivo);
            fread(&cta->capacidad_historial, sizeof(int), 1, archivo);

            if (cta->capacidad_historial < 10) cta->capacidad_historial = 10;
            if (cta->capacidad_historial < cta->num_transacciones) cta->capacidad_historial = cta->num_transacciones;

            cta->historial = (Transaccion *)malloc(cta->capacidad_historial * sizeof(Transaccion));
            if (cta->historial == NULL)
            {
                printf("\nError critico de memoria al cargar base de datos (malloc).\n");
                exit(1);
            }

            if (cta->num_transacciones > 0)
            {
                fread(cta->historial, sizeof(Transaccion), cta->num_transacciones, archivo);
            }
        }
    }

    fclose(archivo);
    return total_clientes;
}

void liberar_memoria_sistema(Cliente banco[], int total_clientes)
{
    // Barrido completo de la estructura dinamica para evitar fugas (Memory Leaks)
    for (int i = 0; i < total_clientes; i++) {
        for (int j = 0; j < banco[i].num_cuentas; j++) {
            if (banco[i].cuentas[j].historial != NULL) {
                free(banco[i].cuentas[j].historial);
                banco[i].cuentas[j].historial = NULL; 
            }
        }
    }
}