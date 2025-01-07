/**
 * @file utils.h
 * @brief Declarações de funções utilitárias para manipulação de durações, datas e arredondamentos.
 *
 * Este ficheiro contém funções para conversão de durações, arredondamento de valores, comparação
 * de dados, e cálculos relacionados a datas específicas (como domingos e sábados).
 *
 */

#ifndef UTILS_H
#define UTILS_H

#include <glib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Converte uma string de duração no formato "HH:MM:SS" para o total em segundos.
 *
 * @param duracao String representando a duração no formato "HH:MM:SS".
 * @return Total de segundos correspondente à duração.
 */
gint duracao_para_segundos(const gchar *duracao);

/**
 * @brief Converte um total de segundos para uma string no formato "HH:MM:SS".
 *
 * @param total_segundos Total de segundos a ser convertido.
 * @return String representando a duração no formato "HH:MM:SS".
 */
gchar *segundos_para_duracao(gint total_segundos);

/**
 * @brief Arredonda um valor de ponto flutuante para um número específico de casas decimais.
 *
 * @param valor Valor a ser arredondado.
 * @param casas Número de casas decimais desejado.
 * @return Valor arredondado para o número de casas decimais especificado.
 */
double arredondar_para_casas_decimais(double valor, int casas);

/**
 * @brief Compara a duração total de discografias de dois artistas.
 *
 * A comparação é feita com base na duração total dos discos. Em caso de empate,
 * a comparação é feita com base na ordem alfabética dos nomes dos artistas.
 *
 * @param a Ponteiro para o primeiro artista.
 * @param b Ponteiro para o segundo artista.
 * @return 1 se `a` tiver menor duração que `b`, -1 se `a` tiver maior duração que `b`,
 *         ou o resultado da comparação alfabética em caso de empate.
 */
gint compara_duracoes_discografia(gconstpointer a, gconstpointer b);

/**
 * @brief Calcula os domingos correspondentes à data inicial e final fornecidas.
 *
 * @param data_inicial String representando a data inicial no formato "YYYY/MM/DD".
 * @param data_final String representando a data final no formato "YYYY/MM/DD".
 * @param domingo_inicial Ponteiro para armazenar o domingo correspondente à data inicial.
 * @param domingo_final Ponteiro para armazenar o domingo correspondente à data final.
 */
void calcular_domingos_inicial_e_final(char *data_inicial, char *data_final, char **domingo_inicial, char **domingo_final);

/**
 * @brief Calcula o domingo da semana correspondente a uma data fornecida.
 *
 * @param data String representando a data no formato "YYYY/MM/DD".
 * @return String representando o domingo correspondente no formato "YYYY/MM/DD".
 */
gchar *calcular_domingo(const gchar *data);

/**
 * @brief Calcula o sábado seguinte à data fornecida.
 *
 * @param data String representando a data no formato "YYYY/MM/DD".
 * @return String representando o sábado seguinte no formato "YYYY/MM/DD".
 */
gchar *calcular_sabado_seguinte(const gchar *data);

/**
 * @brief Converte uma data no formato "YYYY/MM/DD" para um timestamp Unix.
 *
 * @param data String representando a data no formato "YYYY/MM/DD".
 * @return Timestamp Unix correspondente à data, ou -1 em caso de erro.
 */
time_t get_timestamp(const char *data);

#endif /* UTILS_H */
