#include "../include/parsing/rowreader.h"
#include "../include/parsing/string_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

typedef struct rowReader {
    char *row;
    char delimiter;
    char *original_row;
    int cell_count;
    int current_cell;
} RowReader;

// Initializes RowReader with a line and delimiter
RowReader *initialize_row_reader(const char *line, char delimiter) {
    RowReader *reader = malloc(sizeof(RowReader));
    if (!reader)
        return NULL;

    reader->delimiter = delimiter;
    reader->cell_count = 0;
    reader->current_cell = 0;

    if (line) {
        reader->original_row = g_strdup(line);
        reader->row = g_strdup(line);
    } else {
        reader->original_row = g_strdup("");
        reader->row = g_strdup("");
    }

    // Count the number of cells (fields) in the line
    char *temp = reader->row;
    while (*temp != '\0') {
        if (*temp == delimiter) {
            reader->cell_count++;
        }
        temp++;
    }
    reader->cell_count++;  // Include the last field after the last delimiter (or single line)

    return reader;
}

// Sets a new row in the reader and resets reading index
void reset_row(RowReader *reader, const char *line) {
    if (!reader)
        return;

    free_row_content(reader);  // Free the current row content

    reader->original_row = g_strdup(line);
    reader->row = g_strdup(line);

    // Reset cell count and reading index
    reader->cell_count = 0;
    reader->current_cell = 0;

    // Count the number of cells in the new line
    char *temp = reader->row;
    while (*temp != '\0') {
        if (*temp == reader->delimiter) {
            reader->cell_count++;
        }
        temp++;
    }
    reader->cell_count++;
}

// Gets the current cell without advancing the index
char *get_current_cell(const RowReader *reader) {
    if (!reader || reader->row == NULL) {
        return NULL;
    }
    return g_strdup(reader->row);
}

// Gets the next cell and advances the reading index
char *get_next_cell(RowReader *reader) {
    if (!reader || reader->row == NULL || reader->current_cell >= reader->cell_count) {
        return NULL;
    }

    char *start = reader->row;
    while (*reader->row != reader->delimiter && *reader->row != '\0') {
        reader->row++;
    }

    if (*reader->row == reader->delimiter) {
        *reader->row = '\0';  // End the current cell
        reader->row++;        // Move to the next character
    }

    reader->current_cell++;

    // Here, we return a newly allocated string, but it should be freed after usage.
    return g_strdup(start);
}


// Frees the current row content and resets to the original row
void free_row_content(RowReader *reader) {
    if (!reader)
        return;

    g_free(reader->row);
    reader->row = g_strdup(reader->original_row);
}

void free_row_reader(RowReader *reader) {
    if (reader) {
        free_row_content(reader);
        g_free(reader->original_row); // Liberar a memória da linha original
        g_free(reader); // Liberar a memória do próprio objeto RowReader
    }
}
