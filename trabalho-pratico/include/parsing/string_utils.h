#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <glib.h>
#include <stdio.h>
#include <string.h>

void trim_quotes(GString *string);
void trim_singlequotes(GString *string);

#endif