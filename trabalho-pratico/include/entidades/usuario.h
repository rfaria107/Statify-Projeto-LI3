/**
 * @file usuario.h
 * @brief Header do módulo de usuários.
 *
 * Este módulo contém as estruturas e funções para gerir as entidades usuário (Usuario), como
 * a estrutura opaca dos usuariós (Usuario), funções para aceder (get) aos diferentes campos da struct, criar uma entidade usuário nova, inicializar e libertar (free).
 */

#ifndef USUARIO_H
#define USUARIO_H

#include <glib.h>

/**
 * @typedef Usuario
 * @brief Estrutura de dados que representa um usuário (opaca).
 *
 * A estrutura guarda a informação de uma entidade do tipo Usuario.
 */
typedef struct Usuario Usuario;

/**
 * @brief Inicializa uma entidade Usuario.
 *
 * A função cria uma entidade Usuario vazia, ou seja, com os valores a 0 ou NULL nos campos.
 *
 * @return Um pointer para o Usuario inicializado.
 */
Usuario *inicializar_usuario();

/**
 * @brief Cria uma nova entidade Usuario com os parâmetros fornecidos.
 *
 * @param username O ID do usuário (inteiro).
 * @param email O email do usuário (pointer para string).
 * @param first_name O primeiro nome do usuário (pointer para string).
 * @param last_name O sobrenome do usuário (pointer para string).
 * @param birth_date A data de nascimento do usuário (pointer para string no formato YYYY-MM-DD).
 * @param country O país do usuário (pointer para string).
 * @param subscription_type O tipo de assinatura do usuário (pointer para string).
 * @param liked_musics_id Um array de strings com os IDs das músicas com like do usuário.
 * @return Um pointer para a entidade Usuario criada.
 */
Usuario *create_usuario(int username, char *email, char *first_name,
                        char *last_name, char *birth_date,
                        char *country, char *subscription_type,
                        char **liked_musics_id);

/**
 * @brief Liberta a memória reservada a uma entidade Usuario.
 *
 * @param usuario Pointer para a entidade Usuario a ser libertada.
 */
void free_usuario(Usuario *usuario);

/**
 * @brief Calcula a idade do usuário com base na data de nascimento.
 *
 * @param usuario Pointer para a entidade Usuario.
 * @return A idade do usuário (inteiro).
 */
int calcularIdade(Usuario *usuario);

/**
 * @brief Busca (retorna) o ID de um usuário.
 *
 * @param user Pointer para o usuário.
 * @return O ID do usuário (inteiro).
 */
int user_get_id(Usuario *user);

/**
 * @brief Busca (retorna) o email de um usuário.
 *
 * @param user Pointer para o usuário.
 * @return Um pointer para uma string com o email do usuário.
 */
gchar *user_get_email(Usuario *user);

/**
 * @brief Busca (retorna) o primeiro nome de um usuário.
 *
 * @param user Pointer para o usuário.
 * @return Um pointer para uma string com o primeiro nome do usuário.
 */
gchar *user_get_first_name(Usuario *user);

/**
 * @brief Busca (retorna) o sobrenome de um usuário.
 *
 * @param user Pointer para o usuário.
 * @return Um pointer para uma string com o sobrenome do usuário.
 */
gchar *user_get_last_name(Usuario *user);

/**
 * @brief Busca (retorna) a data de nascimento de um usuário.
 *
 * @param user Pointer para o usuário.
 * @return Um pointer para uma string com a data de nascimento do usuário no formato YYYY-MM-DD.
 */
gchar *user_get_birth_date(Usuario *user);

/**
 * @brief Busca (retorna) o país de um usuário.
 *
 * @param user Pointer para o usuário.
 * @return Um pointer para uma string com o país do usuário.
 */
gchar *user_get_country(Usuario *user);

/**
 * @brief Busca (retorna) o tipo de assinatura de um usuário.
 *
 * @param user Pointer para o usuário.
 * @return Um pointer para uma string com o tipo de assinatura do usuário.
 */
gchar *user_get_subscription_type(Usuario *user);

/**
 * @brief Busca (retorna) a lista de IDs das músicas favoritas de um usuário.
 *
 * @param user Pointer para o usuário.
 * @return Um array de strings contendo os IDs das músicas favoritas.
 */
gchar **user_get_liked_musics_id(Usuario *user);

#endif // USUARIO_H
