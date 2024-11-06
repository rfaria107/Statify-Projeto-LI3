// row_reader.h

#ifndef ROW_READER_H
#define ROW_READER_H

#include <stdio.h>

#define DELIMITER ';' // Delimitador para o CSV

typedef struct rowReader RowReader;

// Função para inicializar o RowReader
RowReader *initialize_row_reader(char *line, char delimiter);

//Define a linha atual do rowReader
void reader_set_row(RowReader *reader, char *line);

// Função para ler a próxima célula
char *reader_next_cell(RowReader *reader);

// Função para liberar a memória do RowReader
void free_row_reader(RowReader *reader);

#endif // ROW_READER_H
