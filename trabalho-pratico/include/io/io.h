/**
 * @file io.h
 * @brief Header do módulo de IO.
 *
 * Este módulo contém funções para iniciar a execução do programa abrindo e processando os ficheiros, 
 * e interpretar inputs (recebê-los e retornar o devido output).
 */

#ifndef IO_H
#define IO_H

#include <stdio.h>
#include "../include/gestores/gestor_sistemas.h"

/**
 * @brief Abre e processa ficheiros de entrada e inicializa o sistema.
 *
 * Esta função é responsável por inicializar o sistema principal, processar os ficheiros
 * de dados principais (artistas, músicas, usuários, histórico, álbuns) e chamar o interpretador de
 * inputs para o ficheiro de inputs.
 *
 * @param argc Número de argumentos passados pela linha de comando.
 * @param argv Vetor de strings contendo os argumentos da linha de comando.
 *             - argv[1]: Caminho base para os ficheiros de dados principais.
 *             - argv[2]: Caminho para o ficheiro de inputs.
 * @return Retorna 0 se a execução for bem-sucedida.
 */
int open_file(int argc, char *argv[]);

/**
 * @brief Processa um ficheiro específico e carrega os dados no sistema.
 *
 * Esta função concatena o caminho base com o nome do ficheiro, abre o ficheiro e
 * utiliza o parser principal para carregar os dados no sistema.
 *
 * @param base_path Caminho base onde os ficheiros estão localizados.
 * @param file_name Nome do ficheiro a ser processado (ex.: "artists.csv").
 * @param gestor Ponteiro para o GestorSistema onde os dados serão armazenados.
 * @param tipo Tipo de dados processados:
 *             - 'a': Artistas
 *             - 'm': Músicas
 *             - 'u': Usuários
 *             - 'h': Histórico
 *             - 'l': Álbuns
 */
void process_file(const char *base_path, const char *file_name, GestorSistema *gestor, char tipo);

/**
 * @brief Interpreta comandos de entrada a partir de um ficheiro e executa queries.
 *
 * Esta função lê comandos de entrada linha por linha de um ficheiro, processa os dados
 * necessários e executa as queries correspondentes com base nos comandos fornecidos.
 *
 * @param file Ponteiro para o ficheiro de inputs contendo os comandos.
 * @param gestorsis Ponteiro para o GestorSistema que contém os dados processados.
 */
void interpreter_inputs(FILE *file, GestorSistema *gestorsis);

#endif // IO_H
