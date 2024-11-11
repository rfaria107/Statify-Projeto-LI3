#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "../include/parsing/string_utils.h"

void trim_quotes(GString *string)
{
    // função para retirar as aspas no início e fim de uma string
    if (string->len >= 2 && string->str[0] == '"' && string->str[string->len - 1] == '"')
    {
        g_string_erase(string, 0, 1);               // Tirar a primeira aspa
        g_string_truncate(string, string->len - 1); // Tirar a segunda aspa
    }
}

void trim_singlequotes(GString *string)
{ // função para retirar os ' no inicio e fim de uma string
    if (string->len >= 2 && string->str[0] == '\'' && string->str[string->len - 1] == '\'')
    {
        g_string_erase(string, 0, 1);               // Tirar primeiro '
        g_string_truncate(string, string->len - 1); // Tirar segundo '
    }
}

void trim_parenteses(GString *string)
{ // função para retirar os [] no inicio e fim de uma string
    if (string->len >= 2 && string->str[0] == '[' && string->str[string->len - 1] == ']')
    {
        g_string_erase(string, 0, 1);               // Tirar primeiro '
        g_string_truncate(string, string->len - 1); // Tirar segundo '
    }
}

void trim_parenteses_gchar(gchar *string)
{
    int len = strlen(string);
    if (len >= 2 && string[0] == '[' && string[len - 1] == ']')
    {
        memmove(string, string + 1, len - 2);
        string[len - 2] = '\0';
    }
}

void trim_single_quotes_gchar(gchar *string)
{
    int len = strlen(string);
    if (len >= 2 && string[0] == '\'' && string[len - 1] == '\'')
    {
        memmove(string, string + 1, len - 2);
        string[len - 2] = '\0';
    }
}

void trim_quotes_gchar(gchar *string)
{
    int len = strlen(string);
    if (len >= 2 && string[0] == '"' && string[len - 1] == '"')
    {
        memmove(string, string + 1, len - 2);
        string[len - 2] = '\0';
    }
}

// retorna a string até ao primeiro espaço
gchar *procura_espaço(gchar *buffer)
{
    int i = 0;
    gchar *string = g_strdup(buffer);
    while (string[i] != ' ' && string[i] != '\0')
    {
        i++;
    }
    string[i] = '\0';
    return string;
}

// retorna a string entre o primeiro e segundo espaços
gchar *procura_espaço2(gchar *buffer)
{
    int i = 0, start = 0;
    gchar *string = g_strdup(buffer);

    // Encontra o primeiro espaço
    while (string[i] != ' ' && string[i] != '\0')
    {
        i++;
    }

    // Define o início após o primeiro espaço
    if (string[i] == ' ')
    {
        start = i + 1;
        i++; // Avança para o próximo caractere
    }

    // Encontra o segundo espaço (ou fim da string)
    while (string[i] != ' ' && string[i] != '\0')
    {
        i++;
    }

    // Cria a substring do primeiro até o segundo espaço (ou até o fim)
    gchar *result = g_strndup(string + start, i - start);
    g_free(string);
    return result;
}

// retorna a string a partir do 2º espaço
gchar *procura_espaço3(gchar *buffer)
{
    int i = 0, space_count = 0;
    gchar *string = g_strdup(buffer);

    // Encontra o segundo espaço
    while (string[i] != '\0')
    {
        if (string[i] == ' ')
        {
            space_count++;
            if (space_count == 2)
            {
                i++; // Avança para o próximo caractere após o segundo espaço
                break;
            }
        }
        i++;
    }
    // se chegar ao final sem existir segundo espaço NULL deve ser retornado, de modo a, na query 2 não existir filtro de país
    if (space_count < 2)
    {
        g_free(string);
        return NULL;
    }
    // Cria a substring a partir do segundo espaço até o fim
    gchar *result = g_strdup(string + i);
    g_free(string);
    return result;
}