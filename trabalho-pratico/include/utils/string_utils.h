/**
 * @file string_utils.h
 * @brief Declarações de funções utilitárias para manipulação de strings.
 *
 * Este ficheiro contém funções para remoção de caracteres específicos
 * (como aspas, parênteses e colchetes) e para manipulação de strings
 * relacionadas a espaços em buffers.
 *
 */

#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <glib.h>

/**
 * @brief Remove aspas duplas no início e no fim de uma string GString.
 *
 * @param string Ponteiro para a string do tipo GString a ser modificada.
 */
void trim_quotes(GString *string);

/**
 * @brief Remove aspas simples no início e no fim de uma string GString.
 *
 * @param string Ponteiro para a string do tipo GString a ser modificada.
 */
void trim_singlequotes(GString *string);

/**
 * @brief Remove parenteses retos no início e no fim de uma string GString.
 *
 * @param string Ponteiro para a string do tipo GString a ser modificada.
 */
void trim_parenteses(GString *string);

/**
 * @brief Remove parenteses retos no início e no fim de uma string do tipo gchar.
 *
 * @param string Ponteiro para a string do tipo gchar a ser modificada.
 */
void trim_parenteses_gchar(gchar *string);

/**
 * @brief Remove aspas simples no início e no fim de uma string do tipo gchar.
 *
 * @param string Ponteiro para a string do tipo gchar a ser modificada.
 */
void trim_single_quotes_gchar(gchar *string);

/**
 * @brief Remove aspas duplas no início e no fim de uma string do tipo gchar.
 *
 * @param string Ponteiro para a string do tipo gchar a ser modificada.
 */
void trim_quotes_gchar(gchar *string);

/**
 * @brief Retorna a substring até o primeiro espaço em uma string.
 *
 * @param buffer Ponteiro para a string do tipo gchar.
 * @return Ponteiro para a nova string contendo a substring até o primeiro espaço.
 */
gchar *procura_espaço(gchar *buffer);

/**
 * @brief Retorna a substring entre o primeiro e o segundo espaço em uma string (ou final).
 *
 * @param buffer Ponteiro para a string do tipo gchar.
 * @return Ponteiro para a nova string contendo a substring entre o primeiro e o segundo espaço.
 */
gchar *procura_espaço2(gchar *buffer);

/**
 * @brief Retorna a substring a partir do segundo espaço em uma string.
 *
 * @param buffer Ponteiro para a string do tipo gchar.
 * @return Ponteiro para a nova string contendo a substring a partir do segundo espaço, ou NULL se não houver dois espaços.
 */
gchar *procura_espaço3(gchar *buffer);

/**
 * @brief Retorna a substring a partir do terceiro espaço em uma string.
 *
 * @param buffer Ponteiro para a string do tipo gchar.
 * @return Ponteiro para a nova string contendo a substring a partir do terceiro espaço, ou NULL se não houver três espaços.
 */
gchar *procura_espaço4(gchar *buffer);

#endif /* STRING_UTILS_H */