#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/entidades/artists.h"

struct Artista
{
    gint id;                   // Identificador único do artista (inteiro)
    gchar *name;               // Nome do artista (string dinâmica)
    gdouble recipe_per_stream; // Receita por stream (valor em double)
    gchar **id_constituent;    // Lista de IDs de membros (para grupos musicais), array dinâmico de strings
    gchar *country;            // País de origem (string dinâmica)
    gchar *type;               // Tipo de artista: "individual" ou "grupo musical" (string dinâmica)
    guint duracao_discografia; // Duração da discografia em segundos
};

int get_artist_id(Artista *artista) { return artista->id; }

gchar *get_artist_name(Artista *artista) { return (g_strdup(artista->name)); }

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

// Função para contar o número de constituintes (membros) de um artista/grupo
int get_num_constituents(Artista *artista)
{
    gchar **constituents = get_artist_id_constituent(artista);
    int num_constituents = 0;

    // Conta o número de membros
    if (constituents)
    {
        while (constituents[num_constituents] != NULL)
        {
            num_constituents++;
        }
        g_strfreev(constituents); // Libera memória associada aos IDs
    }

    return num_constituents;
}

gchar *get_artist_country(Artista *artista)
{
    return g_strdup(artista->country);
}

gchar *get_artist_type(Artista *artista)
{
    return g_strdup(artista->type);
}

gint get_artist_duracao_disco(Artista *artista)
{
    return (artista->duracao_discografia);
}

void set_artista_duracao_disco(Artista *artista, gint nova_duracao)
{
    artista->duracao_discografia = nova_duracao;
}

Artista *inicializar_artista()
{
    Artista *artista = malloc(sizeof(Artista)); // Aloca memória para o usuário
    if (!artista)
    {
        return NULL; // Retorna NULL se a alocação falhar
    }

    // Inicializa os campos do artista
    artista->id = 0;
    artista->name = NULL;
    artista->recipe_per_stream = 0.0;
    artista->id_constituent = NULL;
    artista->country = NULL;
    artista->type = NULL;
    artista->duracao_discografia = 0;
    return artista;
}

void free_artista(Artista *artista)
{
    if (!artista)
        return;

    // g_free(artista->id);
    g_free(artista->name);
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

Artista *create_artista(int id, gchar *name, gdouble recipe_per_stream, gchar **id_constituent, gchar *country, gchar *type)
{
    Artista *artista = inicializar_artista(); // Aloca e inicializa o artista
    if (!artista)
    {
        return NULL; // Retorna NULL se a inicialização falhar
    }

    // Define os atributos do artista, copiando as strings dinamicamente
    artista->id = id;
    artista->name = g_strdup(name);
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
