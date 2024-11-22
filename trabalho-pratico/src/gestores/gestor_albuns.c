#include "../../include/gestores/gestor_albuns.h"
#include "../../include/entidades/albuns.h"
#include <glib.h>

struct GestorAlbuns
{
    GHashTable *albuns; // Tabela hash de albuns
};

void free_album_value(gpointer value) {
    free_album((Album *) value); // Assuming liberar_album frees the album struct
}

GHashTable *get_hash_albuns(GestorAlbuns *gestor){
    return (gestor->albuns);
}

GestorAlbuns* criar_gestor_albuns() {
    GestorAlbuns *gestor = malloc(sizeof(GestorAlbuns));
    if (gestor) {
        inicializar_gestor_albuns(gestor);
    }
    return gestor;
}

void inicializar_gestor_albuns(GestorAlbuns *gestor)
{
    gestor->albuns = g_hash_table_new_full(g_str_hash, g_str_equal,g_free,free_album_value);
}

void liberar_gestor_albuns(GestorAlbuns *gestor)
{
    g_hash_table_destroy(gestor->albuns);
    free(gestor);
}

void inserir_album(GestorAlbuns *gestor, Album *album)
{
    gchar *id = get_album_id(album);
    g_hash_table_insert(gestor->albuns, id, album);
}

Album* buscar_album(GestorAlbuns *gestor, const gchar*id) {
    return (Album*) g_hash_table_lookup(gestor->albuns, id);
}