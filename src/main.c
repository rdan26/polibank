#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "modelos.h"
#include "validaciones.h"
#include "cuentas.h"
#include "transacciones.h"

// * Limite de base de datos en RAM para no agotar la memoria del sistema
#define MAX_CLIENTES_BANCO 100 

int main(void) 
{
    // * Seed aleatoria usando el reloj del procesador
    srand(time(NULL));

    int opcion;
    float monto_prueba;
    
    // * Arreglo de estructuras inicializado en cero absoluto
    Cliente banco[MAX_CLIENTES_BANCO] = {0}; 
    int total_clientes = 0; // ! Contador para saber donde escribir el siguiente registro

    do {
        printf("\n========================================\n");
        printf("          POLI BANK - SISTEMA BASE        \n");
        printf("========================================\n");
        printf("Clientes Registrados: %d / %d\n", total_clientes, MAX_CLIENTES_BANCO);
        printf("----------------------------------------\n");
        printf("1. Gestion de Cuentas (Registro)\n");
        printf("2. Transacciones (Depositos)\n");
        printf("3. Transferencias\n");
        printf("4. Reportes y Ranking\n");
        printf("5. Boveda (Cierre de Jornada)\n");
        printf("6. Salir\n");
        printf("========================================\n");
        printf("Ingrese una opcion: ");
        
        scanf("%d", &opcion);
        limpiarBuffer(); 

        switch(opcion) {
            case 1:
                printf("\n[ MODULO DE REGISTRO ]\n");
                
                // * Barrera contra desbordamiento del arreglo de clientes
                if (total_clientes >= MAX_CLIENTES_BANCO) {
                    printf("Error: La capacidad maxima del banco (%d clientes) ha sido alcanzada.\n", MAX_CLIENTES_BANCO);
                    break;
                }

                printf("Ingrese el numero de cedula (10 digitos): ");
                char cedula_temporal[15]; 
                leer_cedula_valida(cedula_temporal);
                
                // * Motor de busqueda con bloqueo de duplicados
                if (buscar_cliente_por_cedula(banco, total_clientes, cedula_temporal) != -1) {
                    printf("Error: La cedula %s ya pertenece a una cuenta existente.\n", cedula_temporal);
                    break; // Aborta el registro
                }
                
                // * Asignacion en memoria, apunta exactamente al espacio libre 'total_clientes'
                registrar_cliente(&banco[total_clientes], cedula_temporal);
                
                // * Actualizamos el contador del sistema
                total_clientes++; 
                break;
                
            case 2:
                printf("\n[ MODULO DE DEPOSITOS ]\n");
                
                // ! No operar si el banco esta vacio
                if (total_clientes == 0) {
                    printf("Error: No hay clientes registrados en el sistema. Registre uno primero (Opcion 1).\n");
                    break;
                }

                printf("Ingrese la cedula de la cuenta a la cual desea depositar: ");
                char cedula_busqueda[15];
                leer_cedula_valida(cedula_busqueda);

                // * Busqueda del indice de destino
                int indice_destino = buscar_cliente_por_cedula(banco, total_clientes, cedula_busqueda);

                // ! Si retorna -1, el cliente no existe
                if (indice_destino == -1) {
                    printf("Error: No se encontro ningun cliente con la cedula %s.\n", cedula_busqueda);
                    break;
                }

                printf("\n----------------------------------------\n");
                printf("Cliente Destino: %s\n", banco[indice_destino].nombre_completo);
                printf("Numero de Cuenta: %d\n", banco[indice_destino].cuenta_bancaria.numero_cuenta);
                printf("----------------------------------------\n");
                printf("INFO: El sistema requiere un formato financiero estricto.\n");
                printf("Utilice el punto (.) para los decimales y proporcione\n");
                printf("exactamente dos digitos al final.\n");
                printf("----------------------------------------\n");
                
                printf("Ingrese el monto a depositar (ej. 15.00, 120.50): $");
                monto_prueba = leerMontoValido(); 
                
                // * Inyeccion de memoria en el puntero exacto del cliente encontrado
                procesar_deposito(&banco[indice_destino], monto_prueba);
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
                // La liberacion de los bloques de malloc (free) de todos los clientes ira aqui
                break;
                
            default:
                printf("\nOpcion no valida. Intente de nuevo.\n");
        }
    } while(opcion != 6);

    return 0;
}