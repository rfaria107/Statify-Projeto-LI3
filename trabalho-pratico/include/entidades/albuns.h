/**
 * @file album.h
 * @brief Header do módulo dos álbuns.
 *
 * Este módulo contém funções e estruturas para as entidades álbum (Album), como
 * a estrutura opaca dos álbuns, funções para aceder (get) aos diferentes campos da struct Album, criar uma entidade álbum nova, incializar e libertar (free).
 */

#ifndef ALBUM_H
#define ALBUM_H

#include <glib.h>

/**
 * @typedef Album
 * @brief Estrutura de dados que representa um álbum (opaca).
 *
 * A estrutura guarda a informação duma entidade do tipo Album.
 */
typedef struct Album Album;

/**
 * @brief Busca (retorna) o ID de um álbum.
 *
 * @param album Pointer para o álbum.
 * @return O ID do álbum (inteiro).
 */
gint get_album_id(Album *album);

/**
 * @brief Busca (retorna) o título de um álbum (tipo Album).
 *
 * @param album Pointer para o Album.
 * @return Um pointer para uma string com o título do álbum.
 */
gchar *get_album_title(Album *album);

/**
 * @brief Busca (retorna) o ano de lançamento de um álbum.
 *
 * @param album Pointer para o álbum.
 * @return O ano de lançamento do álbum (inteiro).
 */
gint get_album_year(Album *album);

/**
 * @brief Busca (retorna) a lista dos id dos artistas de um álbum.
 *
 * @param album Pointer para o álbum.
 * @return Um array de strings que contém os ids dos artistas do álbum.
 */
gchar **get_album_artist_ids(Album *album);

/**
 * @brief Cria uma nova entidade Album com os parâmetros passados.
 *
 * @param id O ID do álbum (inteiro).
 * @param title O título do album (pointer para string).
 * @param artist_ids Um array de strings com os IDs dos artistas.
 * @param year  O ano de lançamento do álbum (inteiro).
 * @return Um pointer para o Album criado.
 */
Album *create_album(gint id, gchar *title, gchar **artist_ids, gint year);

/**
 * @brief Initializa uma entidade Album.
 *
 * A função cria uma entidade Album vazia, ou seja, com os valores a 0 ou NULL nos campos.
 *
 * @return Um pointer para o Album inicializado.
 */
Album *inicializar_album();

/**
 * @brief Liberta a memória reservada a uma entidade Album.
 *
 * @param album Pointer para o Album a libertar.
 */
void free_album(Album *album);

#endif // ALBUM
