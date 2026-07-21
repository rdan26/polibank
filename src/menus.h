/* ====== menus.h ====== */

#ifndef MENUS_H
#define MENUS_H

#include "modelos.h"

// Desacoplamiento de interfaces de usuario
void menu_gestion_cuentas(Cliente banco[], int *total_clientes, Usuario *usuario_actual);
void menu_depositos(Cliente banco[], int total_clientes, Usuario *usuario_actual);
void menu_transferencias(Cliente banco[], int total_clientes, Usuario *usuario_actual);
void menu_retiros(Cliente banco[], int total_clientes, Usuario *usuario_actual);
void menu_estado_cuenta(Cliente banco[], int total_clientes, Usuario *usuario_actual);

#endif // MENUS_H