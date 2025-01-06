#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <glib.h>

void trim_quotes(GString *string);
void trim_singlequotes(GString *string);
void trim_parenteses(GString *string);
void trim_parenteses_gchar(gchar *string);
void trim_single_quotes_gchar(gchar *string);
gchar *procura_espaço(gchar *buffer);
gchar *procura_espaço2(gchar *buffer);
gchar *procura_espaço3(gchar *buffer);
gchar *procura_espaço4(gchar *buffer);
void trim_quotes_gchar(gchar *string);

#endif