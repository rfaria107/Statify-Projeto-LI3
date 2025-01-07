/**
 * @file gestor_histories.h
 * @brief Header do módulo Gestor de Histories.
 *
 * Este módulo define e implementa um gestor de históricos (entidades do tipo History).
 * O gestor utiliza uma GHashTable para armazenar as entidades History e fornece funções para inicializar,
 * inserir, buscar e libertar os históricos geridos.
 */

#ifndef GESTOR_HISTORIES_H
#define GESTOR_HISTORIES_H

#include <glib.h>
#include "../include/entidades/history.h"

/**
 * @typedef GestorHistories
 * @brief Estrutura de dados que representa o gestor de históricos (opaca).
 *
 * O GestorHistories é responsável por armazenar e gerenciar os históricos numa GHashTable.
 */
typedef struct GestorHistories GestorHistories;

/**
 * @brief Cria e inicializa um novo GestorHistories.
 *
 * @return Um pointer para o GestorHistories criado.
 */
GestorHistories* criar_gestor_histories();

/**
 * @brief Inicializa um GestorHistories existente.
 *
 * Esta função prepara o GestorHistories para armazenar históricos, alocando e configurando a GHashTable.
 *
 * @param gestor Pointer para o GestorHistories a ser inicializado.
 */
void inicializar_gestor_histories(GestorHistories *gestor);

/**
 * @brief Liberta a memória associada a um GestorHistories.
 *
 * Esta função liberta a GHashTable e todos os históricos armazenados nela.
 *
 * @param gestor Pointer para o GestorHistories a ser libertado.
 */
void liberar_gestor_histories(GestorHistories *gestor);

/**
 * @brief Insere um histórico no GestorHistories.
 *
 * @param gestor Pointer para o GestorHistories onde o histórico será inserido.
 * @param history Pointer para o History a ser inserido.
 */
void inserir_history(GestorHistories *gestor, History *history);

/**
 * @brief Busca um histórico no GestorHistories pelo ID.
 *
 * @param gestor Pointer para o GestorHistories onde a busca será realizada.
 * @param id O ID do histórico a ser buscado.
 * @return Um pointer para o History encontrado, ou NULL se não for encontrado.
 */
History* buscar_history(GestorHistories *gestor, int id);

/**
 * @brief Busca um histórico no GestorHistories pelo ID de usuário.
 *
 * @param gestor Pointer para o GestorHistories onde a busca será realizada.
 * @param user_id O ID do usuário cujo histórico será buscado.
 * @return Um pointer para o History encontrado, ou NULL se não for encontrado.
 */
History* buscar_history_por_user_id(GestorHistories *gestor, int user_id);

/**
 * @brief Obtém a GHashTable que armazena os históricos geridos pelo GestorHistories.
 *
 * @param gestor Pointer para o GestorHistories.
 * @return Um pointer para a GHashTable que contém os históricos.
 */
GHashTable *get_hash_histories(GestorHistories *gestor);

/**
 * @brief Liberta a memória associada a um valor de histórico armazenado na GHashTable.
 *
 * @param value Um pointer para o valor (History) a ser libertado.
 */
void free_history_value(gpointer value);

#endif // GESTOR_HISTORIES_H
