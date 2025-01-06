#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/entidades/albuns.h"

struct Album
{
    gint id;            // Identificador único do album (inteiro)
    gchar *title;       // Nome do album (string dinâmica)
    gchar **artist_ids; // Lista de IDs dos artistas (array dinâmico de strings)
    gint year;          // Ano de lançamento
};

gint get_album_id(Album *album) { return (album->id); }

gchar *get_album_title(Album *album) { return (g_strdup(album->title)); }

gint get_album_year(Album *album) { return album->year; }

gchar **get_album_artist_ids(Album *album)
{
    if (!album->artist_ids)
        return NULL;

    int count = 0;
    while (album->artist_ids[count] != NULL)
    {
        count++;
    }

    gchar **artist_ids_copy = malloc((count + 1) * sizeof(gchar *));
    for (int i = 0; i < count; i++)
    {
        artist_ids_copy[i] = g_strdup(album->artist_ids[i]);
    }
    artist_ids_copy[count] = NULL;

    return artist_ids_copy;
}

Album *create_album(int id, char *title, char **artist_ids, int year)
{
    Album *album = inicializar_album();

    // Define os atributos da album;
    album->id = id;
    album->year = year;
    album->title = g_strdup(title);

    if (artist_ids)
    {
        int count = 0;
        while (artist_ids[count] != NULL)
        {
            count++; // Conta quantos IDs de artista existem
        }

        album->artist_ids = g_new(gchar *, count + 1); // Aloca espaço para o array
        for (int i = 0; i < count; i++)
        {
            album->artist_ids[i] = g_strdup(artist_ids[i]); // Duplica cada string
        }
        album->artist_ids[count] = NULL; // Termina o array com NULL
    }
    else
    {
        album->artist_ids = NULL; // Caso artist_ids seja NULL
    }

    return album;
}

Album *inicializar_album()
{
    Album *album = malloc(sizeof(Album));
    if (!album)
    {
        return NULL;
    }

    album->id = 0;
    album->title = NULL;
    album->artist_ids = NULL;
    album->year = 0;

    return album;
}

void free_album(Album *album)
{
    if (album)
    {
        // g_free(album->id);
        g_free(album->title);

        if (album->artist_ids)
        {
            for (int i = 0; album->artist_ids[i] != NULL; i++)
            {
                g_free(album->artist_ids[i]); // Libera cada ID do artista
            }
            g_free(album->artist_ids); // Libera o array de IDs
        }

        // Finalmente, libera a estrutura album
        g_free(album);
    }
}