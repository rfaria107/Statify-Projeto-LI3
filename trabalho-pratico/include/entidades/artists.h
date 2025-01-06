/**
 * @file artista.h
 * @brief Header do módulo dos artistas.
 *
 * Este módulo contém funções e estruturas para as entidades artista (Artista), como
 * a estrutura opaca dos artistas, funções para aceder (get) aos diferentes campos da struct, criar uma entidade artista nova, inicializar, modificar e libertar (free).
 */

#ifndef ARTISTA_H
#define ARTISTA_H

#include <glib.h>

/**
 * @typedef Artista
 * @brief Estrutura de dados que representa um artista (opaca).
 *
 * A estrutura guarda a informação de uma entidade do tipo Artista.
 */
typedef struct Artista Artista;

/**
 * @brief Busca (retorna) o ID de um artista.
 *
 * @param artista Pointer para o artista.
 * @return O ID do artista (inteiro).
 */
int get_artist_id(Artista *artista);

/**
 * @brief Busca (retorna) o nome de um artista.
 *
 * @param artista Pointer para o artista.
 * @return Um pointer para uma string com o nome do artista.
 */
gchar *get_artist_name(Artista *artista);

/**
 * @brief Busca (retorna) a receita por stream de um artista.
 *
 * @param artista Pointer para o artista.
 * @return A receita por stream do artista (double).
 */
gdouble get_artist_recipe_per_stream(Artista *artista);

/**
 * @brief Busca (retorna) os IDs dos artistas constituintes (em caso de banda).
 *
 * @param artista Pointer para o artista.
 * @return Um array de strings que contém os IDs dos artistas constituintes.
 */
gchar **get_artist_id_constituent(Artista *artista);

/**
 * @brief Busca (retorna) o país de origem de um artista.
 *
 * @param artista Pointer para o artista.
 * @return Um pointer para uma string com o país de origem do artista.
 */
gchar *get_artist_country(Artista *artista);

/**
 * @brief Busca (retorna) o tipo de artista (e.g., banda ou solo).
 *
 * @param artista Pointer para o artista.
 * @return Um pointer para uma string com o tipo do artista.
 */
gchar *get_artist_type(Artista *artista);

/**
 * @brief Busca (retorna) a duração total do disco do artista.
 *
 * @param artista Pointer para o artista.
 * @return A duração total do disco em minutos (inteiro).
 */
gint get_artist_duracao_disco(Artista *artista);


/**
 * @brief Conta o número de constituintes (membros) de um artista (grupo).
 *
 * Esta função retorna o número de membros constituintes de um artista. Esta função apenas é usada para calcular
 * a receita de participação de um artista membro de um grupo no módulo stats.
 *
 * @param artista Pointer para o artista.
 * @return O número de constituintes do artista (inteiro).
 */
int get_num_constituents(Artista *artista);


/**
 * @brief Define a nova duração total do disco do artista.
 *
 * @param artista Pointer para o artista.
 * @param nova_duracao A nova duração total do disco em minutos (inteiro).
 */
void set_artista_duracao_disco(Artista *artista, gint nova_duracao);

/**
 * @brief Liberta a memória reservada a uma entidade Artista.
 *
 * @param artista Pointer para o Artista a libertar.
 */
void free_artista(Artista *artista);

/**
 * @brief Inicializa uma entidade Artista.
 *
 * A função cria uma entidade Artista vazia, ou seja, com os valores a 0 ou NULL nos campos.
 *
 * @return Um pointer para o Artista inicializado.
 */
Artista *inicializar_artista();

/**
 * @brief Cria uma nova entidade Artista com os parâmetros passados.
 *
 * @param id O ID do artista (inteiro).
 * @param name O nome do artista (pointer para string).
 * @param recipe_per_stream A receita por stream do artista (double).
 * @param id_constituent Um array de strings com os IDs dos artistas constituintes.
 * @param country O país de origem do artista (pointer para string).
 * @param type O tipo do artista (e.g., banda ou solo) (pointer para string).
 * @return Um pointer para o Artista criado.
 */
Artista *create_artista(int id, gchar *name, gdouble recipe_per_stream, gchar **id_constituent, gchar *country, gchar *type);

#endif // ARTISTA_H
