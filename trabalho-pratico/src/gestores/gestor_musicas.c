#include "../../include/gestores/gestor_musicas.h"
#include <glib.h>

struct GestorMusicas
{
    GHashTable *musicas; // Tabela hash de músicas
};

void free_music_value(gpointer value)
{
    free_musica((Musica *)value); // Assuming liberar_musica frees the Musica struct
}

GestorMusicas *criar_gestor_musicas()
{
    GestorMusicas *gestor = malloc(sizeof(GestorMusicas));
    if (gestor)
    {
        inicializar_gestor_musicas(gestor);
    }
    return gestor;
}

void inicializar_gestor_musicas(GestorMusicas *gestor)
{
    gestor->musicas = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, free_music_value);
}

void liberar_gestor_musicas(GestorMusicas *gestor)
{
    g_hash_table_destroy(gestor->musicas);
    free(gestor);
}

void inserir_musica(GestorMusicas *gestor, Musica *musica)
{
    int id = get_music_id(musica);
    g_hash_table_insert(gestor->musicas, GINT_TO_POINTER(id), musica);
}

GHashTable *get_hash_musicas(GestorMusicas *gestor)
{
    return (gestor->musicas);
}

// Função para buscar uma música pelo ID único de música
Musica *buscar_musicas(GestorMusicas *gestor, int id)
{
    return (Musica *)g_hash_table_lookup(gestor->musicas, GINT_TO_POINTER(id));
}