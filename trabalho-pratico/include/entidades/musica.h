/**
 * @file musica.h
 * @brief Header do módulo das músicas.
 *
 * Este módulo contém as estruturas e funções para gerir as entidades música (Musica), como
 * a estrutura opaca das músicas, funções para aceder (get) aos diferentes campos da struct, criar uma entidade música nova, inicializar e libertar (free).
 */

#ifndef MUSICA_H
#define MUSICA_H

#include <glib.h>

/**
 * @typedef Musica
 * @brief Estrutura de dados que representa uma música (opaca).
 *
 * A estrutura guarda a informação de uma entidade do tipo Musica.
 */
typedef struct Musica Musica;

/**
 * @brief Cria uma nova entidade Musica com os parâmetros passados.
 *
 * @param id O ID da música (inteiro).
 * @param title O título da música (pointer para string).
 * @param artist_ids Um array de strings com os IDs dos artistas associados.
 * @param album_id O ID do álbum associado à música (pointer para string).
 * @param duration A duração da música (pointer para string).
 * @param genre O gênero da música (pointer para string).
 * @param year O ano de lançamento da música (inteiro).
 * @param streams O número de streams da música (inteiro).
 * @return Um pointer para a Musica criada.
 */
Musica *create_musica(int id, char *title, char **artist_ids, char *album_id, char *duration, char *genre, int year, int streams);

/**
 * @brief Inicializa uma entidade Musica.
 *
 * A função cria uma entidade Musica vazia, ou seja, com os valores a 0 ou NULL nos campos.
 *
 * @return Um pointer para a Musica inicializada.
 */
Musica *inicializar_musica();

/**
 * @brief Liberta a memória reservada a uma entidade Musica.
 *
 * @param musica Pointer para a Musica a ser libertada.
 */
void free_musica(Musica *musica);

/**
 * @brief Busca (retorna) o ID de uma música.
 *
 * @param musica Pointer para a música.
 * @return O ID da música (inteiro).
 */
int get_music_id(Musica *musica);

/**
 * @brief Busca (retorna) o título de uma música.
 *
 * @param musica Pointer para a música.
 * @return Um pointer para uma string com o título da música.
 */
gchar *get_music_title(Musica *musica);

/**
 * @brief Busca (retorna) os IDs dos artistas associados a uma música.
 *
 * @param musica Pointer para a música.
 * @return Um array de strings com os IDs dos artistas.
 */
gchar **get_music_artist_ids(Musica *musica);

/**
 * @brief Busca (retorna) o ID do álbum associado a uma música.
 *
 * @param musica Pointer para a música.
 * @return Um pointer para uma string com o ID do álbum.
 */
gchar *get_music_album(Musica *musica);

/**
 * @brief Busca (retorna) a duração de uma música.
 *
 * @param musica Pointer para a música.
 * @return Um pointer para uma string com a duração da música.
 */
gchar *get_music_duration(Musica *musica);

/**
 * @brief Busca (retorna) o gênero de uma música.
 *
 * @param musica Pointer para a música.
 * @return Um pointer para uma string com o gênero da música.
 */
gchar *get_music_genre(Musica *musica);

/**
 * @brief Busca (retorna) o ano de lançamento de uma música.
 *
 * @param musica Pointer para a música.
 * @return O ano de lançamento da música (inteiro).
 */
gint get_music_year(Musica *musica);

/**
 * @brief Busca (retorna) o número de streams de uma música.
 *
 * @param musica Pointer para a música.
 * @return O número de streams da música (inteiro).
 */
gint get_music_streams(Musica *musica);

/**
 * @brief Define (atualiza) o número de streams de uma música.
 *
 * @param musica Pointer para a música.
 * @param stream_count O novo número de streams (inteiro).
 */
void set_music_streams(Musica *musica, int stream_count);

#endif // MUSICA_H
