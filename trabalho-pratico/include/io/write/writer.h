/**
 * @file writer.h
 * @brief Header do módulo RowWriter para escrita formatada em ficheiros.
 *
 * Este módulo fornece ferramentas para manipular ficheiros de saída com formatação de linhas,
 * incluindo suporte para modos CSV e tabela, além de funcionalidades específicas para registo de erros.
 */

#ifndef WRITER_H
#define WRITER_H

#include <stdarg.h>


/**
 * @typedef RowWriter
 * @brief Estrutura opaca para escrita formatada em ficheiros.
 *
 * A estrutura RowWriter encapsula os detalhes de implementação para manipulação de ficheiros
 * de saída, incluindo formatação e cabeçalhos.
 */
typedef struct RowWriter RowWriter;

/**
 * @brief Inicializa um RowWriter para escrever num ficheiro de saída.
 *
 * Cria e configura uma estrutura RowWriter associada a um ficheiro de saída, no modo especificado.
 *
 * @param output_file_name Nome do ficheiro de saída.
 * @param writeMode Modo de escrita (CSV ou tabela).
 * @return Ponteiro para a estrutura RowWriter criada, ou NULL em caso de erro.
 */
RowWriter *initialize_row_writer(const char *output_file_name, int writeMode);

/**
 * @brief Inicializa um RowWriter para o ficheiro de erros.
 *
 * Cria um RowWriter específico para registo de erros em formato CSV.
 *
 * @param error_file_name Nome do ficheiro de erros.
 * @return Ponteiro para a estrutura RowWriter criada, ou NULL em caso de erro.
 */
RowWriter *initialize_error_writer(const char *error_file_name);

/**
 * @brief Regista uma linha de erro no ficheiro de erros.
 *
 * Escreve uma linha de erro no ficheiro associado ao RowWriter de erros.
 *
 * @param error_writer Ponteiro para o RowWriter de erros.
 * @param error_line Linha de erro a ser registada.
 */
void log_error(RowWriter *error_writer, const char *error_line);

/**
 * @brief Escreve uma linha formatada no ficheiro associado ao RowWriter.
 *
 * Permite escrever múltiplos campos formatados numa linha, separados pelo caractere especificado.
 *
 * @param writer Ponteiro para o RowWriter.
 * @param separator Caractere separador entre os campos.
 * @param fields Número de campos a serem escritos.
 * @param ... Lista de argumentos contendo os valores dos campos.
 */
void write_row(RowWriter *writer, char separator, int fields, ...);

/**
 * @brief Define os nomes dos campos para o RowWriter.
 *
 * Configura os nomes dos campos que serão usados como cabeçalhos ou referência para a escrita.
 *
 * @param writer Ponteiro para o RowWriter.
 * @param field_names Array de strings com os nomes dos campos.
 * @param fields Número de campos no array.
 */
void row_writer_set_field_names(RowWriter *writer, char **field_names, int fields);

/**
 * @brief Define a formatação dos campos para o RowWriter.
 *
 * Configura o formato de cada campo, utilizando strings de formatação.
 *
 * @param writer Ponteiro para o RowWriter.
 * @param formatting Array de strings com as formatações de cada campo.
 */
void row_writer_set_formatting(RowWriter *writer, char **formatting);

/**
 * @brief Escreve o cabeçalho padrão para erros relacionados a artistas.
 *
 * Configura e escreve os nomes dos campos de cabeçalho para o ficheiro de erros de artistas.
 *
 * @param error_writer Ponteiro para o RowWriter de erros.
 */
void escrever_cabecalho_artists_erro(RowWriter *error_writer);

/**
 * @brief Escreve o cabeçalho padrão para erros relacionados a músicas.
 *
 * Configura e escreve os nomes dos campos de cabeçalho para o ficheiro de erros de músicas.
 *
 * @param error_writer Ponteiro para o RowWriter de erros.
 */
void escrever_cabecalho_musics_erro(RowWriter *error_writer);

/**
 * @brief Escreve o cabeçalho padrão para erros relacionados a usuários.
 *
 * Configura e escreve os nomes dos campos de cabeçalho para o ficheiro de erros de usuários.
 *
 * @param error_writer Ponteiro para o RowWriter de erros.
 */
void escrever_cabecalho_users_erro(RowWriter *error_writer);

/**
 * @brief Escreve o cabeçalho padrão para erros relacionados ao histórico.
 *
 * Configura e escreve os nomes dos campos de cabeçalho para o ficheiro de erros de histórico.
 *
 * @param error_writer Ponteiro para o RowWriter de erros.
 */
void escrever_cabecalho_history_erro(RowWriter *error_writer);

/**
 * @brief Escreve o cabeçalho padrão para erros relacionados a álbuns.
 *
 * Configura e escreve os nomes dos campos de cabeçalho para o ficheiro de erros de álbuns.
 *
 * @param error_writer Ponteiro para o RowWriter de erros.
 */
void escrever_cabecalho_album_erro(RowWriter *error_writer);

/**
 * @brief Finaliza a escrita e liberta os recursos associados ao RowWriter.
 *
 * Garante que todo o conteúdo do buffer seja escrito no ficheiro e liberta a memória alocada.
 *
 * @param writer Ponteiro para o RowWriter a ser libertado.
 */
void free_and_finish_writing(RowWriter *writer);

/**
 * @brief Imprime uma linha formatada no terminal (para o modo interativo).
 *
 * Permite visualizar uma linha formatada com múltiplos campos diretamente no terminal.
 *
 * @param writer Ponteiro para o RowWriter.
 * @param separator Caractere separador entre os campos.
 * @param fields Número de campos a serem escritos.
 * @param ... Lista de argumentos contendo os valores dos campos.
 */
void print_row(RowWriter *writer, char separator, int fields, ...);

#endif // WRITER_H
