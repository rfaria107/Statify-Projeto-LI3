#ifndef GESTOR_USUARIOS_H
#define GESTOR_USUARIOS_H

#include <glib.h>
#include "../include/entidades/usuario.h"

typedef struct GestorUsuarios GestorUsuarios;
typedef struct Usuario Usuario;

GestorUsuarios* criar_gestor_usuarios();
void inicializar_gestor_usuarios(GestorUsuarios *gestor);
void liberar_gestor_usuarios(GestorUsuarios *gestor);
void adicionar_usuario(GestorUsuarios *gestor, Usuario* usuario);
Usuario* buscar_usuario(GestorUsuarios *gestor, Usuario* usuario);
GHashTable *get_hash_usuarios(GestorUsuarios *gestor);
void inserir_usuario(GestorUsuarios *gestor, Usuario *user);

#endif // GESTOR_USUARIOS_H
