/**
 * @file query5_aux.h
 * @brief Declarações de funções auxiliares para a query 5.
 *
 * Este ficheiro contém as declarações das funções utilizadas para preparar e calcular a matriz de classificações de músicas
 * com base nos géneros musicais e utilizadores registados no sistema, bem como as listas destes.
 *
 */

#ifndef QUERY5_AUX_H
#define QUERY5_AUX_H

#include "../include/gestores/gestor_sistemas.h"

/**
 * @brief Pré-processa os nomes dos géneros musicais a partir do sistema de gestão.
 *
 * Esta função obtém todos os géneros únicos registados no sistema e devolve-os como um array de strings.
 *
 * @param gestorsis Ponteiro para o gestor do sistema.
 * @param numGeneros Ponteiro para uma variável onde será armazenado o número de géneros encontrados.
 * @return Um array de strings contendo os nomes dos géneros musicais.
 */
char **preprocessNomesGeneros(GestorSistema *gestorsis, int *numGeneros);

/**
 * @brief Pré-processa os IDs dos utilizadores registados no sistema.
 *
 * Esta função obtém os IDs de todos os utilizadores registados no sistema e devolve-os como um array de strings.
 * Os IDs são formatados no estilo "U0000001".
 *
 * @param gestorsis Ponteiro para o gestor do sistema.
 * @param numUtilizadores Ponteiro para uma variável onde será armazenado o número de utilizadores encontrados.
 * @return Um array de strings contendo os IDs dos utilizadores.
 */
char **preprocessIdsUtilizadores(GestorSistema *gestorsis, int *numUtilizadores);

/**
 * @brief Liberta a memória alocada para a matriz de classificações de músicas.
 *
 * Esta função liberta a memória utilizada pela matriz de classificações de músicas.
 *
 * @param matriz Ponteiro para a matriz de classificações.
 * @param numUtilizadores Número de utilizadores (linhas da matriz).
 */
void freeMatrizClassificacaoMusicas(int **matriz, int numUtilizadores);

/**
 * @brief Cria uma matriz de classificações de músicas.
 *
 * Esta função aloca memória para uma matriz de inteiros, onde cada linha corresponde a um utilizador
 * e cada coluna a um género musical. A matriz é inicializada com zeros.
 *
 * @param numUtilizadores Número de utilizadores (linhas da matriz).
 * @param numGeneros Número de géneros musicais (colunas da matriz).
 * @return Ponteiro para a matriz criada ou NULL em caso de erro.
 */
int **createMatrizClassificacaoMusicas(int numUtilizadores, int numGeneros);

/**
 * @brief Calcula a matriz de classificações de músicas com base nos históricos e géneros musicais.
 *
 * Esta função preenche a matriz de classificações de músicas, incrementando os valores com base nas músicas
 * ouvidas pelos utilizadores, associando cada entrada ao género correspondente.
 *
 * @param matriz Ponteiro para a matriz de classificações a ser preenchida.
 * @param ids_utilizadores Array de strings com os IDs dos utilizadores.
 * @param nomes_generos Array de strings com os nomes dos géneros musicais.
 * @param num_utilizadores Número de utilizadores (linhas da matriz).
 * @param num_generos Número de géneros musicais (colunas da matriz).
 * @param gestorsis Ponteiro para o gestor do sistema.
 */
void calculaMatrizClassificacaoMusicas(int **matriz, char **ids_utilizadores, char **nomes_generos,
                                       int num_utilizadores, int num_generos, GestorSistema *gestorsis);

#endif /* QUERY5_AUX_H */