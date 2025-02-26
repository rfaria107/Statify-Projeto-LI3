#include "../../include/gestores/gestor_histories.h"
#include "../../include/entidades/history.h"
#include <glib.h>

struct GestorHistories
{
    GHashTable *histories; // Tabela hash de histories
};

void free_history_value(gpointer value)
{
    free_history((History *)value); // Assuming liberar_history frees the history struct
}

GHashTable *get_hash_histories(GestorHistories *gestor)
{
    return (gestor->histories);
}

GestorHistories *criar_gestor_histories()
{
    GestorHistories *gestor = malloc(sizeof(GestorHistories));
    if (gestor)
    {
        inicializar_gestor_histories(gestor);
    }
    return gestor;
}

void inicializar_gestor_histories(GestorHistories *gestor)
{
    gestor->histories = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, free_history_value);
}

void liberar_gestor_histories(GestorHistories *gestor)
{
    g_hash_table_destroy(gestor->histories);
    free(gestor);
}

void inserir_history(GestorHistories *gestor, History *history)
{
    int id = get_history_id(history);
    g_hash_table_insert(gestor->histories, GINT_TO_POINTER(id), history);
}

History *buscar_history(GestorHistories *gestor, int id)
{
    return (History *)g_hash_table_lookup(gestor->histories, GINT_TO_POINTER(id));
}

History *buscar_history_por_user_id(GestorHistories *gestor, int user_id)
{

    if (gestor == NULL || gestor->histories == NULL)
    {
        return NULL;
    }

    History *history = (History *)g_hash_table_lookup(gestor->histories, GINT_TO_POINTER(user_id));

    return history;
}
