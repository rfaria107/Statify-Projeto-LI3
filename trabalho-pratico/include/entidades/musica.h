#ifndef MUSICA_H
#define MUSICA_H

#include <glib.h>
#include "../../include/gestores/gestor_sistemas.h"

#define MAX_NAME_LENGTH 50
#define MAX_TITLE_LENGTH 100
#define MAX_GENRE_LENGTH 30
#define MAX_LYRICS_LENGTH 1000

typedef struct Musica Musica;

char **parse_liked_musics (RowReader* reader);
Musica* inicializar_musica();


#endif // MUSICA_H
