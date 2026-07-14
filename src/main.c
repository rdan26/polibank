#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "modelos.h"
#include "validaciones.h"
#include "cuentas.h"
#include "transacciones.h"

#define MAX_CLIENTES_BANCO 100 

int main(void) 
{
    srand(time(NULL));

    int opcion;
    float monto_prueba;
    
    Cliente banco[MAX_CLIENTES_BANCO] = {0}; 
    int total_clientes = 0; 

    do {
        printf("\n========================================\n");
        printf("         POLI BANK - PLATAFORMA         \n");
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
        
        char entrada_opcion[10];
        leerCadenaSegura(entrada_opcion, 10);
        opcion = atoi(entrada_opcion);

        switch(opcion) {
            case 1:
                printf("\n[ MODULO DE REGISTRO / APERTURA DE CUENTAS ]\n");
                
                char cedula_temporal[15]; 
                if (leer_cedula_valida(cedula_temporal) == 0) {
                    printf("=> Operacion cancelada por el usuario. Retornando al menu principal...\n");
                    break;
                }
                
                int indice_cliente = buscar_cliente_por_cedula(banco, total_clientes, cedula_temporal);
                
                if (indice_cliente != -1) {
                    // Cliente ya existe en el sistema
                    Cliente *existente = &banco[indice_cliente];
                    if (existente->num_cuentas >= MAX_CUENTAS_POR_CLIENTE) {
                        printf("\n========================================\n");
                        printf("[ERROR DE REGISTRO]\n");
                        printf("Motivo: Limite de cuentas alcanzado.\n");
                        printf("Causal: El cliente '%s' ya tiene el maximo permitido de %d cuentas.\n", existente->nombre_completo, MAX_CUENTAS_POR_CLIENTE);
                        printf("Accion: Ninguna operacion de registro adicional es viable.\n");
                        printf("========================================\n");
                    } else {
                        printf("\nCliente registrado encontrado: %s\n", existente->nombre_completo);
                        printf("Posee %d cuentas activas. ¿Aperturar una nueva cuenta? (S/N): ", existente->num_cuentas);
                        char conf[10];
                        leerCadenaSegura(conf, 10);
                        if (conf[0] == 'S' || conf[0] == 's') {
                            registrar_cliente(existente, cedula_temporal);
                        } else {
                            printf("=> Operacion de registro cancelada.\n");
                        }
                    }
                } else {
                    // Cliente nuevo
                    if (total_clientes >= MAX_CLIENTES_BANCO) {
                        printf("\n========================================\n");
                        printf("[ERROR DE REGISTRO]\n");
                        printf("Motivo: Base de datos saturada.\n");
                        printf("Causal: Se ha alcanzado el limite maximo de %d clientes en el sistema.\n", MAX_CLIENTES_BANCO);
                        printf("========================================\n");
                        break;
                    }
                    
                    registrar_cliente(&banco[total_clientes], cedula_temporal);
                    total_clientes++; 
                }
                break;
                
            case 2:
                printf("\n[ MODULO DE DEPOSITOS ]\n");
                
                if (total_clientes == 0) {
                    printf("Error: No hay clientes en el sistema. Debe registrar un cliente primero.\n");
                    break;
                }

                char cedula_busqueda[15];
                if (leer_cedula_valida(cedula_busqueda) == 0) {
                    printf("=> Operacion cancelada por el usuario.\n");
                    break;
                }

                int indice_destino = buscar_cliente_por_cedula(banco, total_clientes, cedula_busqueda);

                if (indice_destino == -1) {
                    printf("\n========================================\n");
                    printf("[ERROR DE BUSQUEDA]\n");
                    printf("Motivo: Destinatario no encontrado.\n");
                    printf("Causal: Ningun cliente coincide con la cedula '%s'.\n", cedula_busqueda);
                    printf("Accion: Revise que el titular se encuentre registrado.\n");
                    printf("========================================\n");
                    break;
                }

                Cliente *dest = &banco[indice_destino];
                printf("\nCuentas registradas para %s:\n", dest->nombre_completo);
                for (int i = 0; i < dest->num_cuentas; i++) {
                    printf("- Cuenta #%d (%s) | Saldo actual: $%.2f\n", dest->cuentas[i].numero_cuenta, dest->cuentas[i].tipo_cuenta, dest->cuentas[i].saldo_actual);
                }

                printf("\nIngrese el numero de cuenta (6 digitos) para el deposito: ");
                char num_cta_str[15];
                leerCadenaSegura(num_cta_str, 15);
                int num_cta = atoi(num_cta_str);

                printf("Ingrese el monto a depositar (ej. 15, 120.50): $");
                monto_prueba = leerMontoValido(); 
                
                procesar_deposito(dest, num_cta, monto_prueba);
                break;
                
            case 3:
                printf("\n[ MODULO DE TRANSFERENCIAS ]\n");

                if (total_clientes == 0) {
                    printf("Error: Se requieren registros previos de clientes para transferir.\n");
                    break;
                }

                // * Localización de Origen
                printf("\n[Identificacion del Emisor]\n");
                char cedula_origen[15];
                if (leer_cedula_valida(cedula_origen) == 0) {
                    printf("=> Operacion cancelada.\n");
                    break;
                }
                
                int idx_origen = buscar_cliente_por_cedula(banco, total_clientes, cedula_origen);
                if (idx_origen == -1) {
                    printf("\n========================================\n");
                    printf("[ERROR DE IDENTIFICACION]\n");
                    printf("Motivo: Emisor inexistente.\n");
                    printf("Causal: No se encontro un cliente bajo la cedula %s.\n", cedula_origen);
                    printf("========================================\n");
                    break;
                }

                Cliente *cli_origen = &banco[idx_origen];
                printf("\nCuentas del emisor (%s):\n", cli_origen->nombre_completo);
                for (int i = 0; i < cli_origen->num_cuentas; i++) {
                    printf("- Cuenta #%d (%s) | Saldo: $%.2f\n", cli_origen->cuentas[i].numero_cuenta, cli_origen->cuentas[i].tipo_cuenta, cli_origen->cuentas[i].saldo_actual);
                }

                printf("Ingrese el numero de cuenta de origen: ");
                char cta_orig_str[15];
                leerCadenaSegura(cta_orig_str, 15);
                int cta_orig = atoi(cta_orig_str);

                // * Localización de Destino
                printf("\n[Identificacion del Receptor]\n");
                char cedula_destino[15];
                if (leer_cedula_valida(cedula_destino) == 0) {
                    printf("=> Operacion cancelada.\n");
                    break;
                }
                
                int idx_dest = buscar_cliente_por_cedula(banco, total_clientes, cedula_destino);
                if (idx_dest == -1) {
                    printf("\n========================================\n");
                    printf("[ERROR DE IDENTIFICACION]\n");
                    printf("Motivo: Receptor inexistente.\n");
                    printf("Causal: No se encontro un cliente bajo la cedula %s.\n", cedula_destino);
                    printf("========================================\n");
                    break;
                }

                Cliente *cli_destino = &banco[idx_dest];
                printf("\nCuentas del receptor (%s):\n", cli_destino->nombre_completo);
                for (int i = 0; i < cli_destino->num_cuentas; i++) {
                    printf("- Cuenta #%d (%s)\n", cli_destino->cuentas[i].numero_cuenta, cli_destino->cuentas[i].tipo_cuenta);
                }

                printf("Ingrese el numero de cuenta del destinatario: ");
                char cta_dest_str[15];
                leerCadenaSegura(cta_dest_str, 15);
                int cta_dest = atoi(cta_dest_str);

                // ! Bloqueo de auto-transferencia en la misma cuenta física
                if (cta_orig == cta_dest) {
                    printf("\n========================================\n");
                    printf("[ERROR DE OPERACION]\n");
                    printf("Motivo: Auto-transferencia denegada.\n");
                    printf("Causal: No se permite transaccionar fondos hacia la misma cuenta de origen.\n");
                    printf("========================================\n");
                    break;
                }

                printf("\nIngrese el monto a transferir (ej. 15, 120.50): $");
                float monto_transferencia = leerMontoValido(); 

                procesar_transferencia(cli_origen, cta_orig, cli_destino, cta_dest, monto_transferencia);
                break;
                
            case 4:
                printf("\n[ MODULO DE RETIROS ]\n");
                
                if (total_clientes == 0) {
                    printf("Error: No existen clientes para transaccionar.\n");
                    break;
                }

                char cedula_retiro[15];
                if (leer_cedula_valida(cedula_retiro) == 0) {
                    printf("=> Operacion cancelada.\n");
                    break;
                }

                int idx_retiro = buscar_cliente_por_cedula(banco, total_clientes, cedula_retiro);

                if (idx_retiro == -1) {
                    printf("\n========================================\n");
                    printf("[ERROR DE IDENTIFICACION]\n");
                    printf("Motivo: Cliente no registrado.\n");
                    printf("Causal: Los registros no contemplan la identificacion %s.\n", cedula_retiro);
                    printf("========================================\n");
                    break;
                }

                Cliente *cli_ret = &banco[idx_retiro];
                printf("\nCuentas asociadas a %s:\n", cli_ret->nombre_completo);
                for (int i = 0; i < cli_ret->num_cuentas; i++) {
                    printf("- Cuenta #%d (%s) | Saldo Disponible: $%.2f\n", cli_ret->cuentas[i].numero_cuenta, cli_ret->cuentas[i].tipo_cuenta, cli_ret->cuentas[i].saldo_actual);
                }

                printf("Ingrese el numero de cuenta desde la cual desea retirar: ");
                char cta_ret_str[15];
                leerCadenaSegura(cta_ret_str, 15);
                int cta_ret = atoi(cta_ret_str);

                printf("Ingrese el monto a retirar (ej. 15, 120.50): $");
                float monto_retiro = leerMontoValido(); 

                procesar_retiro(cli_ret, cta_ret, monto_retiro);
                break;
                
            case 5:
                printf("\n[ MODULO DE ESTADO DE CUENTA ]\n");
                
                if (total_clientes == 0) {
                    printf("Error: No hay datos registrados.\n");
                    break;
                }

                char cedula_consulta[15];
                if (leer_cedula_valida(cedula_consulta) == 0) {
                    printf("=> Operacion cancelada.\n");
                    break;
                }

                int idx_consulta = buscar_cliente_por_cedula(banco, total_clientes, cedula_consulta);

                if (idx_consulta == -1) {
                    printf("\n========================================\n");
                    printf("[ERROR DE IDENTIFICACION]\n");
                    printf("Motivo: Cliente no ubicado.\n");
                    printf("Causal: No se hallo informacion asociada a la cedula %s.\n", cedula_consulta);
                    printf("========================================\n");
                    break;
                }
                
                imprimir_estado_cuenta(&banco[idx_consulta]);
                break;
                
            case 6:
                printf("\n[ PROTOCOLO DE DESTRUCCION DE MEMORIA ]\n");
                printf("Liberando recursos asignados en el Heap...\n");
                
                // ! Barrido completo de la estructura bidimensional dinámica
                for (int i = 0; i < total_clientes; i++) {
                    for (int j = 0; j < banco[i].num_cuentas; j++) {
                        if (banco[i].cuentas[j].historial != NULL) {
                            free(banco[i].cuentas[j].historial);
                            banco[i].cuentas[j].historial = NULL; 
                        }
                    }
                }
                
                printf("=> Memoria liberada de forma segura. Apagando terminal.\n");
                break;
                
            default:
                printf("\nOpcion no valida. Por favor intente de nuevo.\n");
        }
    } while(opcion != 6);

    return 0;
}