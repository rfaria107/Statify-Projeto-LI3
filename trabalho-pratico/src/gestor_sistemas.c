#include "../Include/gestor_sistemas.h"

// Função para inicializar o sistema com as hash tables (Glib)
void inicializar_gestor(GestorSistema *gestor) {
    gestor->usuarios = g_hash_table_new(g_str_hash, g_str_equal);
    gestor->artistas = g_hash_table_new(g_str_hash, g_str_equal);
    gestor->musicas = g_hash_table_new(g_str_hash, g_str_equal);
}

// Função para liberar a memória utilizada pelo sistema (Glib)
void liberar_gestor(GestorSistema *gestor) {
    g_hash_table_destroy(gestor->usuarios);
    g_hash_table_destroy(gestor->artistas);
    g_hash_table_destroy(gestor->musicas);
}
