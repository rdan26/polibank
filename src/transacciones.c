#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "transacciones.h"
#include "validaciones.h"

void procesar_deposito(Cliente *cliente, int numero_cuenta, float monto)
{
    Cuenta *cta = NULL;
    
    for (int i = 0; i < cliente->num_cuentas; i++)
    {
        if (cliente->cuentas[i].numero_cuenta == numero_cuenta)
        {
            cta = &cliente->cuentas[i];
            break;
        }
    }

    if (cta == NULL)
    {
        printf("\nError: No se puede procesar el deposito. Cuenta no encontrada.\n");
        return;
    }

    if (cta->num_transacciones >= cta->capacidad_historial)
    {
        int nueva_capacidad = cta->capacidad_historial * 2; 
        Transaccion *temp = (Transaccion *)realloc(cta->historial, nueva_capacidad * sizeof(Transaccion));
        
        if (temp == NULL)
        {
            printf("\nError CRITICO: Memoria insuficiente para procesar la transaccion.\n");
            return; 
        }
        
        cta->historial = temp;
        cta->capacidad_historial = nueva_capacidad;
    }

    int indice = cta->num_transacciones; 
    
    cta->historial[indice].id_transaccion = rand() % 9000 + 1000; 
    strcpy(cta->historial[indice].tipo, "Deposito");
    cta->historial[indice].monto = monto;
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(cta->historial[indice].fecha, 11, "%d/%m/%Y", &tm);

    cta->saldo_actual += monto;
    cta->num_transacciones++;

    printf("\n========================================\n");
    printf("[DEPOSITO PROCESADO]\n");
    printf("Comprobante ID:     #%d\n", cta->historial[indice].id_transaccion);
    printf("Fecha:              %s\n", cta->historial[indice].fecha);
    printf("Cliente:            %s\n", cliente->nombre_completo);
    printf("Cuenta Afectada:    #%d\n", cta->numero_cuenta);
    printf("Monto Ingresado:    $%.2f\n", monto);
    printf("Saldo Actualizado:  $%.2f\n", cta->saldo_actual);
    printf("========================================\n");
}

void procesar_transferencia(Cliente *origen, int num_cuenta_origen, Cliente *destino, int num_cuenta_destino, float monto)
{
    Cuenta *cta_origen = NULL;
    Cuenta *cta_destino = NULL;

    for (int i = 0; i < origen->num_cuentas; i++)
    {
        if (origen->cuentas[i].numero_cuenta == num_cuenta_origen)
        {
            cta_origen = &origen->cuentas[i];
            break;
        }
    }

    for (int i = 0; i < destino->num_cuentas; i++)
    {
        if (destino->cuentas[i].numero_cuenta == num_cuenta_destino)
        {
            cta_destino = &destino->cuentas[i];
            break;
        }
    }

    if (cta_origen == NULL || cta_destino == NULL)
    {
        printf("\nError: Verificacion de cuentas fallida.\n");
        return;
    }

    if (monto > cta_origen->saldo_actual)
    {
        printf("\nError: Fondos insuficientes para la transferencia.\n");
        return;
    }

    if (cta_origen->num_transacciones >= cta_origen->capacidad_historial)
    {
        int nueva_cap_o = cta_origen->capacidad_historial * 2;
        Transaccion *temp_o = (Transaccion *)realloc(cta_origen->historial, nueva_cap_o * sizeof(Transaccion));
        if (temp_o == NULL)
        {
            printf("\nError CRITICO: Memoria insuficiente para registrar en cuenta origen.\n");
            return;
        }
        cta_origen->historial = temp_o;
        cta_origen->capacidad_historial = nueva_cap_o;
    }

    if (cta_destino->num_transacciones >= cta_destino->capacidad_historial)
    {
        int nueva_cap_d = cta_destino->capacidad_historial * 2;
        Transaccion *temp_d = (Transaccion *)realloc(cta_destino->historial, nueva_cap_d * sizeof(Transaccion));
        if (temp_d == NULL)
        {
            printf("\nError CRITICO: Memoria insuficiente para registrar en cuenta destino.\n");
            return;
        }
        cta_destino->historial = temp_d;
        cta_destino->capacidad_historial = nueva_cap_d;
    }

    cta_origen->saldo_actual -= monto;
    cta_destino->saldo_actual += monto;
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    
    int indice_origen = cta_origen->num_transacciones;
    cta_origen->historial[indice_origen].id_transaccion = rand() % 9000 + 1000;
    strcpy(cta_origen->historial[indice_origen].tipo, "Transferencia Enviada");
    cta_origen->historial[indice_origen].monto = monto; 
    strftime(cta_origen->historial[indice_origen].fecha, 11, "%d/%m/%Y", &tm);
    cta_origen->num_transacciones++;
    
    int indice_destino = cta_destino->num_transacciones;
    cta_destino->historial[indice_destino].id_transaccion = rand() % 9000 + 1000;
    strcpy(cta_destino->historial[indice_destino].tipo, "Transferencia Recibida");
    cta_destino->historial[indice_destino].monto = monto;
    strftime(cta_destino->historial[indice_destino].fecha, 11, "%d/%m/%Y", &tm);
    cta_destino->num_transacciones++;
    
    printf("\n=> Transferencia exitosa de $%.2f hacia %s el %s.\n", monto, destino->nombre_completo, cta_origen->historial[indice_origen].fecha);
}

