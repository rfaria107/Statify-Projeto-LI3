#ifndef USUARIO_H
#define USUARIO_H

#include <glib.h>
#include <ctype.h>  
#include <string.h> 

typedef struct {
    gchar* username;           // Nome de usuário (string dinâmica)
    gchar* email;              // Email (string dinâmica)
    gchar* first_name;         // Primeiro nome (string dinâmica)
    gchar* last_name;          // Sobrenome (string dinâmica)
    gchar* birth_date;         // Data de nascimento (string dinâmica no formato YYYY/MM/DD)
    gchar* country;            // País (string dinâmica)
    gchar* subscription_type;  // Tipo de assinatura (string dinâmica)
    gchar** liked_musics_id;   // Lista de IDs de músicas curtidas (array dinâmico de strings)
} Usuario;



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
