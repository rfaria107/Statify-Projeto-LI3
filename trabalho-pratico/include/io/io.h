#ifndef IO_H
#define IO_H

#include <stdio.h>
#include "../include/gestores/gestor_sistemas.h"

int open_file(int argc, char *argv[]);
void process_file(const char *base_path, const char *file_name, GestorSistema *gestor, char tipo);
void interpreter_inputs(FILE *file, GestorSistema *gestorsis);

#endif