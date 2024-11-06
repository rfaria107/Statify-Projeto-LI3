#ifndef GESTOR_MUSICAS_H
#define GESTOR_MUSICAS_H

#include <glib.h>
#include "../include/entidades/musica.h"

typedef struct GestorMusicas GestorMusicas;

// Funções para inicialização e limpeza do gestor de músicas
void inicializar_gestor_musicas(GestorMusicas* gestor);
void liberar_gestor_musicas(GestorMusicas* gestor);

#endif
