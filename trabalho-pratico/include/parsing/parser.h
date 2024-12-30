#ifndef PARSER_H
#define PARSER_H

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "../include/utils/string_utils.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/gestores/gestor_artistas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/gestores/gestor_albuns.h"

void parser_principal(FILE *file, GestorSistema *gestor, char tipo);
Artista *parse_csv_line_artista(gchar *line);
Artista *preenche_artista(GPtrArray *campostemp);

Musica *parse_csv_line_musica(gchar* line, GestorArtistas *gestorartistas, GestorAlbuns *gestoralbuns);
Musica *preenche_musica(GPtrArray *campostemp, GestorArtistas *gestorartistas,GestorAlbuns *gestoralbuns);

Usuario *parse_csv_line_usuario(gchar* line, GestorMusicas *gestormusicas);
Usuario *preenche_usuario(GPtrArray *campostemp, GestorMusicas *gestormusicas);

Album *parse_csv_line_album(gchar *line, GestorAlbuns *gestor_albuns);
Album *preenche_album(GPtrArray *campostemp, GestorAlbuns *gestoralbuns);
History *parse_csv_line_history(gchar *line, GestorHistories *gestorhistory);
History *preenche_history(GPtrArray *campostemp, GestorHistories *gestorhistory);

#endif