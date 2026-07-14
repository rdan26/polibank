#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transacciones.h"

void procesar_deposito(Cliente *cliente, int numero_cuenta, float monto)
{
    Cuenta *cta = NULL;
    
    // * Validación de pertenencia del ID de cuenta
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

    // ! Verificación de límite de memoria para el historial
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
        printf("\n[Sistema] Memoria del historial expandida a %d bloques.\n", nueva_capacidad);
    }

    int indice = cta->num_transacciones; 
    
    cta->historial[indice].id_transaccion = rand() % 9000 + 1000; 
    strcpy(cta->historial[indice].tipo, "Deposito");
    cta->historial[indice].monto = monto;
    strcpy(cta->historial[indice].fecha, "13/07/2026"); 

    // * Actualización del estado de la cuenta
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

    // * Validación de pertenencia de cuenta origen
    for (int i = 0; i < origen->num_cuentas; i++)
    {
        if (origen->cuentas[i].numero_cuenta == num_cuenta_origen)
        {
            cta_origen = &origen->cuentas[i];
            break;
        }
    }

    // * Validación de pertenencia de cuenta destino
    for (int i = 0; i < destino->num_cuentas; i++)
    {
        if (destino->cuentas[i].numero_cuenta == num_cuenta_destino)
        {
            cta_destino = &destino->cuentas[i];
            break;
        }
    }

    if (cta_origen == NULL)
    {
        printf("\n========================================\n");
        printf("[ERROR DE TRANSACCION]\n");
        printf("Motivo: Cuenta origen no valida.\n");
        printf("Causal: El numero de cuenta %d no pertenece al cliente de origen %s.\n", num_cuenta_origen, origen->nombre_completo);
        printf("Accion: Valide los parametros de transferencia.\n");
        printf("========================================\n");
        return;
    }

    if (cta_destino == NULL)
    {
        printf("\n========================================\n");
        printf("[ERROR DE TRANSACCION]\n");
        printf("Motivo: Cuenta destino no valida.\n");
        printf("Causal: El numero de cuenta %d no pertenece al destinatario %s.\n", num_cuenta_destino, destino->nombre_completo);
        printf("Accion: Verifique los datos del destinatario.\n");
        printf("========================================\n");
        return;
    }

    if (monto > cta_origen->saldo_actual)
    {
        printf("\n========================================\n");
        printf("[ERROR DE TRANSACCION]\n");
        printf("Motivo: Fondos insuficientes.\n");
        printf("Causal: El monto solicitado ($%.2f) excede el saldo de la cuenta origen ($%.2f).\n", monto, cta_origen->saldo_actual);
        printf("Accion: Reintente con un valor cubierto por sus fondos disponibles.\n");
        printf("========================================\n");
        return;
    }

    // * Alteración de saldos
    cta_origen->saldo_actual -= monto;
    cta_destino->saldo_actual += monto;
    
    // * Historial ORIGEN
    if (cta_origen->num_transacciones >= cta_origen->capacidad_historial)
    {
        cta_origen->capacidad_historial *= 2;
        Transaccion *temp = (Transaccion *)realloc(cta_origen->historial, cta_origen->capacidad_historial * sizeof(Transaccion));
        
        if (temp == NULL)
        {
            printf("\nError CRITICO: Fuga de memoria (realloc) en el historial de origen.\n");
            exit(1); 
        }
        cta_origen->historial = temp;
    }
    
    int indice_origen = cta_origen->num_transacciones;
    cta_origen->historial[indice_origen].id_transaccion = rand() % 9000 + 1000;
    strcpy(cta_origen->historial[indice_origen].tipo, "Transferencia Enviada");
    cta_origen->historial[indice_origen].monto = monto; 
    strcpy(cta_origen->historial[indice_origen].fecha, "13/07/2026");
    cta_origen->num_transacciones++;
    
    // * Historial DESTINO
    if (cta_destino->num_transacciones >= cta_destino->capacidad_historial)
    {
        cta_destino->capacidad_historial *= 2;
        Transaccion *temp = (Transaccion *)realloc(cta_destino->historial, cta_destino->capacidad_historial * sizeof(Transaccion));
        
        if (temp == NULL)
        {
            printf("\nError CRITICO: Fuga de memoria (realloc) en el historial de destino.\n");
            exit(1); 
        }
        cta_destino->historial = temp;
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
    printf("CUENTA ORIGEN:  #%d\n", cta_origen->numero_cuenta);
    printf("DESTINATARIO:   %s\n", destino->nombre_completo);
    printf("CUENTA DESTINO: #%d\n", cta_destino->numero_cuenta);
    printf("MONTO ENVIADO:  $%.2f\n", monto);
    printf("========================================\n");
}

void procesar_retiro(Cliente *cliente, int numero_cuenta, float monto)
{
    Cuenta *cta = NULL;

    // * Validación de pertenencia del ID de cuenta
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
        printf("Motivo: Cuenta no vinculada.\n");
        printf("Causal: El numero de cuenta %d no pertenece al cliente %s.\n", numero_cuenta, cliente->nombre_completo);
        printf("Accion: Verifique sus credenciales bancarias.\n");
        printf("========================================\n");
        return;
    }

    if (monto > cta->saldo_actual)
    {
        printf("\n========================================\n");
        printf("[ERROR DE TRANSACCION]\n");
        printf("Motivo: Fondos insuficientes.\n");
        printf("Causal: El monto solicitado ($%.2f) excede su saldo actual ($%.2f).\n", monto, cta->saldo_actual);
        printf("Accion: Intente con una cifra menor.\n");
        printf("========================================\n");
        return;
    }
    
    cta->saldo_actual -= monto;
    
    if (cta->num_transacciones >= cta->capacidad_historial)
    {
        cta->capacidad_historial *= 2;
        Transaccion *temp = (Transaccion *)realloc(cta->historial, cta->capacidad_historial * sizeof(Transaccion));
        
        if (temp == NULL)
        {
            printf("\nError CRITICO: Fuga de memoria (realloc) en el historial de retiros.\n");
            exit(1); 
        }
        cta->historial = temp;
    }
    
    int indice = cta->num_transacciones;
    cta->historial[indice].id_transaccion = rand() % 9000 + 1000;
    strcpy(cta->historial[indice].tipo, "Retiro");
    cta->historial[indice].monto = monto;
    strcpy(cta->historial[indice].fecha, "13/07/2026");
    cta->num_transacciones++;
    
    printf("\n========================================\n");
    printf("        COMPROBANTE DE RETIRO           \n");
    printf("========================================\n");
    printf("ESTADO:         EXITOSO\n");
    printf("TITULAR:        %s\n", cliente->nombre_completo);
    printf("CUENTA:         #%d\n", cta->numero_cuenta);
    printf("MONTO RETIRADO: $%.2f\n", monto);
    printf("SALDO RESTANTE: $%.2f\n", cta->saldo_actual);
    printf("========================================\n");
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
}