#include "../include/parsing/rowreader.h"
#include "../include/parsing/string_utils.h"

typedef struct rowReader
{

  char *row;
  char delimiter;
} RowReader;

RowReader *initialize_row_reader(char *line, char delimiter)
{

  RowReader *rowReader = malloc(sizeof(struct rowReader));
  if (!rowReader)
    return NULL;
  if (line != NULL)
  {
    rowReader->row = line;
  }
  else
  {
    rowReader->row = g_strdup("");
  }
  rowReader->delimiter = delimiter;

  return rowReader;
}

void reader_set_row(RowReader *reader, char *line)
{
  if (reader != NULL)
    free_row(reader);
  reader->row = line;
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

void free_row(RowReader *reader) // a row vai ser alterada, assim quando precisar de dar free coloco-a de volta no inicio para dar free à linha inteira
{
  if (reader != NULL)
  g_free(reader->row);
}

void free_row_reader(RowReader *reader)
{
  if (reader != NULL)
  {
    free_row(reader);
    g_free(reader);
  }
}