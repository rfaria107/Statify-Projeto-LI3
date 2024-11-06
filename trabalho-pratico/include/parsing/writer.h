#ifndef WRITER_H
#define WRITER_H

#include <stdarg.h>

typedef enum { WRITE_MODE_CSV, WRITE_MODE_TABLE } WriteMode;

typedef struct RowWriter RowWriter;

// Inicializa um RowWriter para escrever um arquivo de saída específico
RowWriter *initialize_row_writer(const char *output_file_name, WriteMode writeMode);

// Inicializa um RowWriter para o arquivo de erros
RowWriter *initialize_error_writer(const char *error_file_name);

// Escreve valores formatados para o arquivo especificado no RowWriter
void write_row(RowWriter *writer, int fields, ...);

// Define os nomes e formatação dos campos para a escrita
void row_writer_set_field_names(RowWriter *writer, char **field_names, int fields);
void row_writer_set_formatting(RowWriter *writer, char **formatting);

// Escreve o conteúdo do buffer no arquivo e libera o RowWriter
void free_and_finish_writing(RowWriter *writer);

#endif