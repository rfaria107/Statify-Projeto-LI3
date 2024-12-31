#ifndef GESTOR_MUSICAS_H
#define GESTOR_MUSICAS_H

#include <glib.h>
#include "../include/entidades/musica.h"

typedef struct GestorMusicas GestorMusicas;

// Funções para inicialização e limpeza do gestor de músicas
void inicializar_gestor_musicas(GestorMusicas* gestor);
void free_music_value(gpointer value);
GestorMusicas* criar_gestor_musicas();
void liberar_gestor_musicas(GestorMusicas* gestor);
Musica* buscar_musicas(GestorMusicas *gestor, int id); 
GHashTable *get_hash_musicas(GestorMusicas *gestor);
void inserir_musica(GestorMusicas *gestor, Musica *musica);
#endif
