#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/write/file.h"

FileBuffer *initialize_file_buffer(const char *file_name) {
  FileBuffer *buffer = malloc(sizeof(FileBuffer));
  buffer->file = fopen(file_name, "w");
  if (!buffer->file) {
    // Lidar com erro de abertura de arquivo
    free(buffer);
    return NULL;
  }
  buffer->buffer = malloc(1024); // (Maybe desnecessária)
  return buffer;
}

void append_to_file_buffer(FileBuffer *buffer, const char *line) { //Usada no writter

  fwrite(line, sizeof(char), strlen(line), buffer->file);
}

void write_file_buffer(FileBuffer *buffer) { // Usada no writer (garante que tudo foi escrito)
  fflush(buffer->file);
}

void free_file_buffer(FileBuffer *buffer) {
  fclose(buffer->file);
  free(buffer->buffer);
  free(buffer);
}
