#ifndef QUERIES_H
#define QUERIES_H

#include <glib.h>
#include <stdio.h>
#include "../include/gestores/gestor_sistemas.h"
#include "../include/parsing/writer.h"

// Estrutura para representar a popularidade de um gênero
typedef struct {
    char *genre;
    int total_likes;
} GenrePopularity;

// Funções para gerenciamento de GenrePopularity
GenrePopularity *create_genre_popularity(const char *genre);
void free_genre_popularity(GenrePopularity *gp);
int compare_genre_popularity(const void *a, const void *b);

// Funções de conversão de durações
gint duracao_para_segundos(const gchar *duracao);
gchar *segundos_para_duracao(gint total_segundos);

// Funções de query
void querie_1(GestorUsuarios *gestor, char *username, int line_number);
void querie_3(int min_age, int max_age, GestorSistema *gestor_sistema, int line_number);

// Função principal para interpretar inputs e executar queries
void interpreter_inputs(FILE *file, GestorSistema *gestorsis);

// Função para calcular a duração total da discografia de um artista
gchar *calcular_discografia(GestorArtistas *gestorartistas, const Artista *artista);

#endif // QUERIES_H
