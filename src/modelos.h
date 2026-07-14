#ifndef MODELOS_H
#define MODELOS_H

#define MAX_NOMBRE 100
#define MAX_CEDULA 15
#define MAX_TIPO_CUENTA 20

// * Transacción
typedef struct 
{
    int id_transaccion;
    char tipo[20];       // "Deposito", "Retiro", "Transferencia"
    float monto;
    char fecha[11];      // Formato "DD/MM/YYYY"
} Transaccion;

// * Cuenta
typedef struct 
{
    int numero_cuenta;
    char tipo_cuenta[MAX_TIPO_CUENTA]; // "Ahorros", "Corriente"
    float saldo_actual;
    
    // * Manejo de memoria dinámica para listas de transacciones de tamaño variable
    Transaccion *historial; 
    int num_transacciones;     // Cuántas transacciones existen actualmente
    int capacidad_historial;   // Cuánta memoria hemos reservado (para usar con realloc)
} Cuenta;

// * Cliente
typedef struct 
{
    char cedula[MAX_CEDULA];
    char nombre_completo[MAX_NOMBRE]; // Manejo de strings para nombres
    Cuenta cuenta_bancaria;
} Cliente;

#endif