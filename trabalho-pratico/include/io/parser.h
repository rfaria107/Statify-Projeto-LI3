/**
 * @file parser.h
 * @brief Header do módulo Parser.
 *
 * Este módulo fornece funções para interpretar e processar ficheiros CSV,
 * preenchendo os gestores correspondentes com as entidades (Artista, Música, Usuário, Álbum e Histórico).
 * Ele utiliza as estruturas e gestores definidos para organizar e gerir os dados lidos.
 */

#ifndef PARSER_H
#define PARSER_H

#include <glib.h>
#include <stdio.h>
#include "../include/gestores/gestor_sistemas.h"
#include "../include/gestores/gestor_artistas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/gestores/gestor_albuns.h"

/**
 * @brief Função principal do parser para processar um ficheiro CSV.
 *
 * Esta função lê as linhas do ficheiro CSV e, dependendo do tipo especificado,
 * preenche os gestores correspondentes com as entidades processadas.
 *
 * @param file Ponteiro para o ficheiro a ser processado.
 * @param gestor Ponteiro para o GestorSistema onde os dados serão armazenados.
 * @param tipo Caractere que especifica o tipo de entidade a ser processada ('A' para artistas, 'M' para músicas, etc.).
 */
void parser_principal(FILE *file, GestorSistema *gestor, char tipo);

/**
 * @brief Processa uma linha de CSV para criar uma entidade Artista.
 *
 * @param line Linha do ficheiro CSV que contém os dados do artista.
 * @return Ponteiro para a entidade Artista criada.
 */
Artista *parse_csv_line_artista(gchar *line);

/**
 * @brief Preenche uma entidade Artista com os campos fornecidos.
 *
 * @param campostemp Array com os campos extraídos da linha CSV.
 * @return Ponteiro para a entidade Artista preenchida.
 */
Artista *preenche_artista(GPtrArray *campostemp);

/**
 * @brief Processa uma linha de CSV para criar uma entidade Música.
 *
 * @param line Linha do ficheiro CSV que contém os dados da música.
 * @param gestorartistas Ponteiro para o GestorArtistas para buscar informações de artistas (validação).
 * @param gestoralbuns Ponteiro para o GestorAlbuns para buscar informações de álbuns (validação).
 * @return Ponteiro para a entidade Música criada.
 */
Musica *parse_csv_line_musica(gchar* line, GestorArtistas *gestorartistas, GestorAlbuns *gestoralbuns);

/**
 * @brief Preenche uma entidade Música com os campos fornecidos.
 *
 * @param campostemp Array que contém os campos extraídos da linha CSV.
 * @param gestorartistas Ponteiro para o GestorArtistas para buscar informações de artistas (validação).
 * @param gestoralbuns Ponteiro para o GestorAlbuns para buscar informações de álbuns (validação).
 * @return Ponteiro para a entidade Música preenchida.
 */
Musica *preenche_musica(GPtrArray *campostemp, GestorArtistas *gestorartistas, GestorAlbuns *gestoralbuns);

/**
 * @brief Processa uma linha de CSV para criar uma entidade Usuário.
 *
 * @param line Linha do ficheiro CSV que contém os dados do usuário.
 * @param gestormusicas Ponteiro para o GestorMusicas para buscar informações de músicas (validação).
 * @return Ponteiro para a entidade Usuário criada.
 */
Usuario *parse_csv_line_usuario(gchar* line, GestorMusicas *gestormusicas);

/**
 * @brief Preenche uma entidade Usuário com os campos fornecidos.
 *
 * @param campostemp Array que contém os campos extraídos da linha CSV.
 * @param gestormusicas Ponteiro para o GestorMusicas para buscar informações de músicas (validação).
 * @return Ponteiro para a entidade Usuário preenchida.
 */
Usuario *preenche_usuario(GPtrArray *campostemp, GestorMusicas *gestormusicas);

/**
 * @brief Processa uma linha de CSV para criar uma entidade Álbum.
 *
 * @param line Linha do ficheiro CSV que contém os dados do álbum.
 * @param gestoralbuns Ponteiro para o GestorAlbuns para gerir os álbuns.
 * @return Ponteiro para a entidade Álbum criada.
 */
Album *parse_csv_line_album(gchar *line, GestorAlbuns *gestoralbuns);

/**
 * @brief Preenche uma entidade Álbum com os campos fornecidos.
 *
 * @param campostemp Array que contém os campos extraídos da linha CSV.
 * @param gestoralbuns Ponteiro para o GestorAlbuns para gerir os álbuns.
 * @return Ponteiro para a entidade Álbum preenchida.
 */
Album *preenche_album(GPtrArray *campostemp, GestorAlbuns *gestoralbuns);

/**
 * @brief Processa uma linha de CSV para criar uma entidade Histórico (History).
 *
 * @param line Linha do ficheiro CSV que contém os dados do histórico.
 * @param gestorhistory Ponteiro para o GestorHistories para gerir os históricos.
 * @return Ponteiro para a entidade History criada.
 */
History *parse_csv_line_history(gchar *line, GestorHistories *gestorhistory);

/**
 * @brief Preenche uma entidade Histórico (History) com os campos fornecidos.
 *
 * @param campostemp Array que contém os campos extraídos da linha CSV.
 * @param gestorhistory Ponteiro para o GestorHistories para gerir os históricos.
 * @return Ponteiro para a entidade History preenchida.
 */
History *preenche_history(GPtrArray *campostemp, GestorHistories *gestorhistory);

#endif // PARSER_H
