#ifndef ACCESO_H
#define ACCESO_H

#include "modelos.h"

// * Inicializa el archivo usuarios.dat con cuentas por defecto si no existe
void inicializar_usuarios_predeterminados(void);

// * Solicita credenciales y valida contra usuarios.dat usando mascara de asteriscos
int iniciar_sesion(Usuario *usuario_logueado);

// * Retorna la representacion textual del rol
const char* obtener_nombre_rol(Rol rol);

// * Registra las credenciales de un cliente nuevo de forma persistente
void registrar_nuevo_usuario(const char* username, const char* password);

#endif // ACCESO_H