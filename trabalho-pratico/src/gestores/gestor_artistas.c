#include "../../include/gestores/gestor_artistas.h"
#include "../../include/entidades/artists.h"
#include <glib.h>

struct GestorArtistas
{
    GHashTable *artistas; // Tabela hash de artistas
};

GHashTable *get_hash_artistas(GestorArtistas *gestor){
    return (gestor->artistas);
}

GestorArtistas* criar_gestor_artistas() {
    GestorArtistas *gestor = malloc(sizeof(GestorArtistas));
    if (gestor) {
        inicializar_gestor_artistas(gestor);
    }
    return gestor;
}

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
    g_hash_table_insert(gestor->artistas, g_strdup(get_artist_id(artista)), artista);
}
