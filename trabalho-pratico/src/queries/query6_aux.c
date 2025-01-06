#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "../include/queries/query6_aux.h"

struct UserData
{
    GHashTable *hash;
    gboolean reverse;
    gboolean alphabetical;
};

UserData *create_user_data(GHashTable *table, gboolean reverse, gboolean alphabetical)
{
    UserData *user_data = malloc(sizeof(UserData));
    if (user_data == NULL)
    {
        return NULL; // Se a alocação falhar, retornamos NULL
    }
    user_data->hash = table;
    user_data->reverse = reverse;
    user_data->alphabetical = alphabetical;
    return user_data;
}

void free_user_data(UserData *user_data)
{
    if (user_data != NULL)
    {
        free(user_data);
    }
}

gint find_top_entry_with_tiebreaker(GHashTable *table, gboolean alphabetical)
{
    GHashTableIter iter;
    gpointer key, value;
    gint top_key = -1;
    gint64 top_value = -1;

    g_hash_table_iter_init(&iter, table);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {

        gint current_key = GPOINTER_TO_INT(key);
        gint64 current_value = GPOINTER_TO_INT(value);

        if (current_value > top_value ||
            (current_value == top_value && alphabetical && current_key < top_key))
        {
            top_key = current_key;
            top_value = current_value;
        }
    }
    return top_key;
}

gchar *find_top_entry_with_tiebreaker_str(GHashTable *table, gboolean alphabetical)
{
    GHashTableIter iter;
    gpointer key, value;
    gchar *top_key = NULL;
    gint64 top_value = -1;

    g_hash_table_iter_init(&iter, table);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        gchar *current_key = (gchar *)key;
        gint64 current_value = GPOINTER_TO_INT(value);

        if (current_value > top_value ||
            (current_value == top_value && alphabetical && g_strcmp0(current_key, top_key) < 0))
        {
            if (top_key)
            {
                g_free(top_key); // Free the previously allocated top_key
            }
            top_key = g_strdup(current_key); // Duplicate the new top_key
            top_value = current_value;
        }
    }
    return top_key;
}

gchar *find_top_day_with_tiebreaker(GHashTable *day_count)
{
    GHashTableIter iter;
    gpointer key, value;
    gchar *top_day = NULL;
    int max_count = 0;

    g_hash_table_iter_init(&iter, day_count);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        gchar *current_day = (gchar *)key;
        int current_count = GPOINTER_TO_INT(value);

        if (current_count > max_count)
        {
            // Novo top day encontrado
            top_day = g_strdup(current_day);
            max_count = current_count;
        }
        else if (current_count == max_count)
        {
            // Empate: escolher o mais recente (alfabeticamente maior)
            if (g_strcmp0(current_day, top_day) > 0)
            {
                g_free(top_day);
                top_day = g_strdup(current_day);
            }
        }
    }
    return top_day;
}

GList *sort_hash_table_by_value_with_tiebreaker(GHashTable *table, gboolean ascending, gboolean tie_breaker, GHashTable *artist_time)
{
    GList *sorted_list = NULL;

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, table);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        sorted_list = g_list_append(sorted_list, GINT_TO_POINTER(GPOINTER_TO_INT(key)));
    }

    // Ordena a lista com a função de comparação que usa a tabela artist_time como dados adicionais
    sorted_list = g_list_sort_with_data(sorted_list, (GCompareDataFunc)compare_artist_values, artist_time);

    return sorted_list;
}

gint compare_artist_values(gconstpointer a, gconstpointer b, gpointer user_data)
{
    gint artist_id_a = GPOINTER_TO_INT(a);
    gint artist_id_b = GPOINTER_TO_INT(b);

    // Pega os dados da tabela artist_time (tempo)
    GHashTable *artist_time = (GHashTable *)user_data;
    gint time_a = GPOINTER_TO_INT(g_hash_table_lookup(artist_time, GINT_TO_POINTER(artist_id_a)));
    gint time_b = GPOINTER_TO_INT(g_hash_table_lookup(artist_time, GINT_TO_POINTER(artist_id_b)));

    if (time_a == time_b)
    {
        // Se o tempo for o mesmo, compara os IDs de artistas (alfabeticamente).
        return artist_id_a - artist_id_b;
    }

    // Caso contrário, ordena pelo tempo (do maior para o menor).
    return time_b - time_a;
}