#ifndef ARTISTA_H
#define ARTISTA_H

#include <glib.h>

typedef struct Artista Artista;
    
// Funções de acesso aos atributos do artistas
gchar *get_artist_id(Artista *artista);
gchar *get_artist_name(Artista *artista);
gdouble get_artist_recipe_per_stream(Artista *artista);
gchar **get_artist_id_constituent(Artista *artista);
gchar *get_artist_country(Artista *artista);
gchar *get_artist_type(Artista *artista);
gint get_artist_duracao_disco(Artista *artista);

void set_artista_duracao_disco(Artista *artista, gint nova_duracao);

// Funções para manipulação de artistas
void free_artista(Artista *artista);
Artista *inicializar_artista();
Artista *create_artista(gchar *id, gchar *name, gdouble recipe_per_stream, gchar **id_constituent, gchar *country, gchar *type);

#endif // ARTISTA_H
