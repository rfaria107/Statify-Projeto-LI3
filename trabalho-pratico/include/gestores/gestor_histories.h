#ifndef GESTOR_HISTORIES_H
#define GESTOR_HISTORIES_H

#include <glib.h>
#include "../include/entidades/history.h"

typedef struct GestorHistories GestorHistories;

// Funções para inicialização e limpeza do gestor de histories
void free_history_value(gpointer value);
GHashTable *get_hash_histories(GestorHistories *gestor);
GestorHistories* criar_gestor_histories();
void inicializar_gestor_histories(GestorHistories *gestor);
void liberar_gestor_histories(GestorHistories *gestor);
void inserir_history(GestorHistories *gestor, History *history);
History* buscar_history(GestorHistories *gestor, const gchar *id);
History* buscar_history_por_user_id(GestorHistories *gestor, const gchar *user_id);

#endif
