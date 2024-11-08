#ifndef MUSICA_H
#define MUSICA_H

#include <glib.h>

typedef struct Musica Musica;

// Funções para manipulação de usuários
Musica *create_musica(char *id, char *title, char **artist_ids, char *duration, char *genre, int year, char *lyrics);
Musica *inicializar_musica();
void free_musica(Musica *musica);

// Funções de acesso aos atributos do usuário
gchar *get_music_id(Musica *musica);
gchar *get_music_title(Musica *musica);
gchar **get_music_artist_ids(Musica *musica);
gchar *get_music_duration(Musica *musica);
gchar *get_music_genre(Musica *musica);
gint get_music_year(Musica *musica);
gchar *get_music_lyrics(Musica *musica);

#endif // MUSICA_H
