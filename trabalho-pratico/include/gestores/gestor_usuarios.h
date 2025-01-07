/**
 * @file gestor_usuarios.h
 * @brief Header do módulo Gestor de Usuários.
 *
 * Este módulo define e implementa um gestor para manipulação e gerenciamento de usuários (entidades do tipo Usuario).
 * O gestor utiliza uma GHashTable para armazenar as entidades Usuario e fornece funções para inicializar,
 * inserir, buscar e libertar os usuários geridos.
 */

#ifndef GESTOR_USUARIOS_H
#define GESTOR_USUARIOS_H

#include <glib.h>
#include "../include/entidades/usuario.h"

/**
 * @typedef GestorUsuarios
 * @brief Estrutura de dados que representa o gestor de usuários (opaca).
 *
 * O GestorUsuarios é responsável por armazenar e gerir os usuários numa GHashTable.
 */
typedef struct GestorUsuarios GestorUsuarios;

/**
 * @brief Cria e inicializa um novo GestorUsuarios.
 *
 * @return Um pointer para o GestorUsuarios criado.
 */
GestorUsuarios* criar_gestor_usuarios();

/**
 * @brief Inicializa um GestorUsuarios existente.
 *
 * Esta função prepara o GestorUsuarios para armazenar usuários, alocando e configurando a GHashTable.
 *
 * @param gestor Pointer para o GestorUsuarios a ser inicializado.
 */
void inicializar_gestor_usuarios(GestorUsuarios *gestor);

/**
 * @brief Liberta a memória associada a um GestorUsuarios.
 *
 * Esta função liberta a GHashTable e todos os usuários armazenados nela.
 *
 * @param gestor Pointer para o GestorUsuarios a ser libertado.
 */
void liberar_gestor_usuarios(GestorUsuarios *gestor);

/**
 * @brief Insere um usuário no GestorUsuarios.
 *
 * @param gestor Pointer para o GestorUsuarios onde o usuário será inserido.
 * @param user Pointer para o Usuario a ser inserido.
 */
void inserir_usuario(GestorUsuarios *gestor, Usuario *user);

/**
 * @brief Busca um usuário no GestorUsuarios.
 *
 * @param gestor Pointer para o GestorUsuarios onde a busca será realizada.
 * @param usuario Pointer para o Usuario a ser buscado.
 * @return Um pointer para o Usuario encontrado, ou NULL se não for encontrado.
 */
Usuario* buscar_usuario(GestorUsuarios *gestor, Usuario *usuario);

/**
 * @brief Busca um usuário no GestorUsuarios pelo ID.
 *
 * @param gestor Pointer para o GestorUsuarios onde a busca será realizada.
 * @param id_usuario O ID do usuário a ser buscado.
 * @return Um pointer para o Usuario encontrado, ou NULL se não for encontrado.
 */
Usuario* buscar_usuario_id(GestorUsuarios *gestor, int id_usuario);

/**
 * @brief Obtém a GHashTable que armazena os usuários geridos pelo GestorUsuarios.
 *
 * @param gestor Pointer para o GestorUsuarios.
 * @return Um pointer para a GHashTable que contém os usuários.
 */
GHashTable* get_hash_usuarios(GestorUsuarios *gestor);

/**
 * @brief Liberta a memória associada a um valor de usuário armazenado na GHashTable.
 *
 * @param value Um pointer para o valor (Usuario) a ser libertado.
 */
void free_usuario_value(gpointer value);

#endif // GESTOR_USUARIOS_H
