/* ====== acceso.h ====== */

#ifndef ACCESO_H
#define ACCESO_H

#include "modelos.h"

void inicializar_usuarios_predeterminados(void);
int iniciar_sesion(Usuario *usuario_logueado);
const char* obtener_nombre_rol(Rol rol);
void registrar_nuevo_usuario(const char* username, const char* password);

#endif // ACCESO_H