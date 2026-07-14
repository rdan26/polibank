#ifndef VALIDACIONES_H
#define VALIDACIONES_H

// ! Límite de memoria para el búfer temporal de lectura
#define MAX_BUFFER_ENTRADA 50 

// * --- Funciones de Seguridad de Memoria ---

// Drena la basura residual del stdin
void limpiarBuffer(void);

// * Lee texto respetando estrictamente el tamaño del arreglo
void leerCadenaSegura(char* destino, int longitud_max);


// * --- Funciones de Validación Lógica y Financiera ---

// * Retorna un flotante estrictamente formateado (positivo y con 2 decimales)
float leerMontoValido(void);

// * Algoritmo Módulo 10 puro (retorna 1 si es válida, 0 si es falsa)
int validar_cedula_ecuatoriana(const char *cedula);

// * Bucle que exige al usuario una cédula regionalmente válida
void leer_cedula_valida(char *destino);

#endif // VALIDACIONES_H