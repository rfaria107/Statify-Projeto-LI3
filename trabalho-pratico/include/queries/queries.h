#ifndef QUERIES_H
#define QUERIES_H

#include <glib.h>
#include <stdio.h>
#include "../include/gestores/gestor_sistemas.h"
#include "../include/queries/query4_aux.h"

/**
 * @brief Executa a Query 1: : Listar o resumo de um utilizador ou artista, consoante o identificador recebido por argumento.
 *
 * A função executa a primeira query, que pode buscar dados de um usuário ou de um artista,
 * dependendo do token fornecido. Se o token começar com 'U', a consulta será realizada para um 
 * usuário, e se começar com 'A', será realizada para um artista. A função gera um arquivo de 
 * saída com os resultados da consulta, formatando os dados de acordo com o valor das flags `n` e `t`.
 * A consulta pode escrever ou imprimir os dados em diferentes formatos (CSV ou impressão no console).
 *
 * @param gestorsis Ponteiro para o GestorSistema que contém os dados de usuários, artistas, álbuns e músicas.
 * @param token Token que identifica o tipo de consulta: 'U' para usuário ou 'A' para artista, seguido do ID.
 * @param line_number Número da linha do comando no arquivo de entrada. Usado para nomear o arquivo de saída.
 * @param n Flag que controla o separador de escrita, 0 para ; e 1 para =.
 * @param l Flag que controla o formato de saída: 0 para escrever no arquivo e 1 para imprimir na consola.
 */
void query_1(GestorSistema *gestorsis, gchar *token, int line_number, int n, int l);

/**
 * @brief Executa a Query 2: Quais são os top N artistas com maior discografia?
 *
 * A função executa a segunda query, que busca a duração da discografia de artistas com base num número máximo
 * e um país fornecido. A consulta pode ser filtrada por país, retornando apenas os artistas que pertencem
 * ao país especificado. O número máximo de artistas a ser retornado é determinado pelo parâmetro `num`.
 * Os resultados podem ser escritos em um arquivo CSV ou impressos no console, dependendo das flags `n` e `t`.
 *
 * @param gestorsis Ponteiro para o GestorSistema que contém os dados de artistas e músicas.
 * @param num Número máximo de artistas a ser retornado pela consulta.
 * @param country País a ser utilizado como filtro na consulta. Se `NULL`, não há filtro de país.
 * @param line_number Número da linha do comando no arquivo de entrada. Usado para nomear o arquivo de saída.
 * @param n Flag que controla o separador de escrita, 0 para ; e 1 para =.
 * @param l Flag que controla o formato de saída: 0 para escrever no arquivo e 1 para imprimir na consola.
 */
void query_2(GestorSistema *gestorsis, int num, gchar *country, int line_number, int n, int l);

/**
 * @brief Executa a Query 3: Quais são os géneros de música mais populares numa determinada faixa etária?
 *
 * A função executa a terceira query, que produz como output uma lista ordenada dos generos mais populares na faixa etárias
 * em ordem decrescente e o total de likes de cada um destes.
 *
 * @param min_age Idade mínima para a consulta.
 * @param max_age Idade máxima para a consulta.
 * @param gestor_sistema Ponteiro para o GestorSistema que contém os dados.
 * @param line_number Número da linha do comando no arquivo de entrada.
 * @param n Flag que controla o separador de escrita, 0 para ; e 1 para =.
 * @param l Flag que controla o formato de saída: 0 para escrever no arquivo e 1 para imprimir na consola.
 */
void query_3(int min_age, int max_age, GestorSistema *gestor_sistema, int line_number, int n, int l);

/**
 * @brief Executa a Query 4: Qual é o artista que esteve no top 10 mais vezes?
 *
 * A função executa a quarta query, que envolve a busca do artista mais popular num intervalo
 * de datas fornecido ou não, estes intervalos são pré-processados, a query apenas faz a consulta.
 *
 * @param data_inicial Data inicial para o intervalo de consulta.
 * @param data_final Data final para o intervalo de consulta.
 * @param gestor_sistema Ponteiro para o GestorSistema que contém os dados.
 * @param line_number Número da linha do comando no arquivo de entrada.
 * @param n Flag que controla o separador de escrita, 0 para ; e 1 para =.
 * @param resultado Ponteiro para o ResultadoProcessamento com dados adicionais.
 * @param l Flag que controla o formato de saída: 0 para escrever no arquivo e 1 para imprimir na consola.
 */
void query_4(char *data_inicial, char *data_final, GestorSistema *gestor_sistema, int line_number, int n, ResultadoProcessamento *resultado, int l);

/**
 * @brief Executa a Query 5: Recomendação de utilizadores com gostos parecidos
 *
 * A função executa a quinta query, que envolve a recomendação de músicas para
 * um usuário específico com base numa matriz de classificações.
 * 
 * @param user_id ID do usuário para o qual as recomendações serão feitas.
 * @param matrizClassificacaoMusicas Matriz de classificações das músicas.
 * @param idsUtilizadores Lista de strings com IDs dos usuários.
 * @param nomesGeneros Lista de strings com os nomes dos gêneros musicais.
 * @param numUtilizadores Número total de usuários.
 * @param numGeneros Número total de gêneros.
 * @param numRecomendacoes Número de recomendações a serem feitas.
 * @param linenumber Número da linha do comando no arquivo de entrada.
 * @param n Flag que controla o separador de escrita, 0 para ; e 1 para =.
 * @param gestorsis Ponteiro para o GestorSistema que contém os dados.
 * @param l Flag que controla o formato de saída: 0 para escrever no arquivo e 1 para imprimir na consola.
 */
void query_5(char *user_id, int **matrizClassificacaoMusicas, char **idsUtilizadores, char **nomesGeneros,
             int numUtilizadores, int numGeneros, int numRecomendacoes, int linenumber, int n, GestorSistema *gestorsis, int l);

/**
 * @brief Executa a Query 6: Resumo anual para um utilizador
 *
 * A função executa a sexta query, que escreve no ficheiro de output um resumo das estatísticas anuais de um usuário.
 *
 * @param user_id ID do usuário para a query.
 * @param year Ano de referência para a query.
 * @param N Número de registros a serem retornados.
 * @param gestorsis Ponteiro para o GestorSistema que contém os dados.
 * @param line_number Número da linha do comando no arquivo de entrada.
 * @param n Flag que controla o separador de escrita, 0 para ; e 1 para =.
 * @param l Flag que controla o formato de saída: 0 para escrever no arquivo e 1 para imprimir na consola.
 */
void query_6(int user_id, int year, int N, GestorSistema *gestorsis, int line_number, int n, int l);

#endif // QUERIES_H
