/**
 * @file gestor_albuns.h
 * @brief Header do módulo Gestor de Álbuns.
 *
 * Este módulo define e implementa um gestor para manipulação e gerenciamento de álbuns (entidades do tipo Album).
 * O gestor utiliza uma GHashTable para armazenar as entidades Album e fornece funções para inicializar,
 * inserir, buscar e libertar os álbuns gerenciados.
 */

#ifndef GESTOR_ALBUNS_H
#define GESTOR_ALBUNS_H

#include <glib.h>
#include "../include/entidades/albuns.h"

/**
 * @typedef GestorAlbuns
 * @brief Estrutura de dados que representa o gestor de álbuns (opaca).
 *
 * O GestorAlbuns é responsável por armazenar e gerenciar os álbuns numa GHashTable.
 */
typedef struct GestorAlbuns GestorAlbuns;

/**
 * @brief Liberta a memória associada a um valor de álbum armazenado na GHashTable.
 *
 * @param value Um pointer para o valor (Album) a ser libertado.
 */
void free_album_value(gpointer value);

/**
 * @brief Obtém a GHashTable que armazena os álbuns gerenciados pelo GestorAlbuns.
 *
 * @param gestor Pointer para o GestorAlbuns.
 * @return Um pointer para a GHashTable que contém os álbuns.
 */
GHashTable *get_hash_albuns(GestorAlbuns *gestor);

/**
 * @brief Cria e inicializa um novo GestorAlbuns.
 *
 * @return Um pointer para o GestorAlbuns criado.
 */
GestorAlbuns *criar_gestor_albuns();

/**
 * @brief Inicializa um GestorAlbuns existente.
 *
 * Esta função prepara o GestorAlbuns para armazenar álbuns, alocando e configurando a GHashTable.
 *
 * @param gestor Pointer para o GestorAlbuns a ser inicializado.
 */
void inicializar_gestor_albuns(GestorAlbuns *gestor);

/**
 * @brief Liberta a memória associada a um GestorAlbuns.
 *
 * Esta função liberta a GHashTable e todos os álbuns armazenados nela.
 *
 * @param gestor Pointer para o GestorAlbuns a ser libertado.
 */
void liberar_gestor_albuns(GestorAlbuns *gestor);

/**
 * @brief Insere um álbum no GestorAlbuns.
 *
 * @param gestor Pointer para o GestorAlbuns onde o álbum será inserido.
 * @param album Pointer para o Album a ser inserido.
 */
void inserir_album(GestorAlbuns *gestor, Album *album);

/**
 * @brief Busca um álbum no GestorAlbuns pelo ID.
 *
 * @param gestor Pointer para o GestorAlbuns onde a busca será realizada.
 * @param id O ID do álbum a ser buscado.
 * @return Um pointer para o Album encontrado, ou NULL se não for encontrado.
 */
Album *buscar_album(GestorAlbuns *gestor, int id);

#endif // GESTOR_ALBUNS_H