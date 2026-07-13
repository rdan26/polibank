#ifndef VALIDACIONES_H
#define VALIDACIONES_H 

#define MAX_BUFFER_ENTRADA 50

// Declaración de funciones de validación
void limpiarBuffer(void);
void leerCadenaSegura(char* destino, int longitud_max);
float leerMontoValido(void);

//* Nuevas funciones de validación
int validarCedula(const char *cedula);
int validacionFecha(int dia, int mes, int year);
int validarCuenta(const char* usuario, const char* password);

#endif // VALIDACIONES_H