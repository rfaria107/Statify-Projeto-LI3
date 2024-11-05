#include "../include/parsing/rowreader.h"
#include "../include/parsing/string_utils.h"

RowReader *initialize_row_reader (char *line, char delimiter) {
 
 RowReader *rowReader = malloc (sizeof (struct rowReader));
   rowReader->row = line;
   rowReader->delimiter = delimiter;

return rowReader;
}


void reader_set_row(RowReader *reader, char *line) {
  reader->row = line;
}
char *reader_current_cell(RowReader *reader) { return g_strdup(reader->row);}

void *reader_next_cell (RowReader *reader) {
    char* start = reader->row;
    while (*reader-> row != reader->delimiter && *reader-> row!= '\0') {
        reader->row++;
    }
 
 if (reader->row == reader->delimiter) {
    reader->row = '\0';
    reader->row++;
 }
    return start;
}
void free_row_reader(RowReader *reader){
    free ( reader);
}