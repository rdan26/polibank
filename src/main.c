#include <stdio.h>
#include <stdlib.h>
#include "modelos.h"
#include "cuentas.h"
#include "validaciones.h"
#include "transacciones.h"

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
                printf("\n[ MODULO DE REGISTRO ]\n");
                printf("Ingrese el numero de cedula (10 digitos): ");
                
                // * Se valida la cedula en un buffer temporal
                char cedula_temporal[15]; 
                leer_cedula_valida(cedula_temporal);
                
                // * Se inyecta la memoria temporal pura hacia el constructor del cliente
                // * Pasamos &nuevo_cliente para que 'registrar_cliente' modifique la memoria original
                registrar_cliente(&nuevo_cliente, cedula_temporal);
                break;
                
            case 2:
            printf("\n[ MODULO DE DEPOSITOS ]\n");

            // * Validacion de seguridad -> evita que se deposite si el cliente no existe
            // * Usamos el numero de cuenta para saber si la inicializacion de 'case 1' ya ocurrio
            if (nuevo_cliente.cuenta_bancaria.numero_cuenta == 0)
            {
                printf("Error: Debe registrar un cliente primero (Opcion 1).\n");
                break;
            }

            printf("Ingrese el monto a depositar: $");
            monto_prueba = leerMontoValido(); 

            // * Procesamiento matematico y de memoria
            procesar_deposito(&nuevo_cliente, monto_prueba);
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