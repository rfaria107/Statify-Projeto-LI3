#ifndef USUARIO_H
#define USUARIO_H

#include <glib.h>
#include <ctype.h>  
#include <string.h> 

#include "../../include/entidades/usuario.h"

// Funções de validação de datas
gboolean validarFormatoData(const gchar *data);
gboolean validarMesEDia(gint mes, gint dia);
gboolean validarDataFutura(gint ano, gint mes, gint dia);
gboolean validarDataUsuario(const Usuario *usuario);
gint calcularIdade(const Usuario *usuario);

// Função de validar email
gboolean validaEmail(const Usuario *usuario);

// Funçao de validar subscrição
gboolean valida_subscricao(const Usuario *usuario);

// Função de validar se existem as musicas que os usuarios têm like 
gboolean valida_liked_musics_id(const Usuario *usuario, GHashTable *musicas);

#endif
