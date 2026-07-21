#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "cuentas.h"
#include "validaciones.h"
#include "acceso.h"

int registrar_cliente(Cliente *cliente, const char *cedula_validada)
{
    char nueva_password[50] = "";

    if (strlen(cliente->nombre_completo) == 0)
    {
        strcpy(cliente->cedula, cedula_validada);

        printf("Ingrese el nombre completo (nombre/s apellido/s) (o 'X' para cancelar): ");
        if (!leer_nombre_valido(cliente->nombre_completo)) return 0;

        printf("Cree una contrasena para el acceso digital (Enter en blanco para cancelar): ");
        leerCadenaSegura(nueva_password, 50);
        
        if (strlen(nueva_password) == 0) {
            printf("=> Operacion cancelada.\n");
            return 0;
        }
    }

    if (cliente->num_cuentas >= MAX_CUENTAS_POR_CLIENTE)
    {
        printf("\nError: El titular ya posee %d cuentas (limite maximo).\n", MAX_CUENTAS_POR_CLIENTE);
        return 0;
    }

    int indice_nueva_cuenta = cliente->num_cuentas;
    Cuenta *nueva_cuenta = &cliente->cuentas[indice_nueva_cuenta];

    nueva_cuenta->numero_cuenta = rand() % 900000 + 100000;
    
    printf("\nSeleccione el tipo de cuenta:\n");
    printf("1. Ahorros\n");
    printf("2. Corriente\n");
    printf("Ingrese una opcion (o 'X' para cancelar): ");
    
    char op_tipo[10];
    while (1)
    {
        leerCadenaSegura(op_tipo, 10);
        
        if (op_tipo[0] == 'X' || op_tipo[0] == 'x') {
            printf("=> Operacion cancelada.\n");
            return 0;
        }
        
        int op_num = atoi(op_tipo);
        
        if (op_num == 1)
        {
            strcpy(nueva_cuenta->tipo_cuenta, "Ahorros");
            break;
        }
        else if (op_num == 2)
        {
            strcpy(nueva_cuenta->tipo_cuenta, "Corriente");
            break;
        }
        else
        {
            printf("Error: Seleccion invalida. Ingrese 1 (Ahorros) o 2 (Corriente) (o 'X' para cancelar): ");
        }
    }

    nueva_cuenta->saldo_actual = 0.0f;
    nueva_cuenta->num_transacciones = 0;
    nueva_cuenta->capacidad_historial = CAPACIDAD_INICIAL_HISTORIAL;

    nueva_cuenta->historial = (Transaccion *)malloc(CAPACIDAD_INICIAL_HISTORIAL * sizeof(Transaccion));
    if (nueva_cuenta->historial == NULL)
    {
        printf("\nError CRITICO: Fallo al asignar memoria (malloc).\n");
        exit(1); 
    }

    cliente->num_cuentas++;

    if (strlen(nueva_password) > 0) {
        registrar_nuevo_usuario(cliente->cedula, nueva_password);
        printf("=> Credenciales creadas exitosamente. Usuario de acceso: %s\n", cliente->cedula);
    }

    printf("\n========================================\n");
    printf("[REGISTRO EXITOSO]\n");
    printf("Cliente:            %s\n", cliente->nombre_completo);
    printf("Cedula:             %s\n", cliente->cedula);
    printf("Cuenta Aperturada:  #%d (%s)\n", nueva_cuenta->numero_cuenta, nueva_cuenta->tipo_cuenta);
    printf("Cuentas Totales:    %d / %d\n", cliente->num_cuentas, MAX_CUENTAS_POR_CLIENTE);
    printf("========================================\n");
    
    return 1;
}

void actualizar_datos_cliente(Cliente *cliente)
{
    printf("\n--- ACTUALIZACION DE DATOS PERSONALES ---\n");
    printf("Cliente actual: %s\n", cliente->nombre_completo);
    
    char temp_nombre[MAX_NOMBRE];
    printf("Ingrese el nuevo nombre completo (nombre/s apellido/s) (o 'X' para cancelar): ");
    
    if (leer_nombre_valido(temp_nombre) == 0) return;
    
    strcpy(cliente->nombre_completo, temp_nombre);
    printf("\n=> Datos actualizados correctamente. Nuevo nombre: %s\n", cliente->nombre_completo);
}

void cerrar_cuenta(Cliente banco[], int *total_clientes, int indice_cliente, int numero_cuenta)
{
    Cliente *cli = &banco[indice_cliente];
    int idx_cuenta = -1;

    for (int i = 0; i < cli->num_cuentas; i++) {
        if (cli->cuentas[i].numero_cuenta == numero_cuenta) {
            idx_cuenta = i; 
            break;
        }
    }

    if (idx_cuenta == -1) {
        printf("Error: La cuenta ingresada no existe en su perfil.\n");
        return;
    }

    if (cli->cuentas[idx_cuenta].saldo_actual > 0.0f) {
        printf("Error: Transaccion denegada. La cuenta tiene fondos disponibles ($%.2f).\n", cli->cuentas[idx_cuenta].saldo_actual);
        printf("Realice un retiro o transferencia total de sus fondos antes de intentar cerrar la cuenta.\n");
        return;
    }

    if (cli->cuentas[idx_cuenta].historial != NULL) {
        free(cli->cuentas[idx_cuenta].historial);
        cli->cuentas[idx_cuenta].historial = NULL;
    }

    for (int i = idx_cuenta; i < cli->num_cuentas - 1; i++) {
        cli->cuentas[i] = cli->cuentas[i + 1];
    }

    memset(&cli->cuentas[cli->num_cuentas - 1], 0, sizeof(Cuenta));
    cli->num_cuentas--;

    printf("\n=> Cuenta #%d cerrada y eliminada exitosamente del sistema.\n", numero_cuenta);
}