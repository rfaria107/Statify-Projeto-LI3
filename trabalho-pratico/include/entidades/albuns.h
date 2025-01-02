#ifndef ALBUM_H
#define ALBUM_H

#include <glib.h>

typedef struct Album Album;

// Funções de acesso aos atributos do usuário
gint get_album_id (Album *album);
gchar *get_album_title (Album *album);
gint get_album_year (Album *album);
gchar **get_album_artist_ids(Album *album);
gchar **get_album_producers(Album *album);

// Funções para manipulação de usuários
Album *create_album(gint id, gchar *title, gchar **artist_ids, gint year, gchar **producers);
Album *inicializar_album();
void free_album(Album *album);

#endif // ALBUM
