#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/entidades/musica.h"

struct Musica
{
    gint id;            // ID único da música
    gchar *title;       // Título da música (string dinâmica)
    gchar **artist_ids; // Lista de IDs dos artistas (array dinâmico de strings)
    gchar *album_id;    // identificador do album a que a música pertence
    gchar *duration;    // Duração da música em segundos
    gchar *genre;       // Gênero (string dinâmica)
    guint16 year;       // Ano de lançamento
    guint32 streams;
};

// Função para criar músicas

int get_music_id(Musica *musica) { return musica->id; }

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

gchar *get_music_album(Musica *musica) { return g_strdup(musica->album_id); }

gchar *get_music_duration(Musica *musica) { return g_strdup(musica->duration); }

gchar *get_music_genre(Musica *musica) { return g_strdup(musica->genre); }

gint get_music_year(Musica *musica) { return musica->year; }

gint get_music_streams(Musica *musica) { return musica->streams; }

void set_music_streams(Musica *musica, int stream_count) { musica->streams = stream_count; }

Musica *create_musica(int id, char *title, char **artist_ids, gchar *album_id, char *duration, char *genre, int year, int streams)
{
    Musica *musica = inicializar_musica();

    // Define os atributos da musica;
    musica->id = id;
    musica->year = year;
    musica->title = g_strdup(title);
    musica->album_id = g_strdup(album_id);
    musica->duration = g_strdup(duration);
    musica->genre = g_strdup(genre);
    musica->streams = 0;
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

    musica->id = 0;
    musica->title = NULL;
    musica->artist_ids = NULL;
    musica->album_id = NULL;
    musica->duration = NULL;
    musica->genre = NULL;
    musica->year = 0;
    musica->streams = 0;

    return musica;
}

void free_musica(Musica *musica)
{
    if (musica)
    {
        // Libera os campos dinâmicos da estrutura
        // g_free(musica->id);
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
        g_free(musica->album_id);
        g_free(musica->duration);
        g_free(musica->genre);

        // Finalmente, libera a estrutura Musica
        g_free(musica);
    }
}