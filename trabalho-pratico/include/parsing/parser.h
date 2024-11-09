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
Artista *parse_csv_line_artista(gchar *line);
Artista *preenche_artista(GPtrArray *campostemp);

Musica *parse_csv_line_musica(gchar* line, GestorArtistas *gestorartistas);
Musica *preenche_musica(GPtrArray *campostemp, GestorArtistas *gestorartistas);

Usuario *parse_csv_line_usuario(gchar* line, GestorMusicas *gestormusicas);
Usuario *preenche_usuario(GPtrArray *campostemp, GestorMusicas *gestormusicas);

#endif