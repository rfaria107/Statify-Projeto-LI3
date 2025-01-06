#ifndef UTILS_H
#define UTILS_H

#include <glib.h>
#include <stdio.h>
#include <string.h>


gint duracao_para_segundos(const gchar *duracao);
gchar *segundos_para_duracao(gint total_segundos);
double arredondar_para_casas_decimais(double valor, int casas);
gint compara_duracoes_discografia(gconstpointer a, gconstpointer b);


#endif