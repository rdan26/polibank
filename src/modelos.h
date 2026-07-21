/* ====== modelos.h ====== */

#ifndef MODELOS_H
#define MODELOS_H

#define MAX_NOMBRE 100
#define MAX_CEDULA 15
#define MAX_TIPO_CUENTA 20
#define MAX_CUENTAS_POR_CLIENTE 3
#define MAX_CLIENTES_BANCO 100

// Definicion de roles del sistema
typedef enum
{
    GERENTE = 0,
    ADMIN = 1,
    CAJERO = 2,
    USUARIO = 3
} Rol;

// Estructura para credenciales y RBAC
typedef struct
{
    char username[50];
    char password[50];
    Rol rol;
} Usuario;

// Transaccion
typedef struct 
{
    int id_transaccion;
    char tipo[100];       // "Deposito", "Retiro", "Transferencia"
    float monto;
    char fecha[11];      // Formato "DD/MM/YYYY"
} Transaccion;

// Cuenta
typedef struct 
{
    int numero_cuenta;
    char tipo_cuenta[MAX_TIPO_CUENTA]; // "Ahorros", "Corriente"
    float saldo_actual;
    
    // Manejo de memoria dinamica para listas de transacciones de tamano variable
    Transaccion *historial; 
    int num_transacciones;     
    int capacidad_historial;   
} Cuenta;

// Cliente
typedef struct 
{
    char cedula[MAX_CEDULA];
    char nombre_completo[MAX_NOMBRE]; 
    Cuenta cuentas[MAX_CUENTAS_POR_CLIENTE];
    int num_cuentas;
} Cliente;

#endif