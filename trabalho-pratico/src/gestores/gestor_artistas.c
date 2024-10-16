#include "../include/gestores/gestor_artistas.h"

void inicializar_gestor_artistas(GestorArtistas *gestor) {
    gestor->artistas = g_hash_table_new(g_str_hash, g_str_equal);
}

void liberar_gestor_artistas(GestorArtistas *gestor) {
    g_hash_table_destroy(gestor->artistas);
}
