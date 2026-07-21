#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h> 
#include "acceso.h"
#include "validaciones.h"

#define ARCHIVO_USUARIOS "usuarios.dat"

void inicializar_usuarios_predeterminados(void)
{
    FILE *archivo = fopen(ARCHIVO_USUARIOS, "rb");
    
    if (archivo != NULL)
    {
        fclose(archivo);
        return;
    }

    archivo = fopen(ARCHIVO_USUARIOS, "wb");
    if (archivo == NULL)
    {
        printf("Error critico: No se pudo crear el archivo de credenciales.\n");
        return;
    }

    Usuario predeterminados[] = {
        {"admin", "admin", ADMIN},
        {"gerente", "gerente", GERENTE},
        {"cajero", "cajero", CAJERO},
        {"usuario", "usuario", USUARIO}
    };

    int cantidad = sizeof(predeterminados) / sizeof(Usuario);
    fwrite(predeterminados, sizeof(Usuario), cantidad, archivo);
    fclose(archivo);
}

int iniciar_sesion(Usuario *usuario_logueado)
{
    char ingresado_user[50];
    char ingresado_pass[50];

    printf("\n========================================\n");
    printf("         POLI BANK - INICIO DE SESION   \n");
    printf("========================================\n");
    printf("Ingrese su usuario (o 'X' para cancelar): ");
    leerCadenaSegura(ingresado_user, 50);
    
    if (strcmp(ingresado_user, "X") == 0 || strcmp(ingresado_user, "x") == 0) {
        printf("=> Inicio de sesion cancelado.\n");
        return 0;
    }
    
    printf("Ingrese su contrasena (Enter en blanco para cancelar): ");
    
    int i = 0;
    char ch;
    
    while (1)
    {
        ch = getch();
        
        if (ch == 13 || ch == 10)
        {
            ingresado_pass[i] = '\0';
            break;
        }
        else if (ch == 8)
        {
            if (i > 0)
            {
                i--;
                printf("\b \b"); 
            }
        }
        else if (ch >= 32 && ch <= 126)
        {
            if (i < 49) 
            {
                ingresado_pass[i++] = ch;
                printf("*");
            }
        }
    }
    printf("\n");

    if (strlen(ingresado_pass) == 0) {
        printf("=> Inicio de sesion cancelado.\n");
        return 0;
    }

    FILE *archivo = fopen(ARCHIVO_USUARIOS, "rb");
    if (archivo == NULL)
    {
        printf("Error: Base de datos de acceso no encontrada.\n");
        return 0;
    }

    Usuario temp;
    int coincidencia = 0;

    while (fread(&temp, sizeof(Usuario), 1, archivo) == 1)
    {
        if (strcmp(temp.username, ingresado_user) == 0 && strcmp(temp.password, ingresado_pass) == 0)
        {
            *usuario_logueado = temp;
            coincidencia = 1;
            break;
        }
    }

    fclose(archivo);

    if (coincidencia)
    {
        printf("\n========================================\n");
        printf("[ACCESO PERMITIDO]\n");
        printf("Usuario: %s\n", usuario_logueado->username);
        printf("Rol:     %s\n", obtener_nombre_rol(usuario_logueado->rol));
        printf("========================================\n");
        return 1;
    }
    else
    {
        printf("\nError: Credenciales invalidas. Intente de nuevo.\n");
        return 0;
    }
}

const char* obtener_nombre_rol(Rol rol)
{
    switch (rol)
    {
        case GERENTE: return "GERENTE";
        case ADMIN:   return "ADMINISTRADOR";
        case CAJERO:  return "CAJERO";
        case USUARIO: return "USUARIO / CLIENTE";
        default:      return "DESCONOCIDO";
    }
}

void registrar_nuevo_usuario(const char* username, const char* password)
{
    FILE *archivo = fopen(ARCHIVO_USUARIOS, "ab"); 
    if (archivo != NULL)
    {
        Usuario nuevo;
        strcpy(nuevo.username, username);
        strcpy(nuevo.password, password);
        nuevo.rol = USUARIO;
        
        fwrite(&nuevo, sizeof(Usuario), 1, archivo);
        fclose(archivo);
    }
    else
    {
        printf("Error: No se pudo abrir la base de datos de credenciales para el registro.\n");
    }
}