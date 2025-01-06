#ifndef QUERY4_AUX_H
#define QUERY4_AUX_H

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/gestores/gestor_sistemas.h"

typedef struct Semana Semana;
typedef struct ArtistPopularity ArtistPopularity;
typedef struct ResultadoProcessamento ResultadoProcessamento;

void all_historico(GestorSistema *gestorsis, int line_number, int n, ResultadoProcessamento *resultado, int l);
void intervalos_historico(GestorSistema *gestorsis, int line_number, int n, char *data_inicial, char *data_final, ResultadoProcessamento *resultado, int l);
ResultadoProcessamento *processar_semanas_e_contar_artistas(GestorSistema *gestor_sistema);
void calcular_domingos_inicial_e_final(char *data_inicial, char *data_final, char **domingo_inicial, char **domingo_final);
int parse_data(const char *data_str, struct tm *tm);
int calcular_domingo_anterior(const char *data_str, struct tm *domingo_tm);
void destruir_resultado_processamento(ResultadoProcessamento *resultado);
time_t get_timestamp(const char *data);
gchar *calcular_domingo(const char *data);
gchar *calcular_sabado_seguinte(const gchar *data);
Semana *criar_semana(const char *domingo);
GHashTable *get_semanas_artistas(Semana *semana);
void destruir_semana(Semana *semana);
gint contar_aparicoes_artista(Semana *semana, int artist_id);
gint comparar_datas(const char *data1, const char *data2);
void atualizar_top_artistas_na_semana(Semana *semana);
gint get_duracao_artista_na_semana(Semana *semana, int artist_id);
gint comparar_popularidade(struct ArtistPopularity *a, struct ArtistPopularity *b);
GList *get_top_artistas_na_semana(Semana *semana);
void adicionar_artista_na_semana(Semana *semana, int artist_id, int duracao);
void free_popularity(ArtistPopularity *popularity);
void liberar_popularidade(GList *popularity_list);

// void process_top_artists(GHashTable *artist_time, GHashTable *artist_music_count, RowWriter *writer, int N);
// void processar_historico (char* data_incial, char* data_final, GestorSistema *gestorsis, int line_number,int n);
// void processar_historico_intervalo_de_datas (char* data_inicial, char* data_final, GestorSistema *gestor_sistema,int line_number,int n);
// void calcular_artista_mais_apareceu(GestorSistema *gestor_sistema, char *data_inicial, char *data_final, int line_number);

#endif