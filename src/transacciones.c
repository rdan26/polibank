#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transacciones.h"

void procesar_deposito(Cliente *cliente, float monto)
{
    // * Creamos un puntero auxiliar para escribir menos y hacer el codigo mas legible
    Cuenta *cta = &cliente->cuenta_bancaria;

    // ! Verificacion de limite de memoria
    if (cta->num_transacciones >= cta->capacidad_historial)
    {
        // * Multiplicamos la capacidad por 2
        int nueva_capacidad = cta->capacidad_historial * 2; 
        
        // * Asignamos el nuevo bloque a un puntero temporal
        Transaccion *temp = (Transaccion *)realloc(cta->historial, nueva_capacidad * sizeof(Transaccion));
        
        // ! Verificacion de seguridad: Si realloc falla, retorna NULL
        if (temp == NULL)
        {
            printf("\nError CRITICO: Memoria insuficiente para procesar la transaccion.\n");
            return; // ! Abortamos la funcion, pero el historial original queda intacto
        }
        
        // ! Si fue exitoso, actualizamos los punteros del cliente
        cta->historial = temp;
        cta->capacidad_historial = nueva_capacidad;
        // * Imprimir en consola para ver cuando el sistema expande su RAM
        printf("\n[Sistema] Memoria del historial expandida a %d bloques.\n", nueva_capacidad);
    }

    // * Construccion del registro de la transaccion
    int indice = cta->num_transacciones; // * El indice actual es igual a la cantidad de elementos
    
    cta->historial[indice].id_transaccion = rand() % 9000 + 1000; // ! ID de 4 digitos
    strcpy(cta->historial[indice].tipo, "Deposito");
    cta->historial[indice].monto = monto;
    strcpy(cta->historial[indice].fecha, "13/07/2026"); // Fecha estática // TODO: conectar a time.h

    // * Actualizacion del estado matematico de la cuenta
    cta->saldo_actual += monto;
    cta->num_transacciones++;

    printf("=> Comprobante: Transaccion %d procesada exitosamente. Monto ingresado: $%.2f\n", 
        cta->historial[indice].id_transaccion, monto);
}

void procesar_transferencia(Cliente *origen, Cliente *destino, float monto)
{
    // * Alteracion matematica atonica de los saldos
    origen->cuenta_bancaria.saldo_actual -= monto;
    destino->cuenta_bancaria.saldo_actual += monto;
    
    // * Registro en el historial dinamico del ORIGEN (Salida)
    if (origen->cuenta_bancaria.num_transacciones >= origen->cuenta_bancaria.capacidad_historial)
    {
        origen->cuenta_bancaria.capacidad_historial *= 2;
        origen->cuenta_bancaria.historial = (Transaccion *)realloc(
            origen->cuenta_bancaria.historial, 
            origen->cuenta_bancaria.capacidad_historial * sizeof(Transaccion)
        );
        
        // ! Validacion critica de memoria RAM
        if (origen->cuenta_bancaria.historial == NULL)
        {
            printf("\nError CRITICO: Fuga de memoria (realloc) en el historial de origen.\n");
            exit(1); 
        }
    }
    
    int indice_hist_origen = origen->cuenta_bancaria.num_transacciones;
    strcpy(origen->cuenta_bancaria.historial[indice_hist_origen].tipo, "Transferencia Enviada");
    origen->cuenta_bancaria.historial[indice_hist_origen].monto = monto; 
    origen->cuenta_bancaria.num_transacciones++;
    
    // * Registro en el historial dinamico del DESTINO (Entrada)
    if (destino->cuenta_bancaria.num_transacciones >= destino->cuenta_bancaria.capacidad_historial)
    {
        destino->cuenta_bancaria.capacidad_historial *= 2;
        destino->cuenta_bancaria.historial = (Transaccion *)realloc(
            destino->cuenta_bancaria.historial, 
            destino->cuenta_bancaria.capacidad_historial * sizeof(Transaccion)
        );
        
        // ! Validacion critica de memoria RAM
        if (destino->cuenta_bancaria.historial == NULL)
        {
            printf("\nError CRITICO: Fuga de memoria (realloc) en el historial de destino.\n");
            exit(1); 
        }
    }
    
    int indice_hist_destino = destino->cuenta_bancaria.num_transacciones;
    strcpy(destino->cuenta_bancaria.historial[indice_hist_destino].tipo, "Transferencia Recibida");
    destino->cuenta_bancaria.historial[indice_hist_destino].monto = monto;
    destino->cuenta_bancaria.num_transacciones++;
    
    // * Generacion del Comprobante (Con ocultamiento de saldos por privacidad)
    printf("\n========================================\n");
    printf("      COMPROBANTE DE TRANSFERENCIA      \n");
    printf("========================================\n");
    printf("ESTADO: EXITOSA\n");
    printf("ORIGEN:  %s\n", origen->nombre_completo);
    printf("DESTINO: %s\n", destino->nombre_completo);
    printf("MONTO ENVIADO: $%.2f\n", monto);
    printf("========================================\n");
    printf("INFO: Los saldos han sido actualizados de forma segura.\n");
}

