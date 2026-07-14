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

    printf("=> Transaccion %d exitosa. Nuevo saldo: $%.2f\n", cta->historial[indice].id_transaccion, cta->saldo_actual);
}