/**
 * @file gestor_artistas.h
 * @brief Header do módulo Gestor de Artistas.
 *
 * Este módulo define e implementa um gestor para manipulação e gerenciamento de artistas (entidades do tipo Artista).
 * O gestor utiliza uma GHashTable para armazenar as entidades Artista e fornece funções para inicializar,
 * inserir, buscar e libertar os artistas gerenciados.
 */

#ifndef GESTOR_ARTISTAS_H
#define GESTOR_ARTISTAS_H

#include <glib.h>
#include "../include/entidades/artists.h"

/**
 * @typedef GestorArtistas
 * @brief Estrutura de dados que representa o gestor de artistas (opaca).
 *
 * O GestorArtistas é responsável por armazenar e gerenciar os artistas numa GHashTable.
 */
typedef struct GestorArtistas GestorArtistas;

/**
 * @brief Inicializa um GestorArtistas existente.
 *
 * Esta função prepara o GestorArtistas para armazenar artistas, alocando e configurando a GHashTable.
 *
 * @param gestor Pointer para o GestorArtistas a ser inicializado.
 */
void inicializar_gestor_artistas(GestorArtistas *gestor);

/**
 * @brief Cria e inicializa um novo GestorArtistas.
 *
 * @return Um pointer para o GestorArtistas criado.
 */
GestorArtistas* criar_gestor_artistas();

/**
 * @brief Liberta a memória associada a um GestorArtistas.
 *
 * Esta função liberta a GHashTable e todos os artistas armazenados nela.
 *
 * @param gestor Pointer para o GestorArtistas a ser libertado.
 */
void liberar_gestor_artistas(GestorArtistas *gestor);

/**
 * @brief Insere um artista no GestorArtistas.
 *
 * @param gestor Pointer para o GestorArtistas onde o artista será inserido.
 * @param artista Pointer para o Artista a ser inserido.
 */
void inserir_artista(GestorArtistas *gestor, Artista *artista);

/**
 * @brief Obtém a GHashTable que armazena os artistas gerenciados pelo GestorArtistas.
 *
 * @param gestor Pointer para o GestorArtistas.
 * @return Um pointer para a GHashTable que contém os artistas.
 */
GHashTable *get_hash_artistas(GestorArtistas *gestor);

/**
 * @brief Busca um artista no GestorArtistas pelo ID.
 *
 * @param gestor Pointer para o GestorArtistas onde a busca será realizada.
 * @param id O ID do artista a ser buscado.
 * @return Um pointer para o Artista encontrado, ou NULL se não for encontrado.
 */
Artista* buscar_artista(GestorArtistas *gestor, int id);

/**
 * @brief Liberta a memória associada a um valor de artista armazenado na GHashTable.
 *
 * @param value Um pointer para o valor (Artista) a ser libertado.
 */
void free_artista_value(gpointer value);

#endif // GESTOR_ARTISTAS_H
