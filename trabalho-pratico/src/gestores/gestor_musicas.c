#include "../../include/gestores/gestor_musicas.h"

typedef struct GestorMusicas{
    GHashTable *musicas; // Tabela hash de músicas
} GestorMusicas;

void inicializar_gestor_musicas(GestorMusicas *gestor) {
    gestor->musicas = g_hash_table_new(g_str_hash, g_str_equal);
}

void liberar_gestor_musicas(GestorMusicas *gestor) {
    g_hash_table_destroy(gestor->musicas);
}
