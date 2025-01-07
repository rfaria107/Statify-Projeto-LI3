/**
 * @file gestor_sistema.h
 * @brief Header do módulo Gestor do Sistema.
 *
 * Este módulo define e implementa o gestor principal do sistema, que integra e gere todos os outros gestores:
 * Artistas, Usuários, Músicas, Histories e Álbuns.
 */

#ifndef GESTOR_SISTEMA_H
#define GESTOR_SISTEMA_H

#include <glib.h>
#include "../include/gestores/gestor_artistas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/gestores/gestor_musicas.h"
#include "../include/gestores/gestor_albuns.h"
#include "../include/gestores/gestor_histories.h"

/**
 * @typedef GestorSistema
 * @brief Estrutura de dados que representa o gestor principal do sistema (opaca).
 *
 * O GestorSistema é responsável por integrar e gerir todos os outros gestores.
 */
typedef struct GestorSistema GestorSistema;

/**
 * @brief Cria e inicializa um novo GestorSistema.
 *
 * @return Um pointer para o GestorSistema criado.
 */
GestorSistema* criar_gestor_sistema();

/**
 * @brief Inicializa um GestorSistema existente.
 *
 * Esta função incializa o GestorSistema, criando os gestores que este irá geir.
 *
 * @param gestor Pointer para o GestorSistema a ser inicializado.
 */
void inicializar_gestor_sistema(GestorSistema *gestor);

/**
 * @brief Liberta a memória associada a um GestorSistema.
 *
 * Esta função liberta todos os gestores integrados e a memória associada ao GestorSistema.
 *
 * @param gestor Pointer para o GestorSistema a ser libertado.
 */
void liberar_gestor_sistema(GestorSistema *gestor);

/**
 * @brief Obtém o gestor de usuários do GestorSistema.
 *
 * @param gestor Pointer para o GestorSistema.
 * @return Um pointer para o GestorUsuarios.
 */
GestorUsuarios* get_gestor_usuarios(GestorSistema* gestor);

/**
 * @brief Obtém o gestor de artistas do GestorSistema.
 *
 * @param gestor Pointer para o GestorSistema.
 * @return Um pointer para o GestorArtistas.
 */
GestorArtistas* get_gestor_artistas(GestorSistema* gestor);

/**
 * @brief Obtém o gestor de músicas do GestorSistema.
 *
 * @param gestor Pointer para o GestorSistema.
 * @return Um pointer para o GestorMusicas.
 */
GestorMusicas* get_gestor_musicas(GestorSistema* gestor);

/**
 * @brief Obtém o gestor de álbuns do GestorSistema.
 *
 * @param gestor Pointer para o GestorSistema.
 * @return Um pointer para o GestorAlbuns.
 */
GestorAlbuns* get_gestor_albuns(GestorSistema* gestor);

/**
 * @brief Obtém o gestor de históricos do GestorSistema.
 *
 * @param gestor Pointer para o GestorSistema.
 * @return Um pointer para o GestorHistories.
 */
GestorHistories* get_gestor_histories(GestorSistema* gestor);

#endif // GESTOR_SISTEMA_H
