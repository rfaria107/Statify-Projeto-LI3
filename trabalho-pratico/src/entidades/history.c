#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "../../include/entidades/history.h"

struct History
{
    gchar *id;            // ID único do registo;
    gchar *user_id;       // ID único do utilizador a que o registo se refere;
    gchar *music_id;      // ID único da musica a que o registo se refere;
    gchar *timestamp;     // Data e hora em que a música foi ouvida pelo utilizador;
    gchar *duration;      // Tempo de duração da audição da música;
    gchar *platform;      // Plataforma em que a música foi reproduzida;
};

// Função para criar músicas

gchar *get_history_id(History *history) {return g_strdup(history->id);}

gchar *get_history_user_id(History *history) {return g_strdup(history->user_id);}

gchar *get_history_music_id(History *history) {return g_strdup(history->music_id);}

gchar *get_history_timestamp(History *history) {return g_strdup(history->timestamp);}

gchar *get_history_date(History *history) {
    if (history == NULL || history->timestamp == NULL) {
        return NULL; // Verificar se o histórico ou timestamp são válidos
    }
    
    gchar *date = g_malloc(11);
    strncpy(date, history->timestamp, 10);
    date[10] = '\0';
    return date; // Retornar a data como string dinâmica
}


gchar *get_history_duration(History *history) {return g_strdup(history->duration);}

gchar *get_history_platform(History *history) {return g_strdup(history->platform);}

History *create_history(char *id, char *user_id, char *music_id, char *timestamp, char *duration, char *platform)
{
    History *history = inicializar_history();

    // Define os atributos da estrutura
    history->id = g_strdup(id);
    history->user_id = g_strdup(user_id);
    history->music_id = g_strdup(music_id);
    history->timestamp = g_strdup(timestamp);
    history->duration = g_strdup(duration);
    history->platform = g_strdup(platform);

    return history;
}

History *inicializar_history()
{
    History *history = malloc(sizeof(History));
    if (!history)
    {
        return NULL;
    }

    history->id = NULL;
    history->user_id = NULL;
    history->music_id = NULL;
    history->timestamp = NULL;
    history->duration = NULL;
    history->platform = NULL;

    return history;
}

void free_history(History *history)
{
    if (history)
    {
        // Libera os campos dinâmicos
        g_free(history->id);
        g_free(history->user_id);
        g_free(history->music_id);
        g_free(history->timestamp);
        g_free(history->duration);
        g_free(history->platform);

        // Finalmente, libera a estrutura
        g_free(history);
    }
}
