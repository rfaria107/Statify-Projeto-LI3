#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/entidades/artists.h"

struct Artista
{
    gchar *id;                 // Identificador único do artista (inteiro)
    gchar *name;               // Nome do artista (string dinâmica)
    gchar *description;        // Descrição do artista (string dinâmica)
    gdouble recipe_per_stream; // Receita por stream (valor em double)
    gchar **id_constituent;    // Lista de IDs de membros (para grupos musicais), array dinâmico de strings
    gchar *country;            // País de origem (string dinâmica)
    gchar *type;               // Tipo de artista: "individual" ou "grupo musical" (string dinâmica)
};

gchar *get_artist_id(Artista *artista) { return (g_strdup(artista->id)); }

gchar *get_artist_name(Artista *artista) { return (g_strdup(artista->name)); }

gchar *get_artist_description(Artista *artista) { return (g_strdup(artista->description)); }

gdouble get_artist_recipe_per_stream(Artista *artista) { return artista->recipe_per_stream; }

gchar **get_artist_id_constituent(Artista *artista)
{
    if (artista->id_constituent == NULL)
        return NULL;

    int count = 0;
    while (artista->id_constituent[count] != NULL)
        count++;

    gchar **copy = g_malloc0((count + 1) * sizeof(gchar *));
    for (int i = 0; i < count; i++)
    {
        copy[i] = g_strdup(artista->id_constituent[i]);
    }
    return copy;
}

gchar *get_artist_country(Artista *artista) { return g_strdup(artista->country); }

gchar *get_artist_type(Artista *artista) { return g_strdup(artista->type); }

Artista *inicializar_artista()
{
    Artista *artista = malloc(sizeof(Artista)); // Aloca memória para o usuário
    if (!artista)
    {
        return NULL; // Retorna NULL se a alocação falhar
    }

    // Inicializa os campos do usuário
    artista->id = NULL;
    artista->name = NULL;
    artista->description = NULL;
    artista->recipe_per_stream = 0.0;
    artista->id_constituent = NULL;
    artista->country = NULL;
    artista->type = NULL;

    return artista; // Retorna o ponteiro para o usuário inicializado
}

void free_artista(Artista *artista)
{
    if (!artista)
        return;

    g_free(artista->id);
    g_free(artista->name);
    g_free(artista->description);
    // recipe per stream é um double
    if (artista->id_constituent)
    {
        for (gint i = 0; artista->id_constituent[i] != NULL; i++)
        {
            g_free(artista->id_constituent[i]);
        }
        g_free(artista->id_constituent); // Free array
    }
    g_free(artista->country);
    g_free(artista->type);
    g_free(artista);
}

Artista *create_artista(gchar *id, gchar *name, gchar *description, gdouble recipe_per_stream, gchar **id_constituent, gchar *country, gchar *type)
{
    Artista *artista = inicializar_artista(); // Aloca e inicializa o artista
    if (!artista)
    {
        return NULL; // Retorna NULL se a inicialização falhar
    }

    // Define os atributos do artista, copiando as strings dinamicamente
    artista->id = g_strdup(id);
    artista->name = g_strdup(name);
    artista->description = g_strdup(description);
    artista->recipe_per_stream = recipe_per_stream;
    artista->country = g_strdup(country);
    artista->type = g_strdup(type);

    // Inicializa a lista de IDs de membros, se fornecida
    if (id_constituent)
    {
        int count = 0;
        while (id_constituent[count] != NULL)
        {
            count++;
        }

        artista->id_constituent = g_malloc((count + 1) * sizeof(gchar *));
        for (int i = 0; i < count; i++)
        {
            artista->id_constituent[i] = g_strdup(id_constituent[i]);
        }
        artista->id_constituent[count] = NULL; // Finaliza a lista com NULL
    }
    else
    {
        artista->id_constituent = NULL; // Caso não haja membros, define como NULL
    }

    return artista; // Retorna o artista criado
}
