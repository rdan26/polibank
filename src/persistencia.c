#include <stdio.h>
#include <stdlib.h>
#include "modelos.h"

// * Guardar una cuenta en el archivo binario
void guardar_cuenta_binario(Cuenta *c, FILE *archivo) {
    // * Guardar los datos estáticos de la cuenta (tamaño fijo)
    // * Usamos variables temporales para no escribir el puntero en disco
    int numero = c->numero_cuenta;
    float saldo = c->saldo_actual;
    int num_trans = c->num_transacciones;
    
    fwrite(&numero, sizeof(int), 1, archivo);
    fwrite(c->tipo_cuenta, sizeof(char), MAX_TIPO_CUENTA, archivo);
    fwrite(&saldo, sizeof(float), 1, archivo);
    fwrite(&num_trans, sizeof(int), 1, archivo); // ! Cuántas transacciones hay

    // * Guardar el bloque de memoria dinámica (El arreglo de transacciones)
    if (num_trans > 0 && c->historial != NULL) {
        fwrite(c->historial, sizeof(Transaccion), num_trans, archivo);
    }
}