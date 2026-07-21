#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menus.h"
#include "algoritmos.h"
#include "validaciones.h"
#include "cuentas.h"
#include "transacciones.h"

void menu_gestion_cuentas(Cliente banco[], int *total_clientes, Usuario *usuario_actual)
{
    printf("\n[ MODULO DE GESTION DE CUENTAS ]\n");
    
    char cedula_temporal[15]; 
    if (usuario_actual->rol == USUARIO)
    {
        strcpy(cedula_temporal, usuario_actual->username);
        printf("[Sistema] Autenticacion verificada para: %s\n", cedula_temporal);
    }
    else
    {
        if (leer_cedula_valida(cedula_temporal) == 0) return;
    }
    
    int indice_cliente = buscar_cliente_por_cedula(banco, *total_clientes, cedula_temporal);
    
    printf("\nOpciones de Gestion:\n");
    printf("1. Aperturar Nueva Cuenta\n");
    printf("2. Actualizar Datos Personales\n");
    printf("3. Cerrar / Eliminar Cuenta\n");
    printf("Ingrese opcion (o 'X' para cancelar): ");
    
    char op_str[10];
    leerCadenaSegura(op_str, 10);
    
    if ((op_str[0] == 'X' || op_str[0] == 'x') && op_str[1] == '\0') {
        printf("=> Operacion cancelada.\n");
        return;
    }
    
    int opcion = atoi(op_str);

    if (indice_cliente != -1) 
    {
        Cliente *existente = &banco[indice_cliente];
        
        if (opcion == 1) {
            registrar_cliente(existente, cedula_temporal);
        } else if (opcion == 2) {
            actualizar_datos_cliente(existente);
        } else if (opcion == 3) {
            if (existente->num_cuentas == 0) {
                printf("Error: El cliente no posee cuentas activas.\n");
                return;
            }
            printf("\nCuentas activas:\n");
            for(int i=0; i < existente->num_cuentas; i++) {
                printf("- Cuenta #%d | Saldo: $%.2f\n", existente->cuentas[i].numero_cuenta, existente->cuentas[i].saldo_actual);
            }
            printf("Ingrese el numero de cuenta a cerrar (o 'X' para cancelar): ");
            char cta_str[15];
            leerCadenaSegura(cta_str, 15);
            
            if ((cta_str[0] == 'X' || cta_str[0] == 'x') && cta_str[1] == '\0') {
                printf("=> Operacion cancelada.\n");
                return;
            }
            
            cerrar_cuenta(banco, total_clientes, indice_cliente, atoi(cta_str));
        } else {
            printf("Opcion invalida.\n");
        }
    } 
    else 
    {
        if (opcion == 1 && usuario_actual->rol == ADMIN) {
            if (*total_clientes >= MAX_CLIENTES_BANCO) {
                printf("Error: Base de datos saturada.\n");
                return;
            }
            int pos_insercion = obtener_indice_insercion(banco, *total_clientes, cedula_temporal);
            hacer_espacio_cliente(banco, total_clientes, pos_insercion);
            
            if (registrar_cliente(&banco[pos_insercion], cedula_temporal) == 0) {
                for (int i = pos_insercion; i < *total_clientes - 1; i++) {
                    banco[i] = banco[i + 1];
                }
                memset(&banco[*total_clientes - 1], 0, sizeof(Cliente));
                (*total_clientes)--;
            }
        } else {
            printf("Cliente no encontrado en el sistema.\n");
        }
    }
}

void menu_depositos(Cliente banco[], int total_clientes, Usuario *usuario_actual)
{
    printf("\n[ MODULO DE DEPOSITOS ]\n");
    char cedula_busqueda[15];
    
    if (usuario_actual->rol == USUARIO) {
        strcpy(cedula_busqueda, usuario_actual->username);
    } else {
        if (leer_cedula_valida(cedula_busqueda) == 0) return;
    }

    int indice_destino = buscar_cliente_por_cedula(banco, total_clientes, cedula_busqueda);
    if (indice_destino == -1) {
        printf("Error: Cliente no encontrado.\n");
        return;
    }

    Cliente *dest = &banco[indice_destino];
    
    if (dest->num_cuentas == 0) {
        printf("Error: El cliente no posee cuentas activas.\n");
        return;
    }
    
    printf("\nCuentas registradas para %s:\n", dest->nombre_completo);
    for (int i = 0; i < dest->num_cuentas; i++) {
        printf("- Cuenta #%d (%s) | Saldo: $%.2f\n", dest->cuentas[i].numero_cuenta, dest->cuentas[i].tipo_cuenta, dest->cuentas[i].saldo_actual);
    }

    printf("Ingrese numero de cuenta (o 'X' para cancelar): ");
    char cta_str[15]; leerCadenaSegura(cta_str, 15);
    
    if ((cta_str[0] == 'X' || cta_str[0] == 'x') && cta_str[1] == '\0') {
        printf("=> Operacion cancelada.\n");
        return;
    }
    
    printf("Ingrese monto a depositar (o 'X' para cancelar): $");
    float monto = leerMontoValido();
    
    if (monto == -1.0f) {
        printf("=> Operacion cancelada.\n");
        return;
    }
    
    procesar_deposito(dest, atoi(cta_str), monto);
}

