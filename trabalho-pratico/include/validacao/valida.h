#ifndef VALIDA_H
#define VALIDA_H

#include <glib.h>
#include "../../include/gestores/gestor_sistemas.h"
#include "../../include/entidades/musica.h"
#include "../../include/entidades/usuario.h"

gboolean validarFormatoData(gchar *data);
gboolean validarMesEDia(gint mes, gint dia);
gboolean validarDataFutura(gint ano, gint mes, gint dia);
gboolean validarDataUsuario(Usuario *usuario);
gboolean validaEmail(Usuario *usuario);
gboolean valida_subscricao(Usuario *usuario);
gboolean valida_liked_musics_id(Usuario *usuario, GestorMusicas *musicas);
gboolean valida_user(Usuario *user, GestorMusicas *musics);
gboolean tudoNum(gchar *str);
gboolean validaDuracao(Musica *musica);
gboolean valida_artista_individual(Artista *artista);
#endif