#ifndef GESTOR_ALBUNS_H
#define GESTOR_ALBUNS_H

#include <glib.h>
#include "../include/entidades/albuns.h"

typedef struct GestorAlbuns GestorAlbuns;

// Funções para inicialização e limpeza do gestor de artistas
void free_album_value(gpointer value);
GHashTable *get_hash_albuns(GestorAlbuns *gestor);
GestorAlbuns* criar_gestor_albuns();
void inicializar_gestor_albuns(GestorAlbuns *gestor);
void liberar_gestor_albuns(GestorAlbuns *gestor);
void inserir_album(GestorAlbuns *gestor, Album *album);
Album* buscar_album(GestorAlbuns *gestor, int id);

#endif
