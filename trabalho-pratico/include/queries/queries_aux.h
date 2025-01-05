#ifndef QUERIES_AUX_H
#define QUERIES_AUX_H

#include <glib.h>
#include <stdio.h>
#include "../include/gestores/gestor_sistemas.h"
#include "../include/write/writer.h"

// Estrutura para representar a popularidade de um gênero
typedef struct UserData UserData;
typedef struct GenrePopularity GenrePopularity;
typedef struct Semana Semana;
typedef struct ArtistPopularity ArtistPopularity;
typedef struct ResultadoProcessamento ResultadoProcessamento;
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
void calcula_streams(GestorSistema *gestorsis);
double calcular_receita_total_artista(Artista *artista, GestorArtistas *gestorartistas, GestorMusicas *gestormusicas);
int get_num_constituents(Artista *artista);

gchar *find_top_entry_with_tiebreaker_str(GHashTable *table, gboolean alphabetical);
gint find_top_entry_with_tiebreaker(GHashTable *table, gboolean alphabetical);
GList* sort_hash_table_by_value_with_tiebreaker(GHashTable *table, gboolean ascending, gboolean tie_breaker, GHashTable *artist_time);
gint compare_artist_values(gconstpointer a, gconstpointer b, gpointer user_data);
UserData *create_user_data(GHashTable *table, gboolean reverse, gboolean alphabetical);
void free_user_data(UserData *user_data);
void process_top_artists(GHashTable *artist_time, GHashTable *artist_music_count, RowWriter *writer, int N);

void processar_historico (char* data_incial, char* data_final, GestorSistema *gestorsis, int line_number,int n);
void processar_historico_intervalo_de_datas (char* data_inicial, char* data_final, GestorSistema *gestor_sistema,int line_number,int n);
gint get_duracao_artista_na_semana(Semana *semana, const char *artist_id);
GList* get_top_artistas_na_semana(Semana *semana);
void adicionar_artista_na_semana(Semana *semana, const char *artist_id, int duracao);
void free_popularity(ArtistPopularity *popularity);
void liberar_popularidade(GList *popularity_list);
gint comparar_popularidade(struct ArtistPopularity *a, struct ArtistPopularity *b);
void atualizar_top_artistas_na_semana(Semana *semana);
gint comparar_datas(const char *data1, const char *data2);
time_t get_timestamp(const char *data);
gchar* calcular_domingo(const char *data);
gchar* calcular_sabado_seguinte(const gchar *data);
Semana* criar_semana(const char *domingo);
GHashTable *get_semanas_artistas(Semana *semana);
void destruir_semana(Semana *semana);
gint contar_aparicoes_artista(Semana *semana, const gchar *artist_id);
gchar* find_top_day_with_tiebreaker(GHashTable *day_count);


void calcular_domingos_inicial_e_final(char *data_inicial, char *data_final, char **domingo_inicial, char **domingo_final);
void calcular_artista_mais_apareceu(GestorSistema *gestor_sistema, char *data_inicial, char *data_final, int line_number);
ResultadoProcessamento* processar_semanas_e_contar_artistas(GestorSistema *gestor_sistema);
void intervalos_historico(GestorSistema *gestorsis, int line_number, int n, char *data_inicial, char *data_final, ResultadoProcessamento *resultado);
int parse_data(const char *data_str, struct tm *tm);
void all_historico(GestorSistema *gestorsis, int line_number, int n, ResultadoProcessamento *resultado);
int calcular_domingo_anterior(const char *data_str, struct tm *domingo_tm);
void destruir_resultado_processamento(ResultadoProcessamento *resultado);

#endif
