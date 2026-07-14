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
        printf("         POLI BANK - SISTEMA BASE       \n");
        printf("========================================\n");
        printf("Clientes Registrados: %d / %d\n", total_clientes, MAX_CLIENTES_BANCO);
        printf("----------------------------------------\n");
        printf("1. Gestion de Cuentas (Registro)\n");
        printf("2. Depositos\n");
        printf("3. Transferencias\n");
        printf("4. Retiros\n");
        printf("5. Estado de Cuenta (Historial)\n");
        printf("6. Salir\n");
        printf("========================================\n");
        printf("Ingrese una opcion: ");
        
        // * Seguridad contra caracteres invalidos en el menu principal
        char entrada_opcion[10];
        leerCadenaSegura(entrada_opcion, 10);
        
        // * atoi convierte el texto a numero 
        // ! Si detecta letras, devuelve 0 
        opcion = atoi(entrada_opcion);

        switch(opcion) {
            case 1:
                printf("\n[ MODULO DE REGISTRO ]\n");
                
                // * Barrera contra desbordamiento del arreglo de clientes
                if (total_clientes >= MAX_CLIENTES_BANCO) {
                    printf("Error: La capacidad maxima del banco (%d clientes) ha sido alcanzada.\n", MAX_CLIENTES_BANCO);
                    break;
                }

                printf("Ingrese el numero de cedula (10 digitos, o 'X' para cancelar): ");
                char cedula_temporal[15]; 
                
                // ! Intercepcion del estado: 0 significa que el usuario aborto la operacion
                if (leer_cedula_valida(cedula_temporal) == 0) {
                    printf("=> Operacion cancelada por el usuario. Retornando al menu...\n");
                    break;
                }
                
                // * Motor de busqueda con bloqueo de duplicados
                if (buscar_cliente_por_cedula(banco, total_clientes, cedula_temporal) != -1) {
                    printf("Error: La cedula %s ya pertenece a una cuenta existente.\n", cedula_temporal);
                    break; // ! Aborta el registro
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

                printf("Ingrese la cedula de la cuenta a la cual desea depositar (o 'X' para cancelar): ");
                char cedula_busqueda[15];
                
                // ! Intercepcion del estado: 0 significa que el usuario aborto la operacion
                if (leer_cedula_valida(cedula_busqueda) == 0) {
                    printf("=> Operacion cancelada por el usuario. Retornando al menu...\n");
                    break;
                }

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

                // ! Mecanismo de escape
                printf("Es este el destinatario correcto? (S para confirmar, N para cancelar): ");
                char confirmacion[10];
                leerCadenaSegura(confirmacion, 10);

                if (confirmacion[0] == 'N' || confirmacion[0] == 'n')
                {
                    printf("=> Operacion cancelada por el usuario. Retornando al menu...\n");
                    break;
                }
                else if (confirmacion[0] != 'S' && confirmacion[0] != 's')
                {
                    printf("=> Entrada no reconocida. Abortando transaccion por seguridad...\n");
                    break;
                }

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
                printf("\n[ MODULO DE TRANSFERENCIAS ]\n");

                // ! Se necesitan al menos 2 clientes en el sistema
                if (total_clientes < 2) {
                    printf("Error: Se requieren al menos 2 clientes registrados para realizar una transferencia.\n");
                    break;
                }

                // * Localizacion del origen
                printf("Ingrese la cedula de la cuenta ORIGEN (o 'X' para cancelar): ");
                char cedula_origen[15];
                if (leer_cedula_valida(cedula_origen) == 0) {
                    printf("=> Operacion cancelada. Retornando al menu...\n");
                    break;
                }
                
                int indice_origen = buscar_cliente_por_cedula(banco, total_clientes, cedula_origen);
                if (indice_origen == -1) {
                    printf("Error: No se encontro la cuenta de origen.\n");
                    break;
                }

                // * Localizacion del destino
                printf("Ingrese la cedula de la cuenta DESTINO (o 'X' para cancelar): ");
                char cedula_destino[15];
                if (leer_cedula_valida(cedula_destino) == 0) {
                    printf("=> Operacion cancelada. Retornando al menu...\n");
                    break;
                }
                
                int indice_destino_trans = buscar_cliente_por_cedula(banco, total_clientes, cedula_destino);
                if (indice_destino_trans == -1) {
                    printf("Error: No se encontro la cuenta de destino.\n");
                    break;
                }

                // ! Barrera 2: Bloqueo de auto-transferencia (mismo indice de memoria)
                if (indice_origen == indice_destino_trans) {
                    printf("Error: Operacion invalida. No puede transferir fondos a la misma cuenta.\n");
                    break;
                }

                // * Despliegue y confirmacion
                printf("\n----------------------------------------\n");
                printf("ORIGEN:  %s (Saldo Disponible: $%.2f)\n", banco[indice_origen].nombre_completo, banco[indice_origen].cuenta_bancaria.saldo_actual);
                printf("DESTINO: %s\n", banco[indice_destino_trans].nombre_completo);
                printf("----------------------------------------\n");

                printf("Confirmar transaccion? (S para confirmar, N para cancelar): ");
                char conf_trans[10];
                leerCadenaSegura(conf_trans, 10);

                if (conf_trans[0] == 'N' || conf_trans[0] == 'n') {
                    printf("=> Operacion cancelada por el usuario. Retornando al menu...\n");
                    break;
                } else if (conf_trans[0] != 'S' && conf_trans[0] != 's') {
                    printf("=> Entrada no reconocida. Abortando transaccion por seguridad...\n");
                    break;
                }

                // * Validacion matematica de fondos
                printf("Ingrese el monto a transferir (ej. 15.00, 120.50): $");
                float monto_transferencia = leerMontoValido(); 

                // ! Prevencion de sobregiro
                if (monto_transferencia > banco[indice_origen].cuenta_bancaria.saldo_actual) {
                    printf("\nError CRITICO: Fondos insuficientes.\n");
                    printf("=> Saldo actual: $%.2f | Monto solicitado: $%.2f\n", banco[indice_origen].cuenta_bancaria.saldo_actual, monto_transferencia);
                    break;
                }

                // * Inyeccion de memoria sinmultanea
                procesar_transferencia(&banco[indice_origen], &banco[indice_destino_trans], monto_transferencia);
                break;
                
            case 4:
                printf("\n[ MODULO DE RETIROS ]\n");
                
                // ! No operar si el banco esta vacio
                if (total_clientes == 0) {
                    printf("Error: No hay clientes registrados en el sistema.\n");
                    break;
                }

                printf("Ingrese su numero de cedula (o 'X' para cancelar): ");
                char cedula_retiro[15];
                
                if (leer_cedula_valida(cedula_retiro) == 0) {
                    printf("=> Operacion cancelada por el usuario. Retornando al menu...\n");
                    break;
                }

                int indice_retiro = buscar_cliente_por_cedula(banco, total_clientes, cedula_retiro);

                if (indice_retiro == -1) {
                    printf("Error: No se encontro ningun cliente con esa cedula.\n");
                    break;
                }

                printf("\n----------------------------------------\n");
                printf("Cliente: %s\n", banco[indice_retiro].nombre_completo);
                printf("Saldo Disponible: $%.2f\n", banco[indice_retiro].cuenta_bancaria.saldo_actual);
                printf("----------------------------------------\n");

                printf("Es esta su cuenta? (S para confirmar, N para cancelar): ");
                char conf_retiro[10];
                leerCadenaSegura(conf_retiro, 10);

                if (conf_retiro[0] == 'N' || conf_retiro[0] == 'n') {
                    printf("=> Operacion cancelada. Retornando al menu...\n");
                    break;
                } else if (conf_retiro[0] != 'S' && conf_retiro[0] != 's') {
                    printf("=> Entrada no reconocida. Abortando transaccion...\n");
                    break;
                }

                printf("Ingrese el monto a retirar (ej. 15.00, 120.50): $");
                float monto_retiro = leerMontoValido(); 

                // ! Barrera de proteccion: Restriccion de limite de fondos
                if (monto_retiro > banco[indice_retiro].cuenta_bancaria.saldo_actual) {
                    printf("\nError CRITICO: Fondos insuficientes para realizar el retiro.\n");
                    printf("=> Saldo disponible: $%.2f\n", banco[indice_retiro].cuenta_bancaria.saldo_actual);
                    break;
                }
                
                procesar_retiro(&banco[indice_retiro], monto_retiro);
                break;
                
            case 5:
                printf("\n[ MODULO DE ESTADO DE CUENTA ]\n");
                
                if (total_clientes == 0) {
                    printf("Error: No hay clientes registrados en el sistema.\n");
                    break;
                }

                printf("Ingrese el numero de cedula a consultar (o 'X' para cancelar): ");
                char cedula_consulta[15];
                
                if (leer_cedula_valida(cedula_consulta) == 0) {
                    printf("=> Operacion cancelada. Retornando al menu...\n");
                    break;
                }

                int indice_consulta = buscar_cliente_por_cedula(banco, total_clientes, cedula_consulta);

                if (indice_consulta == -1) {
                    printf("Error: No se encontro ninguna cuenta con esa cedula.\n");
                    break;
                }
                
                // * Despliegue de la memoria en modo solo lectura
                imprimir_estado_cuenta(&banco[indice_consulta]);
                break;
                
            case 6:
                printf("\n[ PROTOCOLO DE DESTRUCCION ]\n");
                printf("Cerrando sistema y liberando memoria RAM...\n");
                
                // ! Barrido de memoria dinamica (Heap)
                for (int i = 0; i < total_clientes; i++) {
                    if (banco[i].cuenta_bancaria.historial != NULL) {
                        free(banco[i].cuenta_bancaria.historial);
                        banco[i].cuenta_bancaria.historial = NULL; // !Prevencion de punteros colgantes (Dangling Pointers)
                    }
                }
                
                printf("=> Memoria liberada exitosamente. Hasta pronto.\n");
                break;
                
            default:
                printf("\nOpcion no valida. Intente de nuevo.\n");
        }
    } while(opcion != 6);

    return 0;
}