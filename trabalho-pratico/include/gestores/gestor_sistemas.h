#ifndef GESTOR_SISTEMA_H
#define GESTOR_SISTEMA_H

#include "../include/gestores/gestor_musicas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/gestores/gestor_artistas.h"

typedef struct GestorSistema GestorSistema;

// Funções para inicialização e limpeza do sistema
void inicializar_gestor_sistema(GestorSistema *gestor);
void liberar_gestor_sistema(GestorSistema *gestor);

#endif
