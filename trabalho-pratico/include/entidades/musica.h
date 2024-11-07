#ifndef MUSICA_H
#define MUSICA_H

#include <glib.h>
#include "../../include/gestores/gestor_sistemas.h"
#include "../../include/parsing/rowreader.h"
#include "../../include/gestores/gestor_musicas.h"

typedef struct Musica Musica;
typedef struct GestorMusicas GestorMusicas;

// Funções para manipulação de usuários
Musica *create_musica(int id, char *title, char **artist_ids, char *duration, char *genre, int year, char *lyrics);
char **parse_liked_musics(RowReader *reader);
Musica *inicializar_musica();
void free_musica(Musica* musica);

// Funções de acesso aos atributos do usuário
gint get_music_id(Musica *musica);
gchar *get_music_title(Musica *musica);
gchar **get_music_artist_ids(Musica *musica);
gchar *get_music_duration(Musica *musica);
gchar *get_music_genre(Musica *musica);
gint get_music_year(Musica *musica);
gchar *get_music_lyrics(Musica *musica);

#endif // MUSICA_H
