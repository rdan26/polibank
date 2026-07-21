#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "modelos.h"
#include "validaciones.h"
#include "cuentas.h"
#include "acceso.h"
#include "persistencia.h"
#include "algoritmos.h"
#include "reportes.h"
#include "menus.h"

#define MAX_CLIENTES_BANCO 100 

int verificar_permisos(Rol rol_usuario, int opcion_deseada)
{
    switch (opcion_deseada)
    {
        case 1: return (rol_usuario == USUARIO || rol_usuario == ADMIN);
        case 2: return (rol_usuario == CAJERO);
        case 3: 
        case 4: return (rol_usuario == USUARIO || rol_usuario == CAJERO);
        case 5: return (rol_usuario == USUARIO || rol_usuario == CAJERO || rol_usuario == GERENTE);
        case 6: return (rol_usuario == USUARIO);
        case 7: 
        case 8: 
        case 9: return (rol_usuario == GERENTE);
        case 10: return 1; 
        default: return 0;
    }
}

int main(void) 
{
    srand(time(NULL));
    inicializar_usuarios_predeterminados();

    Cliente banco[MAX_CLIENTES_BANCO] = {0}; 
    int total_clientes = cargar_sistema(banco);
    int programa_activo = 1;

    while (programa_activo)
    {
        system("cls");
        printf("\n========================================\n");
        printf("         POLI BANK - ACCESO DIGITAL     \n");
        printf("========================================\n");
        printf("1. Iniciar sesion\n");
        printf("2. Registrarse (Aperturar Cuenta Nueva)\n");
        printf("3. Salir del programa\n");
        printf("========================================\n");
        printf("Ingrese una opcion: ");

        char entrada_inicio[10];
        leerCadenaSegura(entrada_inicio, 10);
        int op_inicio = atoi(entrada_inicio);

        if (op_inicio == 3)
        {
            printf("\nGuardando base de datos y cerrando sistema...\n");
            guardar_sistema(banco, total_clientes);
            liberar_memoria_sistema(banco, total_clientes); 
            printf("=> Memoria liberada. Sistema fuera de linea.\n");
            programa_activo = 0;
            break;
        }
        else if (op_inicio == 2)
        {
            printf("\n[ MODULO DE AUTORREGISTRO ]\n");
            char cedula_temporal[15]; 
            if (leer_cedula_valida(cedula_temporal) == 0) {
                printf("\nPresione Enter para continuar...");
                getchar();
                continue;
            }
            
            if (buscar_cliente_por_cedula(banco, total_clientes, cedula_temporal) != -1) {
                printf("Error: Cliente ya existente. Por favor inicie sesion.\n");
            } else {
                if (total_clientes >= MAX_CLIENTES_BANCO) {
                    printf("Error: Base de datos saturada.\n");
                    printf("\nPresione Enter para continuar...");
                    getchar();
                    continue;
                }
                
                int pos_insercion = obtener_indice_insercion(banco, total_clientes, cedula_temporal);
                hacer_espacio_cliente(banco, &total_clientes, pos_insercion);
                
                if (registrar_cliente(&banco[pos_insercion], cedula_temporal) == 1) {
                    guardar_sistema(banco, total_clientes);
                } else {
                    for (int i = pos_insercion; i < total_clientes - 1; i++) {
                        banco[i] = banco[i + 1];
                    }
                    memset(&banco[total_clientes - 1], 0, sizeof(Cliente));
                    total_clientes--;
                }
            }
            printf("\nPresione Enter para continuar...");
            getchar();
        }
        else if (op_inicio == 1)
        {
            Usuario usuario_actual;
            if (!iniciar_sesion(&usuario_actual)) {
                printf("\nPresione Enter para continuar...");
                getchar();
                continue;
            }

            int menu_sesion_activo = 1;

            do {
                system("cls");
                printf("\n========================================\n");
                printf("         POLI BANK - PANEL OPERATIVO    \n");
                
                if (usuario_actual.rol == USUARIO) {
                    int idx_saludo = buscar_cliente_por_cedula(banco, total_clientes, usuario_actual.username);
                    if (idx_saludo != -1) {
                        printf("Bienvenido(a), %s\n", banco[idx_saludo].nombre_completo);
                        printf("Cedula: %s | Rol: %s\n", banco[idx_saludo].cedula, obtener_nombre_rol(usuario_actual.rol));
                    } else {
                        printf("Usuario Activo: %s | Rol: %s\n", usuario_actual.username, obtener_nombre_rol(usuario_actual.rol));
                    }
                } else {
                    printf("Usuario Activo: %s | Rol: %s\n", usuario_actual.username, obtener_nombre_rol(usuario_actual.rol));
                }
                
                printf("----------------------------------------\n");
                
                if (verificar_permisos(usuario_actual.rol, 1))  printf("1. Gestion de Cuentas (CRUD)\n");
                if (verificar_permisos(usuario_actual.rol, 2))  printf("2. Depositos\n");
                if (verificar_permisos(usuario_actual.rol, 3))  printf("3. Transferencias\n");
                if (verificar_permisos(usuario_actual.rol, 4))  printf("4. Retiros\n");
                if (verificar_permisos(usuario_actual.rol, 5))  printf("5. Estado de Cuenta (TXT)\n");
                if (verificar_permisos(usuario_actual.rol, 6))  printf("6. Proyeccion de Intereses\n");
                if (verificar_permisos(usuario_actual.rol, 7))  printf("7. Auditoria de Boveda [TXT]\n");
                if (verificar_permisos(usuario_actual.rol, 8))  printf("8. Ranking de Clientes [GERENCIA]\n");
                if (verificar_permisos(usuario_actual.rol, 9))  printf("9. Exportar Balances [CSV]\n");
                printf("10. Cerrar sesion\n");
                printf("========================================\n");
                printf("Ingrese una opcion: ");
                
                char entrada_opcion[10];
                leerCadenaSegura(entrada_opcion, 10);
                int opcion = atoi(entrada_opcion);

                if (opcion == 10)
                {
                    guardar_sistema(banco, total_clientes);
                    liberar_memoria_sistema(banco, total_clientes); 
                    memset(banco, 0, sizeof(banco));
                    total_clientes = cargar_sistema(banco);
                    printf("=> Sesion cerrada de forma segura.\n");
                    printf("\nPresione Enter para continuar...");
                    getchar();
                    menu_sesion_activo = 0;
                    break;
                }

                if (opcion >= 1 && opcion <= 9 && !verificar_permisos(usuario_actual.rol, opcion))
                {
                    printf("Error: Acceso denegado por politicas de seguridad.\n");
                    printf("\nPresione Enter para continuar...");
                    getchar();
                    continue; 
                }

                switch(opcion) {
                    case 1: menu_gestion_cuentas(banco, &total_clientes, &usuario_actual); break;
                    case 2: menu_depositos(banco, total_clientes, &usuario_actual); break;
                    case 3: menu_transferencias(banco, total_clientes, &usuario_actual); break;
                    case 4: menu_retiros(banco, total_clientes, &usuario_actual); break;
                    case 5: menu_estado_cuenta(banco, total_clientes, &usuario_actual); break;
                    case 6: 
                        if (usuario_actual.rol == USUARIO) {
                            int idx = buscar_cliente_por_cedula(banco, total_clientes, usuario_actual.username);
                            if (idx != -1) proyectar_intereses_cliente(&banco[idx]);
                        } else {
                            printf("Funcion exclusiva para el auto-servicio de clientes.\n");
                        }
                        break;
                    case 7: {
                        float liquidez = calcular_boveda_recursivo(banco, total_clientes, 0, 0);
                        printf("\n[ AUDITORIA DE BOVEDA ]\nLiquidez Total: $%.2f\n", liquidez);
                        generar_log_boveda(liquidez, total_clientes);
                        break;
                    }
                    case 8: generar_ranking_saldos(banco, total_clientes); break;
                    case 9: exportar_balances_csv(banco, total_clientes); break;
                    default: printf("Opcion invalida.\n"); break;
                }
                
                guardar_sistema(banco, total_clientes);
                
                printf("\nPresione Enter para continuar...");
                getchar();
                
            } while (menu_sesion_activo);
        }
        else
        {
            printf("Error: Opcion invalida. Intente de nuevo.\n");
            printf("\nPresione Enter para continuar...");
            getchar();
        }
    }
    return 0;
}