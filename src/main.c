#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "modelos.h"
#include "validaciones.h"
#include "cuentas.h"
#include "transacciones.h"
#include "acceso.h"
#include "persistencia.h"
#include "algoritmos.h"
#include "reportes.h"

#define MAX_CLIENTES_BANCO 100 

// * Funcion centralizada de control de acceso (RBAC)
int verificar_permisos(Rol rol_usuario, int opcion_deseada)
{
    switch (opcion_deseada)
    {
        case 1: // Gestion de cuentas (Registro y Apertura de cuentas adicionales)
            return (rol_usuario == USUARIO || rol_usuario == ADMIN);
            
        case 2: // Depositos (Restringido a ventanilla)
            return (rol_usuario == CAJERO);
            
        case 3: // Transferencias
        case 4: // Retiros
            return (rol_usuario == USUARIO || rol_usuario == CAJERO);
            
        case 5: // Estado de cuenta y Exportacion TXT
            return (rol_usuario == USUARIO || rol_usuario == CAJERO || rol_usuario == GERENTE);
            
        case 6: // Auditoria de Boveda (Recursiva)
        case 7: // Ranking de Clientes (Quicksort)
            return (rol_usuario == GERENTE);
            
        case 8: // Cerrar sesion
            return 1; // Acceso universal
            
        default:
            return 0;
    }
}

