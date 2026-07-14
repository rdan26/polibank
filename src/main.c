#include <stdio.h>
#include <stdlib.h>
#include "modelos.h"
#include "validaciones.h"

int main() 
{
    int opcion;
    float monto_prueba;
    Cliente nuevo_cliente; // * Memoria temporal para probar el modulo de registro

    do {
        printf("\n========================================\n");
        printf("          POLI BANK - SISTEMA BASE        \n");
        printf("========================================\n");
        printf("1. Gestion de Cuentas (Registro)\n");
        printf("2. Transacciones (Depositos/Retiros)\n");
        printf("3. Transferencias\n");
        printf("4. Reportes y Ranking\n");
        printf("5. Boveda (Cierre de Jornada)\n");
        printf("6. Salir\n");
        printf("========================================\n");
        printf("Ingrese una opcion: ");
        
        scanf("%d", &opcion);
        // ! Drena el buffer despues del scanf para no corromper los fgets internos
        limpiarBuffer(); 

        switch(opcion) {
            case 1:
                printf("\n[ Modulo de Registro en construccion ]\n");
                printf("Ingrese el numero de cedula (10 digitos): ");
                
                // ! La ejecucion se detiene aqui hasta que se apruebe el algoritmo Modulo 10
                leer_cedula_valida(nuevo_cliente.cedula);
                
                printf("=> Estructura aceptada: Cedula %s validada matematicamente.\n", nuevo_cliente.cedula);
                break;
                
            case 2:
                printf("\n[ Modulo de Transacciones ]\n");
                printf("Ingrese el monto a depositar: $");
                
                // ! La ejecucion se detiene aqui hasta recibir un formato financiero estricto
                monto_prueba = leerMontoValido(); 
                
                printf("=> Logica aceptada: Monto $%.2f procesado exitosamente.\n", monto_prueba);
                break;
                
            case 3:
                printf("\n[ Modulo de Transferencias en construccion ]\n");
                break;
                
            case 4:
                printf("\n[ Modulo de Reportes en construccion ]\n");
                break;
                
            case 5:
                printf("\n[ Modulo de Boveda en construccion ]\n");
                break;
                
            case 6:
                printf("\nCerrando sistema y liberando memoria...\n");
                break;
                
            default:
                printf("\nOpcion no valida. Intente de nuevo.\n");
        }
    } while(opcion != 6);

    return 0;
}