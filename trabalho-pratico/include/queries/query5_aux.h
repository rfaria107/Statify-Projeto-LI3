#ifndef QUERY5_AUX_H
#define QUERY5_AUX_H

#include "../include/gestores/gestor_sistemas.h"

char **preprocessNomesGeneros(GestorSistema *gestorsis, int *numGeneros);
char **preprocessIdsUtilizadores(GestorSistema *gestorsis, int *numUtilizadores);
void freeMatrizClassificacaoMusicas(int **matriz, int numUtilizadores);
int **createMatrizClassificacaoMusicas(int numUtilizadores, int numGeneros);
void calculaMatrizClassificacaoMusicas(int **matriz, char **ids_utilizadores, char **nomes_generos, int num_utilizadores,
                                       int num_generos, GestorSistema *gestorsis);

#endif