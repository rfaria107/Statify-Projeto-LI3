/**
 * @file gestor_musicas.h
 * @brief Header do módulo Gestor de Músicas.
 *
 * Este módulo define e implementa um gestor de músicas (entidades do tipo Musica).
 * O gestor utiliza uma GHashTable para armazenar as entidades Musica e fornece funções para inicializar,
 * inserir, buscar e libertar as músicas geridos.
 */

#ifndef GESTOR_MUSICAS_H
#define GESTOR_MUSICAS_H

#include <glib.h>
#include "../include/entidades/musica.h"

/**
 * @typedef GestorMusicas
 * @brief Estrutura de dados que representa o gestor de músicas (opaca).
 *
 * O GestorMusicas é responsável por armazenar e gerenciar as músicas numa GHashTable.
 */
typedef struct GestorMusicas GestorMusicas;

/**
 * @brief Cria e inicializa um novo GestorMusicas.
 *
 * @return Um pointer para o GestorMusicas criado.
 */
GestorMusicas* criar_gestor_musicas();

/**
 * @brief Inicializa um GestorMusicas existente.
 *
 * Esta função prepara o GestorMusicas para armazenar músicas, alocando e configurando a GHashTable.
 *
 * @param gestor Pointer para o GestorMusicas a ser inicializado.
 */
void inicializar_gestor_musicas(GestorMusicas* gestor);

/**
 * @brief Liberta a memória associada a um GestorMusicas.
 *
 * Esta função liberta a GHashTable e todas as músicas armazenadas nela.
 *
 * @param gestor Pointer para o GestorMusicas a ser libertado.
 */
void liberar_gestor_musicas(GestorMusicas* gestor);

/**
 * @brief Insere uma música no GestorMusicas.
 *
 * @param gestor Pointer para o GestorMusicas onde a música será inserida.
 * @param musica Pointer para a Musica a ser inserida.
 */
void inserir_musica(GestorMusicas *gestor, Musica *musica);

/**
 * @brief Busca uma música no GestorMusicas pelo ID.
 *
 * @param gestor Pointer para o GestorMusicas onde a busca será realizada.
 * @param id O ID da música a ser buscada.
 * @return Um pointer para a Musica encontrada, ou NULL se não for encontrada.
 */
Musica* buscar_musicas(GestorMusicas *gestor, int id);

/**
 * @brief Obtém a GHashTable que armazena as músicas geridos pelo GestorMusicas.
 *
 * @param gestor Pointer para o GestorMusicas.
 * @return Um pointer para a GHashTable que contém as músicas.
 */
GHashTable *get_hash_musicas(GestorMusicas *gestor);

/**
 * @brief Liberta a memória associada a um valor de música armazenado na GHashTable.
 *
 * @param value Um pointer para o valor (Musica) a ser libertado.
 */
void free_music_value(gpointer value);

#endif // GESTOR_MUSICAS_H
