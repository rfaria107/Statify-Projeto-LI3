#ifndef QUERIES_H
#define QUERIES_H

#include <glib.h>
#include <stdio.h>
#include "../include/gestores/gestor_sistemas.h"

void interpreter_inputs(FILE *reader, GestorSistema *gestorsis);
void querie_1(GestorUsuarios *gestor, const char *username, int line);

#endif