int main(void) 
{
    srand(time(NULL));

    // * Inicializa archivo persistente de accesos si no existe
    inicializar_usuarios_predeterminados();

    Cliente banco[MAX_CLIENTES_BANCO] = {0}; 
    int total_clientes = 0; 

    // * Carga inicial de datos desde disco al arrancar el programa
    total_clientes = cargar_sistema(banco);

    int programa_activo = 1;

    // * BUCLE SUPERIOR: Pantalla de Bienvenida y Autorregistro
    while (programa_activo)
    {
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
            // * Salida absoluta: Persistencia forzada y liberacion total de memoria
            printf("\nGuardando base de datos y cerrando sistema de forma segura...\n");
            guardar_sistema(banco, total_clientes);

            // ! Barrido completo de la estructura bidimensional dinamica (Heap)
            for (int i = 0; i < total_clientes; i++) {
                for (int j = 0; j < banco[i].num_cuentas; j++) {
                    if (banco[i].cuentas[j].historial != NULL) {
                        free(banco[i].cuentas[j].historial);
                        banco[i].cuentas[j].historial = NULL; 
                    }
                }
            }
            printf("=> Memoria liberada. Sistema fuera de linea.\n");
            programa_activo = 0;
            break;
        }
        else if (op_inicio == 2)
        {
            // * Flujo de Autorregistro (Sign Up) Exterior
            printf("\n[ MODULO DE AUTORREGISTRO ]\n");
            
            char cedula_temporal[15]; 
            if (leer_cedula_valida(cedula_temporal) == 0) {
                printf("=> Operacion cancelada. Retornando a la pantalla de bienvenida...\n");
                continue;
            }
            
            int indice_cliente = buscar_cliente_por_cedula(banco, total_clientes, cedula_temporal);
            
            if (indice_cliente != -1) {
                printf("\n========================================\n");
                printf("[AVISO DEL SISTEMA]\n");
                printf("Motivo: Cliente ya existente.\n");
                printf("Accion: Usted ya se encuentra registrado en el banco.\n");
                printf("Por favor, utilice la opcion '1. Iniciar sesion'.\n");
                printf("========================================\n");
            } else {
                if (total_clientes >= MAX_CLIENTES_BANCO) {
                    printf("\n========================================\n");
                    printf("[ERROR DE REGISTRO]\n");
                    printf("Motivo: Base de datos saturada.\n");
                    printf("========================================\n");
                    continue;
                }
                
                // * Insercion ordenada garantizada para la Busqueda Binaria
                int pos_insercion = obtener_indice_insercion(banco, total_clientes, cedula_temporal);
                hacer_espacio_cliente(banco, &total_clientes, pos_insercion);
                registrar_cliente(&banco[pos_insercion], cedula_temporal);
                
                // ! CRITICO: Guardado persistente inmediato tras el autorregistro
                guardar_sistema(banco, total_clientes);
                printf("=> Base de datos actualizada.\n");
                printf("=> Ahora puede iniciar sesion con su cedula y contrasena.\n");
            }
        }
        else if (op_inicio == 1)
        {
            // * Flujo de Inicio de Sesion (Login)
            Usuario usuario_actual;
            int sesion_activa = iniciar_sesion(&usuario_actual);

            if (!sesion_activa)
            {
                continue; // * Reintento al bucle superior
            }

            // * BUCLE INTERMEDIO: Panel operativo de la sesion del usuario actual
            int menu_sesion_activo = 1;

            do {
                printf("\n========================================\n");
                printf("         POLI BANK - PANEL OPERATIVO    \n");
                printf("========================================\n");
                printf("Usuario Activo:       %s\n", usuario_actual.username);
                printf("Rol de Acceso:        %s\n", obtener_nombre_rol(usuario_actual.rol));
                
                // * Privacidad de metricas: Solo Admin y Gerente ven clientes registrados
                if (usuario_actual.rol == ADMIN || usuario_actual.rol == GERENTE)
                {
                    printf("Clientes Registrados: %d / %d\n", total_clientes, MAX_CLIENTES_BANCO);
                }
                printf("----------------------------------------\n");
                
                // * Menu Dinamico UX: Impresion condicional basada en roles
                if (verificar_permisos(usuario_actual.rol, 1)) printf("1. Gestion de Cuentas (Aperturar Extra)\n");
                if (verificar_permisos(usuario_actual.rol, 2)) printf("2. Depositos\n");
                if (verificar_permisos(usuario_actual.rol, 3)) printf("3. Transferencias\n");
                if (verificar_permisos(usuario_actual.rol, 4)) printf("4. Retiros\n");
                if (verificar_permisos(usuario_actual.rol, 5)) printf("5. Estado de Cuenta (Impresion y Exportacion)\n");
                if (verificar_permisos(usuario_actual.rol, 6)) printf("6. Auditoria de Boveda [EXCLUSIVO GERENCIA]\n");
                if (verificar_permisos(usuario_actual.rol, 7)) printf("7. Ranking de Clientes [EXCLUSIVO GERENCIA]\n");
                printf("8. Cerrar sesion\n");
                printf("========================================\n");
                printf("Ingrese una opcion: ");
                
                char entrada_opcion[10];
                leerCadenaSegura(entrada_opcion, 10);
                int opcion = atoi(entrada_opcion);

                if (opcion == 8)
                {
                    // * Cerrar sesion: Guardar datos, vaciar memoria volatil y retornar a login
                    printf("\nGuardando cambios de la sesion activa en el disco...\n");
                    guardar_sistema(banco, total_clientes);

                    // ! Barrido de memoria Heap para evitar fugas entre sesiones
                    for (int i = 0; i < total_clientes; i++) {
                        for (int j = 0; j < banco[i].num_cuentas; j++) {
                            if (banco[i].cuentas[j].historial != NULL) {
                                free(banco[i].cuentas[j].historial);
                                banco[i].cuentas[j].historial = NULL; 
                            }
                        }
                    }
                    
                    total_clientes = 0; 
                    memset(banco, 0, sizeof(banco));

                    printf("=> Sesion cerrada de forma exitosa.\n");
                    
                    // * Carga una nueva copia limpia de los datos para la proxima sesion
                    total_clientes = cargar_sistema(banco);

                    menu_sesion_activo = 0;
                    break;
                }

                // ! Validacion estricta en el controlador (Por si el usuario adivina el numero de opcion)
                if (opcion >= 1 && opcion <= 7)
                {
                    if (!verificar_permisos(usuario_actual.rol, opcion))
                    {
                        printf("\n========================================\n");
                        printf("[ERROR DE AUTORIZACION]\n");
                        printf("Motivo: Acceso denegado por Politicas de Seguridad.\n");
                        printf("Causal: Su rol actual (%s) no posee autorizacion.\n", obtener_nombre_rol(usuario_actual.rol));
                        printf("========================================\n");
                        continue; 
                    }
                }

                switch(opcion) {
                    case 1:
                        printf("\n[ MODULO DE GESTION DE CUENTAS ]\n");
                        
                        char cedula_temporal[15]; 
                        
                        // * Optimizacion UX: Autocompletado para el rol USUARIO
                        if (usuario_actual.rol == USUARIO)
                        {
                            strcpy(cedula_temporal, usuario_actual.username);
                            printf("[Sistema] Autenticacion verificada para el titular: %s\n", cedula_temporal);
                        }
                        else
                        {
                            // * El ADMIN debe ingresar manualmente la cedula a gestionar
                            if (leer_cedula_valida(cedula_temporal) == 0) {
                                printf("=> Operacion cancelada. Retornando al panel...\n");
                                break;
                            }
                        }
                        
                        // * Uso del motor de Busqueda Binaria (O(log n))
                        int indice_cliente = buscar_cliente_por_cedula(banco, total_clientes, cedula_temporal);
                        
                        if (indice_cliente != -1) {
                            Cliente *existente = &banco[indice_cliente];
                            if (existente->num_cuentas >= MAX_CUENTAS_POR_CLIENTE) {
                                printf("\n========================================\n");
                                printf("[ERROR DE GESTION]\n");
                                printf("Motivo: Limite de cuentas alcanzado.\n");
                                printf("Accion: No se permiten cuentas adicionales.\n");
                                printf("========================================\n");
                            } else {
                                printf("\nCliente registrado encontrado: %s\n", existente->nombre_completo);
                                printf("Posee %d cuentas activas. Desea aperturar una nueva? (S/N): ", existente->num_cuentas);
                                char conf[10];
                                leerCadenaSegura(conf, 10);
                                if (conf[0] == 'S' || conf[0] == 's') {
                                    registrar_cliente(existente, cedula_temporal);
                                } else {
                                    printf("=> Operacion cancelada.\n");
                                }
                            }
                        } else {
                            // * Permite al Admin registrar a un cliente desde adentro
                            if (total_clientes >= MAX_CLIENTES_BANCO) {
                                printf("\n========================================\n");
                                printf("[ERROR DE REGISTRO]\n");
                                printf("Motivo: Base de datos saturada.\n");
                                printf("========================================\n");
                                break;
                            }
                            
                            int pos_insercion = obtener_indice_insercion(banco, total_clientes, cedula_temporal);
                            hacer_espacio_cliente(banco, &total_clientes, pos_insercion);
                            registrar_cliente(&banco[pos_insercion], cedula_temporal);
                        }
                        break;
                        
                    case 2:
                        printf("\n[ MODULO DE DEPOSITOS ]\n");
                        
                        if (total_clientes == 0) {
                            printf("Error: Sistema vacio. No hay cuentas registradas.\n");
                            break;
                        }

                        char cedula_busqueda[15];
                        if (usuario_actual.rol == USUARIO)
                        {
                            strcpy(cedula_busqueda, usuario_actual.username);
                            printf("[Sistema] Cuenta destino verificada: %s\n", cedula_busqueda);
                        }
                        else
                        {
                            if (leer_cedula_valida(cedula_busqueda) == 0) break;
                        }

                        int indice_destino = buscar_cliente_por_cedula(banco, total_clientes, cedula_busqueda);

                        if (indice_destino == -1) {
                            printf("\nError: Cliente no encontrado en la base de datos.\n");
                            break;
                        }

                        Cliente *dest = &banco[indice_destino];
                        printf("\nCuentas registradas para %s:\n", dest->nombre_completo);
                        for (int i = 0; i < dest->num_cuentas; i++) {
                            printf("- Cuenta #%d (%s) | Saldo actual: $%.2f\n", dest->cuentas[i].numero_cuenta, dest->cuentas[i].tipo_cuenta, dest->cuentas[i].saldo_actual);
                        }

                        printf("\nIngrese el numero de cuenta (6 digitos): ");
                        char num_cta_str[15];
                        leerCadenaSegura(num_cta_str, 15);
                        int num_cta = atoi(num_cta_str);

                        printf("Ingrese el monto a depositar (ej. 15, 120.50): $");
                        float monto_prueba = leerMontoValido(); 
                        
                        procesar_deposito(dest, num_cta, monto_prueba);
                        break;
                        
                    case 3:
                        printf("\n[ MODULO DE TRANSFERENCIAS ]\n");

                        if (total_clientes == 0) {
                            printf("Error: Se requieren registros previos para transferir.\n");
                            break;
                        }

                        printf("\n[Identificacion del Emisor]\n");
                        char cedula_origen[15];
                        if (usuario_actual.rol == USUARIO)
                        {
                            strcpy(cedula_origen, usuario_actual.username);
                            printf("[Sistema] Origen verificado: %s\n", cedula_origen);
                        }
                        else
                        {
                            if (leer_cedula_valida(cedula_origen) == 0) break;
                        }
                        
                        int idx_origen = buscar_cliente_por_cedula(banco, total_clientes, cedula_origen);
                        if (idx_origen == -1) {
                            printf("\nError: Emisor inexistente.\n");
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

                        printf("\n[Identificacion del Receptor]\n");
                        char cedula_destino[15];
                        if (leer_cedula_valida(cedula_destino) == 0) break;
                        
                        int idx_dest = buscar_cliente_por_cedula(banco, total_clientes, cedula_destino);
                        if (idx_dest == -1) {
                            printf("\nError: Receptor inexistente.\n");
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

                        if (cta_orig == cta_dest) {
                            printf("\nError: Auto-transferencia denegada.\n");
                            break;
                        }

                        printf("\nIngrese el monto a transferir (ej. 15.50): $");
                        float monto_transferencia = leerMontoValido(); 

                        procesar_transferencia(cli_origen, cta_orig, cli_destino, cta_dest, monto_transferencia);
                        break;
                        
                    case 4:
                        printf("\n[ MODULO DE RETIROS ]\n");
                        
                        if (total_clientes == 0) {
                            printf("Error: No existen clientes registrados.\n");
                            break;
                        }

                        char cedula_retiro[15];
                        if (usuario_actual.rol == USUARIO)
                        {
                            strcpy(cedula_retiro, usuario_actual.username);
                            printf("[Sistema] Autenticacion verificada: %s\n", cedula_retiro);
                        }
                        else
                        {
                            if (leer_cedula_valida(cedula_retiro) == 0) break;
                        }

                        int idx_retiro = buscar_cliente_por_cedula(banco, total_clientes, cedula_retiro);

                        if (idx_retiro == -1) {
                            printf("\nError: Cliente no ubicado.\n");
                            break;
                        }

                        Cliente *cli_ret = &banco[idx_retiro];
                        printf("\nCuentas asociadas a %s:\n", cli_ret->nombre_completo);
                        for (int i = 0; i < cli_ret->num_cuentas; i++) {
                            printf("- Cuenta #%d (%s) | Saldo Disponible: $%.2f\n", cli_ret->cuentas[i].numero_cuenta, cli_ret->cuentas[i].tipo_cuenta, cli_ret->cuentas[i].saldo_actual);
                        }

                        printf("Ingrese el numero de cuenta a debitar: ");
                        char cta_ret_str[15];
                        leerCadenaSegura(cta_ret_str, 15);
                        int cta_ret = atoi(cta_ret_str);

                        printf("Ingrese el monto a retirar: $");
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
                        if (usuario_actual.rol == USUARIO)
                        {
                            strcpy(cedula_consulta, usuario_actual.username);
                            printf("[Sistema] Autenticacion verificada: %s\n", cedula_consulta);
                        }
                        else
                        {
                            if (leer_cedula_valida(cedula_consulta) == 0) break;
                        }

                        int idx_consulta = buscar_cliente_por_cedula(banco, total_clientes, cedula_consulta);

                        if (idx_consulta == -1) {
                            printf("\nError: Cliente no encontrado.\n");
                            break;
                        }
                        
                        imprimir_estado_cuenta(&banco[idx_consulta]);
                        break;
                        
                    case 6:
                        printf("\n[ MODULO DE AUDITORIA DE BOVEDA ]\n");
                        
                        // * Integracion del Motor Recursivo
                        float acumulado_boveda = calcular_boveda_recursivo(banco, total_clientes, 0, 0);

                        printf("\n========================================\n");
                        printf("         AUDITORIA GENERAL DE BOVEDA    \n");
                        printf("========================================\n");
                        printf("Clientes en Base:     %d\n", total_clientes);
                        printf("Liquidez en Boveda:   $%.2f\n", acumulado_boveda);
                        printf("Estado del Sistema:   AUDITADO Y OPERATIVO\n");
                        printf("========================================\n");
                        break;

                    case 7:
                        printf("\n[ MODULO DE INTELIGENCIA DE NEGOCIOS ]\n");
                        generar_ranking_saldos(banco, total_clientes);
                        break;
                        
                    default:
                        if (opcion != 8) {
                            printf("\nOpcion no valida. Por favor intente de nuevo.\n");
                        }
                        break;
                }
            } while (menu_sesion_activo);
        }
        else
        {
            printf("Opcion invalida. Intente de nuevo.\n");
        }
    }

    return 0;
}