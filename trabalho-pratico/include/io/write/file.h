/**
 * @file file.h
 * @brief Header do módulo FileBuffer, utilizado pelo RowWriter.
 *
 * Este módulo fornece uma interface para manipular buffers de escrita em ficheiros
 */

#ifndef FILE_H
#define FILE_H

/**
 * @typedef FileBuffer
 * @brief Estrutura opaca que representa um buffer de escrita num ficheiro.
 *
 * A estrutura FileBuffer é usada para as operações de escrita num ficheiro pelo writer.
 */
typedef struct FileBuffer FileBuffer;

/**
 * @brief Inicializa um buffer de escrita para o ficheiro especificado.
 *
 * Esta função cria e configura uma estrutura FileBuffer associada a um ficheiro.
 * O ficheiro será aberto no modo de escrita ("w").
 *
 * @param file_name Nome do ficheiro a ser aberto.
 * @return Ponteiro para a estrutura FileBuffer criada, ou NULL em caso de erro.
 */
FileBuffer *initialize_file_buffer(const char *file_name);

/**
 * @brief Adiciona uma linha ao buffer de escrita.
 *
 * Esta função escreve uma linha no ficheiro associado ao buffer.
 *
 * @param buffer Ponteiro para a estrutura FileBuffer.
 * @param line String contendo a linha a ser escrita no ficheiro.
 */
void append_to_file_buffer(FileBuffer *buffer, const char *line);

/**
 * @brief Escreve o conteúdo do buffer no ficheiro.
 *
 * Esta função força a escrita do conteúdo do buffer para o ficheiro.
 *
 * @param buffer Ponteiro para a estrutura FileBuffer.
 */
void write_file_buffer(FileBuffer *buffer);

/**
 * @brief Libera os recursos associados a um buffer de escrita.
 *
 * Fecha o ficheiro associado e libera a memória alocada para a estrutura FileBuffer.
 *
 * @param buffer Ponteiro para a estrutura FileBuffer a ser liberada.
 */
void free_file_buffer(FileBuffer *buffer);

#endif // FILE_H