void procesar_retiro(Cliente *cliente, int numero_cuenta, float monto)
{
    Cuenta *cta = NULL;
    for (int i = 0; i < cliente->num_cuentas; i++)
    {
        if (cliente->cuentas[i].numero_cuenta == numero_cuenta)
        {
            cta = &cliente->cuentas[i];
            break;
        }
    }

    if (cta == NULL || monto > cta->saldo_actual)
    {
        printf("\nError: Cuenta no valida o fondos insuficientes.\n");
        return;
    }
    
    if (cta->num_transacciones >= cta->capacidad_historial)
    {
        int nueva_capacidad = cta->capacidad_historial * 2;
        Transaccion *temp = (Transaccion *)realloc(cta->historial, nueva_capacidad * sizeof(Transaccion));
        if (temp == NULL)
        {
            printf("\nError CRITICO: Memoria insuficiente para procesar el retiro.\n");
            return;
        }
        cta->historial = temp;
        cta->capacidad_historial = nueva_capacidad;
    }
    
    cta->saldo_actual -= monto;
    
    int indice = cta->num_transacciones;
    cta->historial[indice].id_transaccion = rand() % 9000 + 1000;
    strcpy(cta->historial[indice].tipo, "Retiro");
    cta->historial[indice].monto = monto;
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    strftime(cta->historial[indice].fecha, 11, "%d/%m/%Y", &tm);
    
    cta->num_transacciones++;
    
    printf("\nRetiro exitoso de $%.2f el %s. Nuevo saldo: $%.2f\n", monto, cta->historial[indice].fecha, cta->saldo_actual);
}

void imprimir_estado_cuenta(const Cliente *cliente)
{
    printf("\n========================================\n");
    printf("          ESTADO DE CUENTA              \n");
    printf("========================================\n");
    printf("Titular:  %s\n", cliente->nombre_completo);
    printf("Cedula:   %s\n", cliente->cedula);
    printf("Cuentas Activas: %d\n", cliente->num_cuentas);
    
    for (int j = 0; j < cliente->num_cuentas; j++)
    {
        const Cuenta *cta = &cliente->cuentas[j];
        printf("\n----------------------------------------\n");
        printf("Cuenta:   #%d (%s)\n", cta->numero_cuenta, cta->tipo_cuenta);
        printf("Saldo:    $%.2f\n", cta->saldo_actual);
        printf("----------------------------------------\n");

        if (cta->num_transacciones == 0)
        {
            printf("No hay transacciones registradas.\n");
        }
        else
        {
            for (int i = 0; i < cta->num_transacciones; i++)
            {
                printf("[%s] %-22s : $%.2f\n", cta->historial[i].fecha, cta->historial[i].tipo, cta->historial[i].monto);
            }
        }
    }
    printf("========================================\n");

    printf("\nDesea exportar este recibo a un archivo de texto? (S/N): ");
    char conf[10];
    leerCadenaSegura(conf, 10);
    
    if (conf[0] == 'S' || conf[0] == 's')
    {
        char nombre_archivo[50];
        sprintf(nombre_archivo, "estado_cuenta_%s.txt", cliente->cedula);
        
        FILE *archivo = fopen(nombre_archivo, "w");
        if (archivo != NULL)
        {
            fprintf(archivo, "========================================\n");
            fprintf(archivo, "          ESTADO DE CUENTA              \n");
            fprintf(archivo, "========================================\n");
            fprintf(archivo, "Titular:  %s\n", cliente->nombre_completo);
            fprintf(archivo, "Cedula:   %s\n", cliente->cedula);
            
            for (int j = 0; j < cliente->num_cuentas; j++)
            {
                const Cuenta *cta = &cliente->cuentas[j];
                fprintf(archivo, "\n----------------------------------------\n");
                fprintf(archivo, "Cuenta:   #%d (%s)\n", cta->numero_cuenta, cta->tipo_cuenta);
                fprintf(archivo, "Saldo:    $%.2f\n", cta->saldo_actual);
                fprintf(archivo, "----------------------------------------\n");
                
                if (cta->num_transacciones == 0)
                {
                    fprintf(archivo, "No hay transacciones registradas.\n");
                }
                else
                {
                    for (int i = 0; i < cta->num_transacciones; i++)
                    {
                        fprintf(archivo, "[%s] %-22s : $%.2f\n", cta->historial[i].fecha, cta->historial[i].tipo, cta->historial[i].monto);
                    }
                }
            }
            fprintf(archivo, "========================================\n");
            fclose(archivo);
            printf("=> Exportacion exitosa. Archivo guardado como: %s\n", nombre_archivo);
        }
    }
}