#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "../include/parsing/string_utils.h"

void trim_quotes(GString *string) {
    // função para retirar as aspas no início e fim de uma string
    if (string->len >= 2 && string->str[0] == '"' && string->str[string->len - 1] == '"') {
        g_string_erase(string, 0, 1);            // Tirar a primeira aspa
        g_string_truncate(string, string->len - 1); // Tirar a segunda aspa
    }
}
