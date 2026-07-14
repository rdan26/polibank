#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
        printf("\n========================================\n");
        printf("[ERROR DE TRANSACCION]\n");
        printf("Motivo: No se puede procesar el deposito.\n");
        printf("Causal: El numero de cuenta %d no pertenece al titular con cedula %s.\n", numero_cuenta, cliente->cedula);
        printf("Accion: Verifique el identificador de cuenta e intente de nuevo.\n");
        printf("========================================\n");
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
    strcpy(cta->historial[indice].fecha, "13/07/2026"); 

    cta->saldo_actual += monto;
    cta->num_transacciones++;

    printf("\n========================================\n");
    printf("[DEPOSITO PROCESADO]\n");
    printf("Comprobante ID:     #%d\n", cta->historial[indice].id_transaccion);
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
        printf("\n========================================\n");
        printf("[ERROR DE TRANSACCION]\n");
        printf("Motivo: Verificacion de cuentas fallida.\n");
        printf("Accion: Valide los parametros de transferencia.\n");
        printf("========================================\n");
        return;
    }

    if (monto > cta_origen->saldo_actual)
    {
        printf("\n========================================\n");
        printf("[ERROR DE TRANSACCION]\n");
        printf("Motivo: Fondos insuficientes.\n");
        printf("Causal: El monto solicitado ($%.2f) excede el saldo ($%.2f).\n", monto, cta_origen->saldo_actual);
        printf("========================================\n");
        return;
    }

    cta_origen->saldo_actual -= monto;
    cta_destino->saldo_actual += monto;
    
    // Historial origen
    if (cta_origen->num_transacciones >= cta_origen->capacidad_historial)
    {
        cta_origen->capacidad_historial *= 2;
        cta_origen->historial = (Transaccion *)realloc(cta_origen->historial, cta_origen->capacidad_historial * sizeof(Transaccion));
    }
    
    int indice_origen = cta_origen->num_transacciones;
    cta_origen->historial[indice_origen].id_transaccion = rand() % 9000 + 1000;
    strcpy(cta_origen->historial[indice_origen].tipo, "Transferencia Enviada");
    cta_origen->historial[indice_origen].monto = monto; 
    strcpy(cta_origen->historial[indice_origen].fecha, "13/07/2026");
    cta_origen->num_transacciones++;
    
    // Historial destino
    if (cta_destino->num_transacciones >= cta_destino->capacidad_historial)
    {
        cta_destino->capacidad_historial *= 2;
        cta_destino->historial = (Transaccion *)realloc(cta_destino->historial, cta_destino->capacidad_historial * sizeof(Transaccion));
    }
    
    int indice_destino = cta_destino->num_transacciones;
    cta_destino->historial[indice_destino].id_transaccion = rand() % 9000 + 1000;
    strcpy(cta_destino->historial[indice_destino].tipo, "Transferencia Recibida");
    cta_destino->historial[indice_destino].monto = monto;
    strcpy(cta_destino->historial[indice_destino].fecha, "13/07/2026");
    cta_destino->num_transacciones++;
    
    printf("\n========================================\n");
    printf("      COMPROBANTE DE TRANSFERENCIA      \n");
    printf("========================================\n");
    printf("ESTADO:         EXITOSA\n");
    printf("TITULAR ORIGEN: %s\n", origen->nombre_completo);
    printf("DESTINATARIO:   %s\n", destino->nombre_completo);
    printf("MONTO ENVIADO:  $%.2f\n", monto);
    printf("========================================\n");
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
    
    cta->saldo_actual -= monto;
    
    if (cta->num_transacciones >= cta->capacidad_historial)
    {
        cta->capacidad_historial *= 2;
        cta->historial = (Transaccion *)realloc(cta->historial, cta->capacidad_historial * sizeof(Transaccion));
    }
    
    int indice = cta->num_transacciones;
    cta->historial[indice].id_transaccion = rand() % 9000 + 1000;
    strcpy(cta->historial[indice].tipo, "Retiro");
    cta->historial[indice].monto = monto;
    strcpy(cta->historial[indice].fecha, "13/07/2026");
    cta->num_transacciones++;
    
    printf("\nRetiro exitoso de $%.2f. Nuevo saldo: $%.2f\n", monto, cta->saldo_actual);
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
        printf("        HISTORIAL DE TRANSACCIONES      \n");
        printf("----------------------------------------\n");

        if (cta->num_transacciones == 0)
        {
            printf("No hay transacciones registradas.\n");
        }
        else
        {
            for (int i = 0; i < cta->num_transacciones; i++)
            {
                printf("[%d] %-25s : $%.2f\n", 
                       i + 1, 
                       cta->historial[i].tipo, 
                       cta->historial[i].monto);
            }
        }
    }
    printf("========================================\n");

    // * Logica de Exportacion de Archivos corregida para codificacion limpia
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
                        fprintf(archivo, "[%d] %-25s : $%.2f\n", 
                               i + 1, 
                               cta->historial[i].tipo, 
                               cta->historial[i].monto);
                    }
                }
            }
            fprintf(archivo, "========================================\n");
            fclose(archivo);
            printf("=> Exportacion exitosa. Archivo guardado como: %s\n", nombre_archivo);
        }
        else
        {
            printf("Error: No se pudo generar el archivo de texto.\n");
        }
    }
}