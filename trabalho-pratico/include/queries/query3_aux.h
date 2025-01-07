/**
 * @file query3_aux.h
 * @brief Header das declarações de funções auxiliares para a query3.
 *
 * As funções auxiliares da query 3 consistem maioritariamente no processamento da estrtura
 * GenrePopularity, que associa a um género de música os likes que este teve, necessário para
 * a execução da query.
 *
 */

#ifndef QUERY3_AUX_H
#define QUERY3_AUX_H

/**
 * @typedef GenrePopularity
 * @brief Estrutura que representa a popularidade de um género musical.
 *
 * A estrutura `GenrePopularity` armazena informações sobre um género musical e a sua popularidade,
 * que é medida pelo total de "likes" associados a esse género.
 */
typedef struct GenrePopularity GenrePopularity;

/**
 * @brief Cria uma nova instância de `GenrePopularity`.
 *
 * A função cria uma nova instância da estrutura `GenrePopularity`, inicializando o género musical
 * e o número total de "likes". A memória alocada deve ser libertada com a função `free_genre_popularity`.
 *
 * @param genre Género musical a ser associado à nova instância.
 * @return Ponteiro para a nova instância de `GenrePopularity`.
 */
GenrePopularity *create_genre_popularity(const char *genre);

/**
 * @brief Liberta a memória de uma instância de `GenrePopularity`.
 *
 * A função liberta a memória alocada para a instância de `GenrePopularity`, incluindo o género associado.
 *
 * @param gp Ponteiro para a instância de `GenrePopularity` a ser libertada.
 */
void free_genre_popularity(GenrePopularity *gp);

/**
 * @brief Função de comparação para ordenar instâncias de `GenrePopularity` por popularidade.
 *
 * A função compara duas instâncias de `GenrePopularity` com base no total de "likes" para ordenar
 * os géneros de forma decrescente, ou seja, o género com mais "likes" será considerado maior.
 *
 * @param a Ponteiro para a primeira instância de `GenrePopularity`.
 * @param b Ponteiro para a segunda instância de `GenrePopularity`.
 * @return Valor negativo se `a` for menos popular que `b`, valor positivo se `a` for mais popular que `b`,
 *         ou zero se ambos tiverem a mesma popularidade.
 */
int compare_genre_popularity(const void *a, const void *b);

/**
 * @brief Obtém o género musical associado a uma instância de `GenrePopularity`.
 *
 * A função retorna o género musical armazenado na instância de `GenrePopularity`.
 *
 * @param gp Ponteiro para a instância de `GenrePopularity`.
 * @return O género musical associado à instância.
 */
char *get_gp_genre(GenrePopularity *gp);

/**
 * @brief Define o género musical para uma instância de `GenrePopularity`.
 *
 * A função define o género musical associado a uma instância de `GenrePopularity`.
 *
 * @param gp Ponteiro para a instância de `GenrePopularity`.
 * @param genre Género musical a ser associado à instância.
 */
void set_gp_genre(GenrePopularity *gp, char *genre);

/**
 * @brief Obtém o total de "likes" de uma instância de `GenrePopularity`.
 *
 * A função retorna o número total de "likes" associado ao género musical na instância de `GenrePopularity`.
 *
 * @param gp Ponteiro para a instância de `GenrePopularity`.
 * @return O número total de "likes" associado ao género musical.
 */
int get_gp_total_likes(GenrePopularity *gp);

/**
 * @brief Define o total de "likes" para uma instância de `GenrePopularity`.
 *
 * A função define o número total de "likes" associado ao género musical na instância de `GenrePopularity`.
 *
 * @param gp Ponteiro para a instância de `GenrePopularity`.
 * @param total_likes Número total de "likes" a ser associado à instância.
 */
void set_gp_total_likes(GenrePopularity *gp, int total_likes);

#endif
