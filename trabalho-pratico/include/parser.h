#ifndef PARSER_H
#define PARSER_H

#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "../include/string_utils.h"
#include "../include/entidades/artists.h"
#include "../include/entidades/usuario.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/gestores/gestor_artistas.h"
#include "../include/gestores/gestor_usuarios.h"

void parser_principal(FILE *file, GestorSistema gestorsis, char tipo);
void parse_csv_line_artista(GestorArtistas gestorartistas, gchar *linha, Artista artista);
void parse_csv_line_users(GestorUsuarios gestorusuarios, gchar *linha, Usuario *usuario);

#endif