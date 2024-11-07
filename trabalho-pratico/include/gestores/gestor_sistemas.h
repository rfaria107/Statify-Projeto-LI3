#ifndef GESTOR_SISTEMA_H
#define GESTOR_SISTEMA_H

#include <glib.h>
#include "../include/gestores/gestor_artistas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/gestores/gestor_musicas.h"

typedef struct GestorSistema GestorSistema;

// Funções para inicialização e limpeza do sistema
GestorSistema* criar_gestor_sistema();
void inicializar_gestor_sistema(GestorSistema *gestor);
void liberar_gestor_sistema(GestorSistema *gestor);
GestorUsuarios* get_gestor_usuarios(GestorSistema* gestor);
GestorArtistas* get_gestor_artistas(GestorSistema* gestor);
GestorMusicas* get_gestor_musicas(GestorSistema* gestor);

#endif