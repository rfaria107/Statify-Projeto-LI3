#include "../../include/gestores/gestor_musicas.h"
#include <glib.h>

struct GestorMusicas
{
    GHashTable *musicas; // Tabela hash de músicas
};

GestorMusicas* criar_gestor_musicas() {
    GestorMusicas *gestor = malloc(sizeof(GestorMusicas));
    if (gestor) {
        inicializar_gestor_musicas(gestor);
    }
    return gestor;
}

void inicializar_gestor_musicas(GestorMusicas *gestor)
{
    gestor->musicas = g_hash_table_new(g_str_hash, g_str_equal);
}

void liberar_gestor_musicas(GestorMusicas *gestor)
{
    g_hash_table_destroy(gestor->musicas);
}

void inserir_musica(GestorMusicas *gestor, Musica *musica)
{
    g_hash_table_insert(gestor->musicas, get_music_id(musica), musica);
}


GHashTable *get_hash_musicas(GestorMusicas *gestor){
    return (gestor->musicas);
}

// Função para buscar uma música pelo ID único de música
Musica* buscar_musicas(GestorMusicas *gestor, const gint id) {
    return (Musica*) g_hash_table_lookup(gestor->musicas, GINT_TO_POINTER(id));
}