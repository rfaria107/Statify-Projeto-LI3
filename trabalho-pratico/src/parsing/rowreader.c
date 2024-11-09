#include "../include/parsing/rowreader.h"
#include "../include/parsing/string_utils.h"

typedef struct rowReader
{

  char *row;
  char delimiter;
  char *row_original;
} RowReader;

RowReader *initialize_row_reader(char *line, char delimiter)
{

  RowReader *rowReader = malloc(sizeof(struct rowReader));
  if (!rowReader)
    return NULL;
  if (line != NULL)
  {
    rowReader->row_original = g_strdup(line);
    rowReader->row = g_strdup(line);
  }
  else
  {
    rowReader->row_original = g_strdup("");
    rowReader->row = g_strdup("");
  }
  rowReader->delimiter = delimiter;

  return rowReader;
}

void reader_set_row(RowReader *reader, char *line)
{
  if (reader == NULL)
    return;
  free_row(reader);
  reader->row_original = g_strdup(line);
  reader->row = g_strdup(line);
}

char *reader_current_cell(RowReader *reader)
{
  return g_strdup(reader->row);
}

char *reader_next_cell(RowReader *reader)
{
  char *start = reader->row;
  while (*reader->row != reader->delimiter && *reader->row != '\0')
  {
    reader->row++;
  }

  if (*reader->row == reader->delimiter)
  {
    *reader->row = '\0';
    reader->row++;
  }
  return start;
}

void free_row(RowReader *reader)//a row vai ser alterada, assim quando precisar de dar free coloco-a de volta no inicio para dar free à linha inteira
{
    if (reader == NULL)
        return;
    reader->row = g_strdup(reader->row_original); //colocar a linha de volta no inicio
    g_free(reader->row);
    g_free(reader->row_original);          
}

void free_row_reader(RowReader *reader)
{
  if (reader != NULL)
  {
    free_row(reader);
    g_free(reader);
  }
}