#ifndef MUSICA_H
#define MUSICA_H

#include <glib.h>
#include "../../include/gestores/gestor_sistemas.h"
#include "../../include/parsing/rowreader.h"
#include "../../include/gestores/gestor_musicas.h"

typedef struct Musica Musica;
typedef struct GestorMusicas GestorMusicas;

Musica *create_musica(int id, char *title, char **artist_ids, char *duration, char *genre, int year, char *lyrics);
char **parse_liked_musics(RowReader *reader);
Musica *inicializar_musica();
int parse_musica_and_add_him(RowReader *reader, GestorMusicas *gestorMusic);
gint *get_music_id(Musica *musica);

#endif // MUSICA_H
