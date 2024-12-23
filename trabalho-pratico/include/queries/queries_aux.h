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

// Funções para calcular o num_albuns e o recipe total
int get_artist_num_albuns_individual(Artista *artista, GestorAlbuns *gestor_albuns);
double calcular_receita_total_artista(Artista *artista,GHashTable *hash_musicas, GHashTable *hash_history, GHashTable *hash_artistas);
double calcular_receita_direta(Artista *artista, GHashTable *hash_musicas, GHashTable *hash_history);
double calcular_receita_participacao(Artista *artista, GHashTable *hash_musicas, GHashTable *hash_history, GHashTable *hash_artistas);
int is_artist_member_of_group(Artista *artista, Artista *grupo);
double contar_reproducao_receita(Artista *artista, Musica *musica, GHashTable *hash_history);
double contar_reproducao_receita_grupo(Artista *grupo, Musica *musica, GHashTable *hash_history);
int get_num_constituents(Artista *artista);
#endif
