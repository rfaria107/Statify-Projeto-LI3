#ifndef STATS_H
#define STATS_H

#include "../include/gestores/gestor_sistemas.h"
#include "../include/gestores/gestor_musicas.h"
#include "../include/gestores/gestor_artistas.h"
#include "../include/gestores/gestor_albuns.h"
#include "../include/entidades/artists.h"

void calcular_discografia_artistas(GestorSistema *gestorsis);
void calcula_streams(GestorSistema *gestorsis);
double calcular_receita_total_artista(Artista *artista, GestorArtistas *gestorartistas, GestorMusicas *gestormusicas);
int calcula_artista_num_albuns_individual(Artista *artista, GestorAlbuns *gestor_albuns);

#endif