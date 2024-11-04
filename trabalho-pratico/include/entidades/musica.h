#ifndef MUSICA_H
#define MUSICA_H

#include <glib.h>
#include "../../include/gestores/gestor_sistemas.h"

#define MAX_NAME_LENGTH 50
#define MAX_TITLE_LENGTH 100
#define MAX_GENRE_LENGTH 30
#define MAX_LYRICS_LENGTH 1000

typedef struct {
    gint id;                // ID único da música
    gchar* title;           // Título da música (string dinâmica)
    gchar** artist_ids;     // Lista de IDs dos artistas (array dinâmico de strings)
    gchar* duration;        // Duração da música em segundos
    gchar* genre;           // Gênero (string dinâmica)
    gint year;              // Ano de lançamento
    gchar* lyrics;          // Letra da música (string dinâmica)
} Musica;

char **parse_liked_musics (RowReader* reader);

#endif // MUSICA_H
