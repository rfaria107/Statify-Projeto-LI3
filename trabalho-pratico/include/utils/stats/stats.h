/**
 * @file stats.h
 * @brief Declarações de funções para cálculos estatísticos relacionados a artistas, músicas e álbuns.
 *
 * Este ficheiro contém as definições de funções utilizadas para calcular dados estatísticos,
 * como discografia de artistas, streams de músicas, receita total e número de álbuns individuais, 
 * relevantes para pré processamento.
 *
 */

#ifndef STATS_H
#define STATS_H

#include "../include/gestores/gestor_sistemas.h"
#include "../include/gestores/gestor_musicas.h"
#include "../include/gestores/gestor_artistas.h"
#include "../include/gestores/gestor_albuns.h"
#include "../include/entidades/artists.h"

/**
 * @brief Calcula a discografia dos artistas e atualiza a duração total de suas músicas.
 *
 * Esta função percorre todas as músicas do sistema e atualiza a duração total
 * das músicas associadas a cada artista.
 *
 * @param gestorsis Ponteiro para o gestor do sistema que contém os gestores de artistas e músicas.
 */
void calcular_discografia_artistas(GestorSistema *gestorsis);

/**
 * @brief Atualiza o número de streams para cada música com base no histórico.
 *
 * Esta função percorre o histórico do sistema e incrementa o contador de streams
 * das músicas correspondentes.
 *
 * @param gestorsis Ponteiro para o gestor do sistema que contém o histórico e o gestor de músicas.
 */
void calcula_streams(GestorSistema *gestorsis);

/**
 * @brief Calcula a receita total gerada por um artista.
 *
 * A receita é calculada com base no número de streams das músicas associadas ao artista.
 * Para artistas individuais, também considera participações em grupos.
 *
 * @param artista Ponteiro para o artista cuja receita será calculada.
 * @param gestorartistas Ponteiro para o gestor de artistas.
 * @param gestormusicas Ponteiro para o gestor de músicas.
 * @return Receita total do artista arredondada para duas casas decimais.
 */
double calcular_receita_total_artista(Artista *artista, GestorArtistas *gestorartistas, GestorMusicas *gestormusicas);

/**
 * @brief Calcula o número de álbuns associados a um artista individual.
 *
 * Esta função verifica todos os álbuns do sistema e conta aqueles que têm o artista especificado.
 *
 * @param artista Ponteiro para o artista cuja discografia será analisada.
 * @param gestor_albuns Ponteiro para o gestor de álbuns.
 * @return Número de álbuns associados ao artista.
 */
int calcula_artista_num_albuns_individual(Artista *artista, GestorAlbuns *gestor_albuns);

#endif /* STATS_H */