void menu_transferencias(Cliente banco[], int total_clientes, Usuario *usuario_actual)
{
    printf("\n[ MODULO DE TRANSFERENCIAS ]\n");
    char cedula_origen[15];
    
    if (usuario_actual->rol == USUARIO) {
        strcpy(cedula_origen, usuario_actual->username);
    } else {
        printf("Datos del EMISOR:\n");
        if (leer_cedula_valida(cedula_origen) == 0) return;
    }
    
    int idx_origen = buscar_cliente_por_cedula(banco, total_clientes, cedula_origen);
    if (idx_origen == -1) { printf("Emisor no encontrado.\n"); return; }
    
    Cliente *cli_origen = &banco[idx_origen];
    
    if (cli_origen->num_cuentas == 0) {
        printf("Error: El cliente no posee cuentas activas.\n");
        return;
    }
    
    printf("\nCuentas disponibles (Origen):\n");
    for (int i = 0; i < cli_origen->num_cuentas; i++) {
        printf("- Cuenta #%d (%s) | Saldo disponible: $%.2f\n", cli_origen->cuentas[i].numero_cuenta, cli_origen->cuentas[i].tipo_cuenta, cli_origen->cuentas[i].saldo_actual);
    }
    
    printf("Ingrese cuenta de origen (o 'X' para cancelar): ");
    char cta_orig_str[15]; leerCadenaSegura(cta_orig_str, 15);
    
    if ((cta_orig_str[0] == 'X' || cta_orig_str[0] == 'x') && cta_orig_str[1] == '\0') {
        printf("=> Operacion cancelada.\n");
        return;
    }
    
    printf("\nDatos del RECEPTOR:\n");
    char cedula_destino[15];
    if (leer_cedula_valida(cedula_destino) == 0) return;
    
    int idx_dest = buscar_cliente_por_cedula(banco, total_clientes, cedula_destino);
    if (idx_dest == -1) { printf("Receptor no encontrado.\n"); return; }
    
    Cliente *cli_destino = &banco[idx_dest];
    
    if (cli_destino->num_cuentas == 0) {
        printf("Error: El receptor no posee cuentas activas.\n");
        return;
    }
    
    printf("\nCuentas activas del Receptor (%s):\n", cli_destino->nombre_completo);
    for (int i = 0; i < cli_destino->num_cuentas; i++) {
        printf("- Cuenta #%d (%s) | Saldo: $%.2f\n", cli_destino->cuentas[i].numero_cuenta, cli_destino->cuentas[i].tipo_cuenta, cli_destino->cuentas[i].saldo_actual);
    }
    
    printf("Ingrese cuenta de destino (o 'X' para cancelar): ");
    char cta_dest_str[15]; leerCadenaSegura(cta_dest_str, 15);
    
    if ((cta_dest_str[0] == 'X' || cta_dest_str[0] == 'x') && cta_dest_str[1] == '\0') {
        printf("=> Operacion cancelada.\n");
        return;
    }
    
    if (atoi(cta_orig_str) == atoi(cta_dest_str)) {
        printf("Error: Auto-transferencia denegada.\n");
        return;
    }
    
    printf("Ingrese monto a transferir (o 'X' para cancelar): $");
    float monto = leerMontoValido();
    
    if (monto == -1.0f) {
        printf("=> Operacion cancelada.\n");
        return;
    }
    
    procesar_transferencia(cli_origen, atoi(cta_orig_str), cli_destino, atoi(cta_dest_str), monto);
}

void menu_retiros(Cliente banco[], int total_clientes, Usuario *usuario_actual)
{
    printf("\n[ MODULO DE RETIROS ]\n");
    char cedula_retiro[15];
    
    if (usuario_actual->rol == USUARIO) {
        strcpy(cedula_retiro, usuario_actual->username);
    } else {
        if (leer_cedula_valida(cedula_retiro) == 0) return;
    }

    int idx_retiro = buscar_cliente_por_cedula(banco, total_clientes, cedula_retiro);
    if (idx_retiro == -1) { printf("Cliente no encontrado.\n"); return; }

    Cliente *cli_ret = &banco[idx_retiro];
    
    if (cli_ret->num_cuentas == 0) {
        printf("Error: El cliente no posee cuentas activas.\n");
        return;
    }
    
    printf("\nCuentas registradas para %s:\n", cli_ret->nombre_completo);
    for (int i = 0; i < cli_ret->num_cuentas; i++) {
        printf("- Cuenta #%d (%s) | Saldo disponible: $%.2f\n", cli_ret->cuentas[i].numero_cuenta, cli_ret->cuentas[i].tipo_cuenta, cli_ret->cuentas[i].saldo_actual);
    }

    printf("Ingrese cuenta a debitar (o 'X' para cancelar): ");
    char cta_str[15]; leerCadenaSegura(cta_str, 15);
    
    if ((cta_str[0] == 'X' || cta_str[0] == 'x') && cta_str[1] == '\0') {
        printf("=> Operacion cancelada.\n");
        return;
    }
    
    printf("Ingrese monto a retirar (o 'X' para cancelar): $");
    float monto = leerMontoValido();
    
    if (monto == -1.0f) {
        printf("=> Operacion cancelada.\n");
        return;
    }
    
    procesar_retiro(cli_ret, atoi(cta_str), monto);
}

void menu_estado_cuenta(Cliente banco[], int total_clientes, Usuario *usuario_actual)
{
    printf("\n[ MODULO DE ESTADO DE CUENTA ]\n");
    char cedula_consulta[15];
    
    if (usuario_actual->rol == USUARIO) {
        strcpy(cedula_consulta, usuario_actual->username);
    } else {
        if (leer_cedula_valida(cedula_consulta) == 0) return;
    }

    int idx_consulta = buscar_cliente_por_cedula(banco, total_clientes, cedula_consulta);
    if (idx_consulta != -1) {
        imprimir_estado_cuenta(&banco[idx_consulta]);
    } else {
        printf("Cliente no encontrado.\n");
    }
}