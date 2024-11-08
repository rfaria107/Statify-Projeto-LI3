#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/entidades/musica.h"

struct Musica
{
    gchar *id;          // ID único da música
    gchar *title;       // Título da música (string dinâmica)
    gchar **artist_ids; // Lista de IDs dos artistas (array dinâmico de strings)
    gchar *duration;    // Duração da música em segundos
    gchar *genre;       // Gênero (string dinâmica)
    gint year;          // Ano de lançamento
    gchar *lyrics;      // Letra da música (string dinâmica)
};

// Função para criar músicas

gchar *get_music_id(Musica *musica) { return g_strdup(musica->id); }

gchar *get_music_title(Musica *musica) { return g_strdup(musica->title); }

gchar **get_music_artist_ids(Musica *musica)
{
    if (!musica->artist_ids)
        return NULL;

    int count = 0;
    while (musica->artist_ids[count] != NULL)
    {
        count++;
    }

    gchar **artist_ids_copy = malloc((count + 1) * sizeof(gchar *));
    for (int i = 0; i < count; i++)
    {
        artist_ids_copy[i] = g_strdup(musica->artist_ids[i]);
    }
    artist_ids_copy[count] = NULL;

    return artist_ids_copy;
}

gchar *get_music_duration(Musica *musica) { return g_strdup(musica->duration); }

gchar *get_music_genre(Musica *musica) { return g_strdup(musica->genre); }

gint get_music_year(Musica *musica) { return musica->year; }

gchar *get_music_lyrics(Musica *musica) { return g_strdup(musica->lyrics); }

Musica *create_musica(char *id, char *title, char **artist_ids,
                      char *duration, char *genre, int year,
                      char *lyrics)
{
    Musica *musica = inicializar_musica();

    // Define os atributos da musica;
    musica->id = g_strdup(id);
    musica->year = year;
    musica->title = g_strdup(title);
    musica->duration = g_strdup(duration);
    musica->genre = g_strdup(genre);
    musica->lyrics = g_strdup(lyrics);

    if (artist_ids)
    {
        int count = 0;
        while (artist_ids[count] != NULL)
        {
            count++; // Conta quantos IDs de artista existem
        }

        musica->artist_ids = g_new(gchar *, count + 1); // Aloca espaço para o array
        for (int i = 0; i < count; i++)
        {
            musica->artist_ids[i] = g_strdup(artist_ids[i]); // Duplica cada string
        }
        musica->artist_ids[count] = NULL; // Termina o array com NULL
    }
    else
    {
        musica->artist_ids = NULL; // Caso artist_ids seja NULL
    }

    return musica;
}

Musica *inicializar_musica()
{
    Musica *musica = malloc(sizeof(Musica));
    if (!musica)
    {
        return NULL;
    }

    musica->id = NULL;
    musica->title = NULL;
    musica->artist_ids = NULL;
    ;
    musica->duration = NULL;
    musica->genre = NULL;
    musica->year = 0;
    musica->lyrics = NULL;

    return musica;
}

void free_musica(Musica *musica)
{
    if (musica)
    {
        // Libera os campos dinâmicos da estrutura
        g_free(musica->id);
        g_free(musica->title);

        // Libera cada string no array artist_ids
        if (musica->artist_ids)
        {
            for (int i = 0; musica->artist_ids[i] != NULL; i++)
            {
                g_free(musica->artist_ids[i]); // Libera cada ID do artista
            }
            g_free(musica->artist_ids); // Libera o array de IDs
        }

        g_free(musica->duration);
        g_free(musica->genre);
        g_free(musica->lyrics);

        // Finalmente, libera a estrutura Musica
        g_free(musica);
    }
}