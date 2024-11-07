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
