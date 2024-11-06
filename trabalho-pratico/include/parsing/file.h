#include <stdio.h>

typedef struct {
  FILE *file;
  char *buffer;
} FileBuffer;

FileBuffer *initialize_file_buffer(const char *file_name);

void append_to_file_buffer(FileBuffer *buffer, const char *line);

void append_to_file_buffer(FileBuffer *buffer, const char *line);

void write_file_buffer(FileBuffer *buffer);

