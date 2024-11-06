#ifndef GESTOR_ARTISTAS_H
#define GESTOR_ARTISTAS_H

#include <glib.h>
#include "../include/entidades/artists.h"

typedef struct GestorArtistas GestorArtistas;

// Funções para inicialização e limpeza do gestor de artistas
void inicializar_gestor_artistas(GestorArtistas *gestor);
void liberar_gestor_artistas(GestorArtistas *gestor);
void inserir_artista(GestorArtistas *gestor, Artista *artista);

#endif
