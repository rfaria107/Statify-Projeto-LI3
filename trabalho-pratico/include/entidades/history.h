/**
 * @file history.h
 * @brief Header do módulo do histórico de reproduções.
 *
 * Este módulo contém as estruturas e funções para gerir as entidades histórico (History), como
 * a estrutura opaca dos históricos, funções para aceder (get) aos diferentes campos da struct, criar uma entidade histórico nova, inicializar e libertar (free).
 */

#ifndef HISTORY_H
#define HISTORY_H

#include <glib.h>

/**
 * @typedef History
 * @brief Estrutura de dados que representa um histórico de reprodução (opaca).
 *
 * A estrutura guarda a informação de uma entidade do tipo History.
 */
typedef struct History History;

/**
 * @brief Busca (retorna) o ID de um histórico.
 *
 * @param history Pointer para o histórico.
 * @return O ID do histórico (inteiro).
 */
int get_history_id(History *history);

/**
 * @brief Busca (retorna) o ID do usuário associado ao histórico.
 *
 * @param history Pointer para o histórico.
 * @return O ID do usuário (inteiro).
 */
int get_history_user_id(History *history);

/**
 * @brief Busca (retorna) o ID da música associada ao histórico.
 *
 * @param history Pointer para o histórico.
 * @return O ID da música (inteiro).
 */
int get_history_music_id(History *history);

/**
 * @brief Busca (retorna) o timestamp do histórico.
 *
 * @param history Pointer para o histórico.
 * @return Um pointer para uma string com o timestamp do histórico.
 */
gchar *get_history_timestamp(History *history);

/**
 * @brief Busca (retorna) a duração da reprodução no histórico.
 *
 * @param history Pointer para o histórico.
 * @return Um pointer para uma string com a duração da reprodução.
 */
gchar *get_history_duration(History *history);

/**
 * @brief Busca (retorna) a plataforma associada ao histórico.
 *
 * @param history Pointer para o histórico.
 * @return Um pointer para uma string com a plataforma associada.
 */
gchar *get_history_platform(History *history);

/**
 * @brief Busca (retorna) a data associada ao histórico.
 *
 * @param history Pointer para o histórico.
 * @return Um pointer para uma string com a data associada.
 */
gchar *get_history_date(History *history);

/**
 * @brief Cria uma nova entidade History com os parâmetros passados.
 *
 * @param id O ID do histórico (inteiro).
 * @param user_id O ID do usuário associado ao histórico (inteiro).
 * @param music_id O ID da música associada ao histórico (inteiro).
 * @param timestamp O timestamp do histórico (pointer para string).
 * @param duration A duração da reprodução no histórico (pointer para string).
 * @param platform A plataforma associada ao histórico (pointer para string).
 * @return Um pointer para o History criado.
 */
History *create_history(int id, int user_id, int music_id, char *timestamp, char *duration, char *platform);

/**
 * @brief Inicializa uma entidade History.
 *
 * A função cria uma entidade History vazia, ou seja, com os valores a 0 ou NULL nos campos.
 *
 * @return Um pointer para o History inicializado.
 */
History *inicializar_history();

/**
 * @brief Liberta a memória reservada a uma entidade History.
 *
 * @param history Pointer para o History a libertar.
 */
void free_history(History *history);

#endif // HISTORY_H
