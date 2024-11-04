#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "../include/string_utils.h"

void trim_quotes(GString *string)
{ // função para retirar as aspas no inicio e fim de uma string
    if (string->len >= 2 && str->string[0] == '"' && str->string[str->len - 1] == '"')
    {
        g_string_erase(str, 0, 1);            // Tirar primeira aspa
        g_string_truncate(str, str->len - 1); // Tirar segunda aspa
    }
}

void trim_singlequotes(GString *string)
{ // função para retirar os ' no inicio e fim de uma string
    if (string->len >= 2 && str->string[0] == '\'' && str->string[str->len - 1] == '\'')
    {
        g_string_erase(str, 0, 1);            // Tirar primeiro '
        g_string_truncate(str, str->len - 1); // Tirar segundo '
    }
}