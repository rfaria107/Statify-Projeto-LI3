#include "../../include/gestores/gestor_artistas.h"
#include "../../include/entidades/artists.h"
#include <glib.h>

struct GestorArtistas
{
    GHashTable *artistas; // Tabela hash de artistas
};

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

void inicializar_gestor_artistas(GestorArtistas *gestor)
{
    gestor->artistas = g_hash_table_new(g_str_hash, g_str_equal);
}

void liberar_gestor_artistas(GestorArtistas *gestor)
{
    g_hash_table_destroy(gestor->artistas);
}

void inserir_artista(GestorArtistas *gestor, Artista *artista)
{
    g_hash_table_insert(gestor->artistas, g_strdup(artista->id), artista);
}
