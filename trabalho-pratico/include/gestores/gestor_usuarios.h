#ifndef GESTOR_USUARIOS_H
#define GESTOR_USUARIOS_H

#include <glib.h>
#include "../../include/entidades/usuario.h"

typedef struct {
    GHashTable *usuarios; // Tabela hash para armazenar usuários
} GestorUsuarios;

void inicializar_gestor_usuarios(GestorUsuarios *gestor);
void liberar_gestor_usuarios(GestorUsuarios *gestor);
void adicionar_usuario(GestorUsuarios *gestor, Usuario *usuario);
Usuario* buscar_usuario(GestorUsuarios *gestor, const gchar *username);
void imprimir_usuario(GestorUsuarios *gestor, const gchar *username) ;

#endif // GESTOR_USUARIOS_H