void procesar_retiro(Cliente *cliente, float monto)
{
    // * Alteracion matematica del saldo
    cliente->cuenta_bancaria.saldo_actual -= monto;
    
    // * Registro en el historial dinamico con expansion elastica
    if (cliente->cuenta_bancaria.num_transacciones >= cliente->cuenta_bancaria.capacidad_historial)
    {
        cliente->cuenta_bancaria.capacidad_historial *= 2;
        cliente->cuenta_bancaria.historial = (Transaccion *)realloc(
            cliente->cuenta_bancaria.historial, 
            cliente->cuenta_bancaria.capacidad_historial * sizeof(Transaccion)
        );
        
        if (cliente->cuenta_bancaria.historial == NULL)
        {
            printf("\nError CRITICO: Fuga de memoria (realloc) en el historial.\n");
            exit(1); 
        }
    }
    
    int indice = cliente->cuenta_bancaria.num_transacciones;
    strcpy(cliente->cuenta_bancaria.historial[indice].tipo, "Retiro");
    cliente->cuenta_bancaria.historial[indice].monto = monto;
    cliente->cuenta_bancaria.num_transacciones++;
    
    // * Comprobante
    printf("\n========================================\n");
    printf("        COMPROBANTE DE RETIRO           \n");
    printf("========================================\n");
    printf("ESTADO: EXITOSO\n");
    printf("CLIENTE: %s\n", cliente->nombre_completo);
    printf("MONTO RETIRADO: $%.2f\n", monto);
    printf("NUEVO SALDO:    $%.2f\n", cliente->cuenta_bancaria.saldo_actual);
    printf("========================================\n");
}

void imprimir_estado_cuenta(const Cliente *cliente)
{
    printf("\n========================================\n");
    printf("          ESTADO DE CUENTA              \n");
    printf("========================================\n");
    printf("Titular:  %s\n", cliente->nombre_completo);
    printf("Cedula:   %s\n", cliente->cedula);
    printf("Cuenta:   %d (%s)\n", cliente->cuenta_bancaria.numero_cuenta, cliente->cuenta_bancaria.tipo_cuenta);
    printf("Saldo:    $%.2f\n", cliente->cuenta_bancaria.saldo_actual);
    printf("----------------------------------------\n");
    printf("        HISTORIAL DE TRANSACCIONES      \n");
    printf("----------------------------------------\n");

    if (cliente->cuenta_bancaria.num_transacciones == 0)
    {
        printf("No hay transacciones registradas.\n");
    }
    else
    {
        // * Iteracion controlada sobre la memoria dinamica
        for (int i = 0; i < cliente->cuenta_bancaria.num_transacciones; i++)
        {
            printf("[%d] %-25s : $%.2f\n", 
                   i + 1, 
                   cliente->cuenta_bancaria.historial[i].tipo, 
                   cliente->cuenta_bancaria.historial[i].monto);
        }
    }
    printf("========================================\n");
}