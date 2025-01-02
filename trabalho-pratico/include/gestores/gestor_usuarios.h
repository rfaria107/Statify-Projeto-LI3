#ifndef GESTOR_USUARIOS_H
#define GESTOR_USUARIOS_H

#include <glib.h>
#include "../include/entidades/usuario.h"

typedef struct GestorUsuarios GestorUsuarios;

GestorUsuarios* criar_gestor_usuarios();
void inicializar_gestor_usuarios(GestorUsuarios *gestor);
void liberar_gestor_usuarios(GestorUsuarios *gestor);
Usuario* buscar_usuario(GestorUsuarios *gestor, Usuario* usuario);
Usuario *buscar_usuario_id(GestorUsuarios *gestor, int id_usuario);
GHashTable *get_hash_usuarios(GestorUsuarios *gestor);
void inserir_usuario(GestorUsuarios *gestor, Usuario *user);
void free_usuario_value(gpointer value);
#endif // GESTOR_USUARIOS_H
