#ifndef GESTOR_MUSICAS_H
#define GESTOR_MUSICAS_H

#include <glib.h>

typedef struct GestorMusicas GestorMusicas;
typedef struct Musica Musica;

// Funções para inicialização e limpeza do gestor de músicas
void inicializar_gestor_musicas(GestorMusicas *gestor);
void liberar_gestor_musicas(GestorMusicas *gestor);

#endif
