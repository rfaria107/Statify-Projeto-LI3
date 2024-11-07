#ifndef PARSER_H
#define PARSER_H


#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "../include/parsing/string_utils.h"
#include "../include/entidades/artists.h"
#include "../include/entidades/usuario.h"
#include "../include/entidades/musica.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/gestores/gestor_artistas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/parsing/rowreader.h"

void parser_principal(FILE *file, GestorSistema *gestor, char tipo);
int parse_csv_line_usuario(RowReader *reader, Usuario *usuario, GestorMusicas *gestormusicas);
int parse_csv_line_artista(RowReader *reader, Artista *artista);
int parse_csv_line_musica(RowReader *reader, Musica *musica);
int preenche_musica(GPtrArray *campostemp, Musica *musica);
int preenche_usuario(GPtrArray *campostemp, Usuario *usuario, GestorMusicas *gestormusicas);
int preenche_artista(GPtrArray *campostemp, Artista *artista);
char **parse_liked_musics(RowReader *reader);

#endif