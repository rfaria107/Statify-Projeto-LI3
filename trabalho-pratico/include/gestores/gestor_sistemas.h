#ifndef GESTOR_SISTEMA_H
#define GESTOR_SISTEMA_H

#include "gestor_usuarios.h"
#include "gestor_artistas.h"
#include "gestor_musicas.h"

typedef struct {
    GestorUsuarios gestor_usuarios; // Gestor de usuários
    GestorArtistas gestor_artistas; // Gestor de artistas
    GestorMusicas gestor_musicas;   // Gestor de músicas
} GestorSistema;

// Funções para inicialização e limpeza do sistema
void inicializar_gestor_sistema(GestorSistema *gestor);
void liberar_gestor_sistema(GestorSistema *gestor);

#endif
