#ifndef QUERIES_AUX_H
#define QUERIEES_AUX_H

#include <glib.h>
#include <stdio.h>
#include "../include/gestores/gestor_sistemas.h"
#include "../include/write/writer.h"

// Estrutura para representar a popularidade de um gênero
typedef struct GenrePopularity GenrePopularity;
typedef struct Semana Semana ;

// Funções para gerenciamento de GenrePopularity
GenrePopularity *create_genre_popularity(const char *genre);
void free_genre_popularity(GenrePopularity *gp);
int compare_genre_popularity(const void *a, const void *b);

gint compara_duracoes_discografia(gconstpointer a, gconstpointer b);
void calcular_discografia_artistas(GestorSistema *gestorsis);

// Funções de conversão de durações
gint duracao_para_segundos(const gchar *duracao);
gchar *segundos_para_duracao(gint total_segundos);

#endif
