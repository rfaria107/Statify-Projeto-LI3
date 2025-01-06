#ifndef QUERY6_AUX_H
#define QUERY6_AUX_H

#include <glib.h>

typedef struct UserData UserData;

UserData *create_user_data(GHashTable *table, gboolean reverse, gboolean alphabetical);
void free_user_data(UserData *user_data);

gchar* find_top_day_with_tiebreaker(GHashTable *day_count);
gchar *find_top_entry_with_tiebreaker_str(GHashTable *table, gboolean alphabetical);
gint find_top_entry_with_tiebreaker(GHashTable *table, gboolean alphabetical);
GList* sort_hash_table_by_value_with_tiebreaker(GHashTable *table, gboolean ascending, gboolean tie_breaker, GHashTable *artist_time);
gint compare_artist_values(gconstpointer a, gconstpointer b, gpointer user_data);

#endif