#ifndef GESTOR_ARTISTAS_H
#define GESTOR_ARTISTAS_H

#include <glib.h>

typedef struct {
    GHashTable *artistas; // Tabela hash de artistas
} GestorArtistas;

// Funções para inicialização e limpeza do gestor de artistas
void inicializar_gestor_artistas(GestorArtistas *gestor);
void liberar_gestor_artistas(GestorArtistas *gestor);

#endif
