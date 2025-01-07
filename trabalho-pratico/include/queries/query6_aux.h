/**
 * @file query6_aux.h
 * @brief Declarações de funções auxiliares da query 6.
 *
 * Este ficheiro contém as definições das funções e estruturas utilizadas para processar dados,
 * aplicar critérios de desempate e ordenar tabelas de hash para a query 6.
 *
 */

#ifndef QUERY6_AUX_H
#define QUERY6_AUX_H

#include <glib.h>

/**
 * @struct UserData
 * @brief Estrutura para armazenar dados auxiliares utilizados em operações de ordenação e desempate.
 *
 */
typedef struct UserData UserData;

/**
 * @brief Cria uma instância da estrutura UserData.
 *
 * Esta função inicializa uma estrutura UserData com os parâmetros fornecidos.
 *
 * @param table Ponteiro para a tabela de hash associada.
 * @param reverse Indica se a ordem deve ser invertida (TRUE para ordem decrescente).
 * @param alphabetical Indica se o critério de desempate é alfabético.
 * @return Ponteiro para a nova estrutura UserData ou NULL em caso de falha na alocação de memória.
 */
UserData *create_user_data(GHashTable *table, gboolean reverse, gboolean alphabetical);

/**
 * @brief Liberta a memória associada a uma estrutura UserData.
 *
 * @param user_data Ponteiro para a estrutura UserData a ser libertada.
 */
void free_user_data(UserData *user_data);

/**
 * @brief Encontra a entrada com o maior valor numa tabela de hash com critério de desempate.
 *
 * Esta função percorre uma tabela de hash e encontra a chave correspondente ao maior valor.
 * Em caso de empate, utiliza a ordem alfabética como critério de desempate.
 *
 * @param table Tabela de hash a ser analisada.
 * @param alphabetical Indica se o critério de desempate é alfabético.
 * @return A chave correspondente à entrada com o maior valor.
 */
gint find_top_entry_with_tiebreaker(GHashTable *table, gboolean alphabetical);

/**
 * @brief Encontra a entrada com o maior valor numa tabela de hash de strings com critério de desempate.
 *
 * Similar a `find_top_entry_with_tiebreaker`, mas opera em tabelas de hash onde as chaves são strings.
 *
 * @param table Tabela de hash a ser analisada.
 * @param alphabetical Indica se o critério de desempate é alfabético.
 * @return String correspondente à entrada com o maior valor.
 */
gchar *find_top_entry_with_tiebreaker_str(GHashTable *table, gboolean alphabetical);

/**
 * @brief Encontra o dia com o maior número de ocorrências numa tabela de hash.
 *
 * Esta função analisa uma tabela de hash onde as chaves representam dias e os valores representam contagens.
 * Em caso de empate, escolhe o dia mais recente.
 *
 * @param day_count Tabela de hash com os dias e suas contagens.
 * @return String correspondente ao dia com o maior número de ocorrências.
 */
gchar *find_top_day_with_tiebreaker(GHashTable *day_count);

/**
 * @brief Ordena uma tabela de hash com base nos valores e aplica critérios de desempate.
 *
 * Esta função converte as chaves da tabela de hash numa lista ordenada com base nos valores associados.
 * Utiliza critérios de desempate adicionais fornecidos por uma tabela de hash auxiliar.
 *
 * @param table Tabela de hash a ser ordenada.
 * @param ascending Indica se a ordenação deve ser crescente (TRUE) ou decrescente.
 * @param tie_breaker Indica se deve aplicar o critério de desempate.
 * @param artist_time Tabela de hash auxiliar utilizada no critério de desempate.
 * @return Lista ordenada das chaves da tabela de hash.
 */
GList *sort_hash_table_by_value_with_tiebreaker(GHashTable *table, gboolean ascending, gboolean tie_breaker, GHashTable *artist_time);

/**
 * @brief Compara valores de artistas para ordenação.
 *
 * Função de comparação utilizada para ordenar artistas com base no tempo associado.
 * Em caso de empate no tempo, utiliza os IDs dos artistas como critério de desempate.
 *
 * @param a Ponteiro para o primeiro artista a ser comparado.
 * @param b Ponteiro para o segundo artista a ser comparado.
 * @param user_data Ponteiro para a tabela de hash que contém os tempos associados aos artistas.
 * @return Valor negativo se `a` for menor que `b`, positivo se `a` for maior que `b`, ou zero se forem iguais.
 */
gint compare_artist_values(gconstpointer a, gconstpointer b, gpointer user_data);

#endif /* QUERY6_AUX_H */
