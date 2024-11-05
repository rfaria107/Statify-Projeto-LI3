#ifndef GESTOR_USUARIOS_H
#define GESTOR_USUARIOS_H

#include <glib.h>
#include "../entidades/usuario.h"

typedef struct GestorUsuarios GestorUsuarios;
typedef struct Usuario Usuario;

void inicializar_gestor_usuarios(GestorUsuarios *gestor);
void liberar_gestor_usuarios(GestorUsuarios *gestor);
void adicionar_usuario(GestorUsuarios *gestor, Usuario *usuario);
Usuario* buscar_usuario(GestorUsuarios *gestor, Usuario *usuario);

#endif // GESTOR_USUARIOS_H
