/**
 * @file valida.h
 * @brief Declara funções de validação para dados do sistema, incluindo usuários, músicas, artistas e outros elementos.
 */

#ifndef VALIDA_H
#define VALIDA_H

#include <glib.h>
#include "../include/gestores/gestor_sistemas.h"
#include "../include/gestores/gestor_musicas.h"
#include "../include/entidades/musica.h"
#include "../include/entidades/usuario.h"

/**
 * @brief Valida se a data está no formato "YYYY/MM/DD".
 * @param data String contendo a data a ser validada.
 * @return TRUE se o formato for válido, FALSE caso contrário.
 */
gboolean validarFormatoData(gchar *data);

/**
 * @brief Verifica se o mês e o dia fornecidos são válidos.
 * @param mes Mês (1-12).
 * @param dia Dia (1-31).
 * @return TRUE se os valores forem válidos, FALSE caso contrário.
 */
gboolean validarMesEDia(gint mes, gint dia);

/**
 * @brief Verifica se a data fornecida está no futuro em relação à data atual.
 * @param ano Ano a ser validado.
 * @param mes Mês a ser validado.
 * @param dia Dia a ser validado.
 * @return TRUE se a data não estiver no futuro, FALSE caso contrário.
 */
gboolean validarDataFutura(gint ano, gint mes, gint dia);

/**
 * @brief Valida a data de nascimento de um usuário.
 * @param usuario Ponteiro para o usuário cuja data será validada.
 * @return TRUE se a data for válida, FALSE caso contrário.
 */
gboolean validarDataUsuario(Usuario *usuario);

/**
 * @brief Valida o formato do e-mail de um usuário.
 * @param usuario Ponteiro para o usuário cujo e-mail será validado.
 * @return TRUE se o e-mail for válido, FALSE caso contrário.
 */
gboolean validaEmail(Usuario *usuario);

/**
 * @brief Valida o tipo de subscrição de um usuário (normal ou premium).
 * @param usuario Ponteiro para o usuário cuja subscrição será validada.
 * @return TRUE se a subscrição for válida, FALSE caso contrário.
 */
gboolean valida_subscricao(Usuario *usuario);

/**
 * @brief Valida se as músicas curtidas pelo usuário existem no sistema.
 * @param usuario Ponteiro para o usuário.
 * @param musicas Ponteiro para o gestor de músicas.
 * @return TRUE se todas as músicas existirem, FALSE caso contrário.
 */
gboolean valida_liked_musics_id(Usuario *usuario, GestorMusicas *musicas);

/**
 * @brief Valida se o usuário possui informações consistentes.
 * @param user Ponteiro para o usuário.
 * @param musics Ponteiro para o gestor de músicas.
 * @return TRUE se o usuário for válido, FALSE caso contrário.
 */
gboolean valida_user(Usuario *user, GestorMusicas *musics);

/**
 * @brief Verifica se uma string contém apenas números.
 * @param str String a ser validada.
 * @return TRUE se a string for numérica, FALSE caso contrário.
 */
gboolean tudoNum(gchar *str);

/**
 * @brief Valida a duração de uma música no formato "HH:MM:SS".
 * @param musica Ponteiro para a música cuja duração será validada.
 * @return TRUE se a duração for válida, FALSE caso contrário.
 */
gboolean validaDuracao(Musica *musica);

/**
 * @brief Valida se um artista individual não possui IDs de constituintes.
 * @param artista Ponteiro para o artista a ser validado.
 * @return TRUE se o artista for válido, FALSE caso contrário.
 */
gboolean valida_artista_individual(Artista *artista);

/**
 * @brief Valida se uma string representando uma lista começa e termina com colchetes.
 * @param string String a ser validada.
 * @return 1 se for válida, 0 caso contrário.
 */
int valida_parenteses_lista(gchar *string);

/**
 * @brief Valida se uma string representando um artista começa e termina com aspas simples.
 * @param artista String a ser validada.
 * @return 1 se for válida, 0 caso contrário.
 */
int valida_single_quotes_lista_artistas(gchar *artista);

/**
 * @brief Valida se uma string representando um álbum começa e termina com aspas simples.
 * @param album String a ser validada.
 * @return 1 se for válida, 0 caso contrário.
 */
int valida_single_quotes_lista_album(gchar *album);

/**
 * @brief Valida o ano de lançamento de uma música.
 * @param musica Ponteiro para a música cujo ano será validado.
 * @return 1 se o ano for válido, 0 caso contrário.
 */
int valida_ano_lançamento(Musica *musica);

/**
 * @brief Valida se os IDs de artistas associados a uma música existem no sistema.
 * @param musica Ponteiro para a música.
 * @param gestorartistas Ponteiro para o gestor de artistas.
 * @return 1 se todos os IDs forem válidos, 0 caso contrário.
 */
int valida_artistids_musica(Musica *musica, GestorArtistas *gestorartistas);

/**
 * @brief Valida se o álbum associado a uma música existe no sistema.
 * @param musica Ponteiro para a música.
 * @param gestor_albuns Ponteiro para o gestor de álbuns.
 * @return TRUE se o álbum for válido, FALSE caso contrário.
 */
gboolean valida_album(Musica *musica, GestorAlbuns *gestor_albuns);

/**
 * @brief Valida se a plataforma de acesso é válida (mobile ou desktop).
 * @param historico Ponteiro para o histórico cujo campo de plataforma será validado.
 * @return TRUE se a plataforma for válida, FALSE caso contrário.
 */
gboolean valida_plataforma(History *historico);

/**
 * @brief Valida o tipo de um artista (individual ou grupo).
 * @param artista Ponteiro para o artista cujo tipo será validado.
 * @return TRUE se o tipo for válido, FALSE caso contrário.
 */
gboolean valida_artista_tipo(Artista *artista);

#endif // VALIDA_H
