#ifndef queryS_H
#define queryS_H

#include <glib.h>
#include <stdio.h>
#include "../include/gestores/gestor_sistemas.h"
#include "../include/queries/query4_aux.h"

void query_1(GestorSistema *gestorsis, gchar *token, int line_number, int n);
void query_2(GestorSistema *gestorsis, int num, gchar *country, int line_number, int n);
void query_3(int min_age, int max_age, GestorSistema *gestor_sistema, int line_number, int n);
void query_4(char *data_inicial, char *data_final, GestorSistema *gestor_sistema, int line_number, int n, ResultadoProcessamento *resultado);
void query_5(char *user_id, int **matrizClassificacaoMusicas, char **idsUtilizadores, char **nomesGeneros,
             int numUtilizadores, int numGeneros, int numRecomendacoes, int linenumber, int n, GestorSistema *gestorsis);
void query_6(int user_id, int year, int N, GestorSistema *gestorsis, int line_number, int n);

// Função principal para interpretar inputs e executar querys
void interpreter_inputs(FILE *file, GestorSistema *gestorsis);

#endif // queryS_H
