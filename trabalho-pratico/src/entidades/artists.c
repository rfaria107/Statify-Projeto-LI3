#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../../include/entidades/artists.h"
#include "../../include/gestores/gestor_artistas.h"
#include "../../include/gestores/gestor_musicas.h"
#include "../../include/entidades/musica.h"

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

gboolean valida_artista_individual(const Artista *artista)
{
    // Verifica se o artista é nulo
    if (artista == NULL)
    {
        return FALSE;
    }

    // Verifica se o tipo do artista é "individual"
    if (g_strcmp0(artista->type, "individual") == 0)
    {
        // Verifica se o id_constituent é nulo, já que um artista individual não deve ter constituintes
        if (artista->id_constituent != NULL)
        {
            return FALSE; // Artista individual não pode ter constituinte
        }
    }

    // Valida se campos obrigatórios estão preenchidos
    if (artista->id == NULL || artista->name == NULL ||
        artista->recipe_per_stream <= 0 || artista->country == NULL ||
        artista->type == NULL)
    {
        return FALSE; // Algum campo obrigatório está faltando
    }

    return TRUE; // Artista individual válido
}

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