#ifndef ROW_READER_H
#define ROW_READER_H

#include <stdio.h>

#define DELIMITER ';' // Delimitador para o CSV

typedef struct rowReader RowReader;

RowReader *initialize_row_reader(const char *line, char delimiter);
void reset_row(RowReader *reader, const char *line);
char *get_current_cell(const RowReader *reader);
char *get_next_cell(RowReader *reader);
void free_row_content(RowReader *reader);
void free_row_reader(RowReader *reader);

#endif // ROW_READER_H
