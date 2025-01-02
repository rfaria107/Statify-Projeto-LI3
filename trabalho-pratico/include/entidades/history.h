#ifndef HISTORY_H
#define HISTORY_H

#include <glib.h>

typedef struct History History;

// Funções de acesso aos atributos do usuário
int get_history_id(History *history);
int get_history_user_id(History *history);
int get_history_music_id(History *history);
gchar *get_history_timestamp(History *history);
gchar *get_history_duration(History *history);
gchar *get_history_platform(History *history);
gchar *get_history_date (History *history);
// Funções para manipulação de histories
History *create_history(int id, int user_id, int music_id, char *timestamp, char *duration, char *platform);
History *inicializar_history();
void free_history(History *history);

#endif // HISTORY_H