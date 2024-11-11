#ifndef queryS_H
#define queryS_H

#include <glib.h>
#include <stdio.h>
#include "../include/gestores/gestor_sistemas.h"
#include "../include/parsing/writer.h"

// Estrutura para representar a popularidade de um gênero
typedef struct GenrePopularity GenrePopularity;

// Funções para gerenciamento de GenrePopularity
GenrePopularity *create_genre_popularity(const char *genre);
void free_genre_popularity(GenrePopularity *gp);
int compare_genre_popularity(const void *a, const void *b);

gint compara_duracoes_discografia(gconstpointer a, gconstpointer b);
void calcular_discografia_artistas(GestorSistema *gestorsis);

// Funções de conversão de durações
gint duracao_para_segundos(const gchar *duracao);
gchar *segundos_para_duracao(gint total_segundos);

// Funções de query
void query_1(GestorUsuarios *gestor, char *username, int line_number);
void query_2(GestorSistema *gestorsis, int num, gchar *country, int line_number);
void query_3(int min_age, int max_age, GestorSistema *gestor_sistema, int line_number);

// Função principal para interpretar inputs e executar querys
void interpreter_inputs(FILE *file, GestorSistema *gestorsis);

#endif // queryS_H
