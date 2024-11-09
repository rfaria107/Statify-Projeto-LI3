#include "../../include/gestores/gestor_artistas.h"
#include "../../include/entidades/artists.h"
#include <glib.h>

struct GestorArtistas
{
    GHashTable *artistas; // Tabela hash de artistas
};

void free_artista_value(gpointer value) {
    free_artista((Artista *) value); // Assuming liberar_artista frees the Artista struct
}

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
    gestor->artistas = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,free_artista_value);
}

void liberar_gestor_artistas(GestorArtistas *gestor)
{
    g_hash_table_destroy(gestor->artistas);
    free(gestor);
}

void inserir_artista(GestorArtistas *gestor, Artista *artista)
{
    gchar *id = get_artist_id(artista);
    g_hash_table_insert(gestor->artistas, id, artista);
}

Artista* buscar_artista(GestorArtistas *gestor, const gchar*id) {
    return (Artista*) g_hash_table_lookup(gestor->artistas, id);
}