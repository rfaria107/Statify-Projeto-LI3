/**
 * @file query4_aux.h
 * @brief Header das declarações de funções auxiliares para a query4.
 *
 * Este arquivo contém as declarações de funções utilizadas para o processamento de dados de popularidade de artistas
 * e para a manipulação de semanas, incluindo a contagem de artistas no topo e a geração de relatórios que guardam os registos de todas as semanas
 * para ser acedido pela query 4.
 *
 */

#ifndef QUERY4_AUX_H
#define QUERY4_AUX_H

#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/gestores/gestor_sistemas.h"

/**
 * @typedef Semana
 * @brief Estrutura que representa uma semana com os dados de artistas.
 */
typedef struct Semana Semana;

/**
 * @typedef ArtistPopularity
 * @brief Estrutura que representa a popularidade de um artista.
 */
typedef struct ArtistPopularity ArtistPopularity;

/**
 * @typedef ResultadoProcessamento
 * @brief Estrutura que contém os resultados do processamento das semanas e contagem dos artistas.
 */
typedef struct ResultadoProcessamento ResultadoProcessamento;

/**
 * @brief Processa o histórico de popularidade de artistas e escreve os resultados num arquivo.
 *
 * Esta função processa todos os dados históricos de popularidade de artistas e escreve o artista com mais aparições
 * no topo num arquivo de saída.
 *
 * @param gestorsis Gestor do sistema que contém os dados dos artistas.
 * @param line_number Número da linha do comando que gerou a consulta.
 * @param n Indica se o resultado deve ser escrito ou impresso.
 * @param resultado Resultado do processamento das semanas e contagem dos artistas.
 * @param l Parâmetro adicional usado para definir o tipo de escrita.
 */
void all_historico(GestorSistema *gestorsis, int line_number, int n, ResultadoProcessamento *resultado, int l);

/**
 * @brief Processa o histórico de popularidade de artistas num intervalo de datas e escreve os resultados num arquivo.
 *
 * Esta função processa os dados históricos de popularidade de artistas dentro de um intervalo de datas especificado
 * e escreve o artista com mais aparições no topo dentro desse intervalo num arquivo de saída.
 *
 * @param gestorsis Gestor do sistema que contém os dados dos artistas.
 * @param line_number Número da linha do comando que gerou a consulta.
 * @param n Indica se o resultado deve ser escrito ou impresso.
 * @param data_inicial Data inicial do intervalo no formato "YYYY/MM/DD".
 * @param data_final Data final do intervalo no formato "YYYY/MM/DD".
 * @param resultado Resultado do processamento das semanas e contagem dos artistas.
 * @param l Parâmetro adicional usado para definir o tipo de escrita.
 */
void intervalos_historico(GestorSistema *gestorsis, int line_number, int n, char *data_inicial, char *data_final, ResultadoProcessamento *resultado, int l);

/**
 * @brief Processa as semanas e conta as aparições dos artistas.
 *
 * Esta função processa as semanas e calcula a popularidade dos artistas, retornando os resultados numa estrutura
 * de processamento.
 *
 * @param gestor_sistema Gestor do sistema que contém os dados necessários para o processamento.
 * @return ResultadoProcessamento Estrutura com os resultados do processamento das semanas e contagem dos artistas.
 */
ResultadoProcessamento *processar_semanas_e_contar_artistas(GestorSistema *gestor_sistema);

/**
 * @brief Destrói a estrutura de ResultadoProcessamento e libera a memória alocada.
 *
 * @param resultado Ponteiro para a estrutura ResultadoProcessamento a ser destruída.
 */
void destruir_resultado_processamento(ResultadoProcessamento *resultado);

/**
 * @brief Cria uma nova semana com base na data do domingo.
 *
 * Esta função cria uma nova estrutura de semana a partir de uma data de domingo fornecida.
 *
 * @param domingo Data do domingo no formato "YYYY/MM/DD".
 * @return Ponteiro para a nova estrutura Semana criada.
 */
Semana *criar_semana(const char *domingo);

/**
 * @brief Destrói a estrutura Semana e libera a memória alocada.
 *
 * @param semana Ponteiro para a estrutura Semana a ser destruída.
 */
void destruir_semana(Semana *semana);

/**
 * @brief Atualiza a lista dos top artistas de uma semana.
 *
 * Esta função atualiza a lista dos artistas mais populares (top 10) para uma determinada semana.
 *
 * @param semana Ponteiro para a estrutura Semana onde os artistas devem ser atualizados.
 */
void atualizar_top_artistas_na_semana(Semana *semana);

/**
 * @brief Compara a popularidade de dois artistas.
 *
 * Esta função compara dois artistas com base em sua popularidade, retornando um valor negativo, zero ou positivo
 * dependendo da comparação.
 *
 * @param a Ponteiro para a estrutura ArtistPopularity do primeiro artista.
 * @param b Ponteiro para a estrutura ArtistPopularity do segundo artista.
 * @return Valor negativo, zero ou positivo dependendo da comparação da popularidade.
 */
gint comparar_popularidade(struct ArtistPopularity *a, struct ArtistPopularity *b);

/**
 * @brief Libera a memória alocada para a estrutura de popularidade de um artista.
 *
 * @param popularity Ponteiro para a estrutura ArtistPopularity a ser liberada.
 */
void free_popularity(ArtistPopularity *popularity);

/**
 * @brief Libera a memória alocada para a lista de popularidade de artistas.
 *
 * @param popularity_list Lista de popularidade dos artistas a ser liberada.
 */
void liberar_popularidade(GList *popularity_list);

/**
 * @brief Adiciona um artista a uma semana com a duração da sua popularidade.
 *
 * @param semana Ponteiro para a estrutura Semana onde o artista será adicionado.
 * @param artist_id ID do artista a ser adicionado.
 * @param duracao Duração da popularidade do artista durante a semana.
 */
void adicionar_artista_na_semana(Semana *semana, int artist_id, int duracao);

/**
 * @brief Obtém a lista dos top artistas de uma semana.
 *
 * Esta função retorna a lista dos artistas mais populares (top 10) para uma determinada semana.
 *
 * @param semana Ponteiro para a estrutura Semana da qual os top artistas devem ser recuperados.
 * @return GList* Lista dos artistas mais populares na semana.
 */
GList *get_top_artistas_na_semana(Semana *semana);

#endif /* QUERY4_AUX_H */
