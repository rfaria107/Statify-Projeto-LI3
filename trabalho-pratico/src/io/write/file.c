#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "../include/io/write/file.h"

FileBuffer *initialize_file_buffer(const char *file_name)
{
    FileBuffer *buffer = malloc(sizeof(FileBuffer));
    if (!buffer)
    {
        fprintf(stderr, "Erro ao alocar memória para FileBuffer\n");
        return NULL;
    }

    buffer->file = fopen(file_name, "w");
    if (!buffer->file)
    {
        fprintf(stderr, "Erro ao abrir o arquivo: %s\n", file_name);
        free(buffer);
        return NULL;
    }

    return buffer;
}

void append_to_file_buffer(FileBuffer *buffer, const char *line)
{
    if (!buffer || !buffer->file)
    {
        fprintf(stderr, "Erro: buffer ou buffer->file é NULL\n");
        return;
    }
    fwrite(line, sizeof(char), strlen(line), buffer->file);
}

void write_file_buffer(FileBuffer *buffer)
{
    if (!buffer || !buffer->file)
    {
        fprintf(stderr, "Erro: buffer ou buffer->file é NULL\n");
        return;
    }
    fflush(buffer->file);
}

void free_file_buffer(FileBuffer *buffer)
{
    if (!buffer)
        return;

    if (buffer->file)
        fclose(buffer->file);
    free(buffer);
}
