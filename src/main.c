#include <stdio.h>
#include <stdlib.h>
#include "modelos.h"
#include "validaciones.h"

int main() {
    int opcion;
    float monto_prueba;

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
        
        // Usamos la validación del Dev 2 adaptada para enteros
        // (El Dev 2 debe crear una variante de leer_monto_valido que retorne int)
        scanf("%d", &opcion);
        limpiarBuffer(); // Crucial después de cada scanf

        switch(opcion) {
            case 1:
                printf("\n[ Modulo de Registro en construccion ]\n");
                // Aqui ira tu funcion: registrar_cliente(...)
                break;
            case 2:
                printf("\n[ Modulo de Transacciones ]\n");
                printf("Ingrese el monto a depositar: ");
                monto_prueba = leerMontoValido(); // Uso de la funcion segura
                printf("Monto %.2f procesado exitosamente (simulacion).\n", monto_prueba);
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