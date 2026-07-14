#ifndef VALIDACIONES_H
#define VALIDACIONES_H

#define MAX_BUFFER_ENTRADA 50 

void limpiarBuffer(void);
void leerCadenaSegura(char* destino, int longitud_max);
float leerMontoValido(void);
int validar_cedula_ecuatoriana(const char *cedula);
int leer_cedula_valida(char *destino);

#endif // VALIDACIONES_H