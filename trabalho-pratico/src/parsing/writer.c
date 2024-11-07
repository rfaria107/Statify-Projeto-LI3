#include "../include/parsing/writer.h"
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/parsing/file.h"

#define BUFFER_SIZE 1024
#define VALUE_BUFFER_SIZE 128

struct RowWriter {
    char **field_names;
    char **format;
    int row;
    char fields;
    FileBuffer *buffer;
    WriteMode mode;
};


// Inicializa o RowWriter para um arquivo de saída específico
RowWriter *initialize_row_writer(const char *output_file_name, WriteMode writeMode) {
    RowWriter *writer = malloc(sizeof(RowWriter));
    writer->field_names = NULL;
    writer->format = NULL;
    writer->fields = 0;
    writer->row = 0;
    writer->buffer = initialize_file_buffer(output_file_name);
    writer->mode = writeMode;
    return writer;
}

// Inicializa o RowWriter para o arquivo de erros
RowWriter *initialize_error_writer(const char *error_file_name) {
    return initialize_row_writer(error_file_name, WRITE_MODE_CSV);
}

// Define os nomes dos campos e a formatação
void row_writer_set_field_names(RowWriter *writer, char **field_names, int fields) {
    writer->field_names = malloc(fields * sizeof(char *));
    for (int i = 0; i < fields; i++) {
        writer->field_names[i] = strdup(field_names[i]);
    }
    writer->fields = (char)fields;
}

// Formata uma linha (os campos serao posteriormente passados)
// Ex:  *char *formats[] = {"%s", "%d", "%.2f"};
void row_writer_set_formatting(RowWriter *writer, char **formatting) {
    writer->format = formatting;
}

// Função para escrever dados em formato CSV
void write_row(RowWriter *writer, int fields, ...) {
    va_list args;
    va_start(args, fields); // Inicializa a manipulação dos argumentos variáveis

    char line[BUFFER_SIZE] = "";
    char formatting[BUFFER_SIZE] = "";

    // Adiciona os formatos para cada campo, separados por ponto e vírgula
    for (int i = 0; i < fields - 1; i++) {
        strcat(formatting, writer->format[i]);
        strcat(formatting, ";"); // Adiciona o ";" que caracteriza o ficheiro .csv
    }
    
    strcat(formatting, writer->format[fields - 1]);
    strcat(formatting, "\n");

    vsnprintf(line, BUFFER_SIZE, formatting, args);

    writer->row++;
    append_to_file_buffer(writer->buffer, line); // Chama append_to_file_buffer para escrever a linha no arquivo

    va_end(args); // Finaliza a manipulação dos argumentos variáveis
}

void log_error(RowWriter *error_writer, const char *error_line) {
    append_to_file_buffer(error_writer->buffer, error_line);
    append_to_file_buffer(error_writer->buffer, "\n");  // Adiciona nova linha ao final do erro
}

// Escreve o conteúdo do buffer no arquivo e libera o RowWriter
void free_and_finish_writing(RowWriter *writer) {
    write_file_buffer(writer->buffer); // Garante que tudo seja escrito no arquivo antes de fechá-lo (duvidas)

    free_file_buffer(writer->buffer);

    // Libera os nomes dos campos, se foram alocados
    if (writer->field_names) {
        for (int i = 0; i < writer->fields; i++) {
            free(writer->field_names[i]);
        }
        free(writer->field_names);
    }

    free(writer);
}

