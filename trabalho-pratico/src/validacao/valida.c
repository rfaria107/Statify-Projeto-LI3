#include <stdio.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>

#include "../include/entidades/artists.h"
#include "../include/entidades/musica.h"
#include "../include/entidades/usuario.h"
#include "../include/gestores/gestor_artistas.h"
#include "../include/gestores/gestor_musicas.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/parsing/parser.h"
#include "../include/parsing/rowreader.h"
#include "../include/parsing/string_utils.h"
#include "../include/validacao/valida.h"

// Valida se a data está no formato correto "YYYY/MM/DD"
gboolean validarFormatoData(gchar *data)
{
    if (strlen(data) != 10 || data[4] != '/' || data[7] != '/')
    {
        return FALSE; // Formato inválido
    }

    for (gint i = 0; i < 10; i++)
    {
        if (i == 4 || i == 7)
            continue;
        if (!g_ascii_isdigit(data[i]))
            return FALSE;
    }

    return TRUE;
}

// Verifica se o mês e o dia são válidos
gboolean validarMesEDia(gint mes, gint dia)
{
    return (mes >= 1 && mes <= 12 && dia >= 1 && dia <= 31);
}

// Verifica se a data fornecida está no futuro em relação à data atual
gboolean validarDataFutura(gint ano, gint mes, gint dia)
{
    gint anoAtual = 2024, mesAtual = 9, diaAtual = 9; // data atual

    if (ano > anoAtual || (ano == anoAtual && (mes > mesAtual || (mes == mesAtual && dia > diaAtual))))
    {
        return FALSE; // Data no futuro
    }

    return TRUE;
}

// Valida se a data de nascimento do usuário é válida
gboolean validarDataUsuario(Usuario *usuario)
{
    gchar *birth_date = user_get_birth_date(usuario);

    if (!validarFormatoData(birth_date))
    {
        return FALSE;
    }

    gint ano, mes, dia;
    sscanf(birth_date, "%4d/%2d/%2d", &ano, &mes, &dia);

    if (!validarMesEDia(mes, dia) || !validarDataFutura(ano, mes, dia))
    {
        return FALSE;
    }

    return TRUE;
}

// Função que valida o email
gboolean validaEmail(Usuario *usuario)
{
    gchar *email = user_get_email(usuario);

    gint arroba_count = 0;
    gint ponto_count = 0;
    gint a = -1;
    gint p = -1;
    gboolean antes_arroba = TRUE;

    for (gint i = 0; email[i] != '\0'; i++)
    {
        if (email[i] == '@')
        {
            arroba_count++;
            antes_arroba = FALSE;
            a = i;
        }
        else if (email[i] == '.')
        {
            ponto_count++;
            p = i;
        }
        else if (antes_arroba)
        {
            if (!g_ascii_islower(email[i]) && !g_ascii_isdigit(email[i]))
            {
                return FALSE;
            }
        }
        else
        {
            if (!g_ascii_islower(email[i]))
            {
                return FALSE;
            }
        }
    }

    if (arroba_count != 1 || ponto_count != 1)
        return FALSE;
    if (a == 0 || p == a + 1 || p == -1 || (p - a <= 1))
        return FALSE;

    gint tamanho_apos_ponto = strlen(email) - p - 1;
    if (tamanho_apos_ponto < 2 || tamanho_apos_ponto > 3)
        return FALSE;

    return TRUE;
}

// Função que valida a subscrição do usuário
gboolean valida_subscricao(Usuario *usuario)
{
    if (g_strcmp0(user_get_subscription_type(usuario), "normal") != 0 && g_strcmp0(user_get_subscription_type(usuario), "premium") != 0)
    {
        return FALSE;
    }
    return TRUE;
}

// Função que valida se as músicas que têm gosto existem
gboolean valida_liked_musics_id(Usuario *usuario, GestorMusicas *gestor_musicas)
{

    gchar **liked_musics_copy = user_get_liked_musics_id(usuario);

    if (liked_musics_copy == NULL)
    {
        return TRUE;
    }

    GHashTable *hash_musicas = get_hash_musicas(gestor_musicas);

    for (gint i = 0; liked_musics_copy[i] != NULL; i++)
    {
        Musica *musica_encontrada = (Musica *)g_hash_table_lookup(hash_musicas, liked_musics_copy[i]);

        if (musica_encontrada == NULL)
        {
            g_strfreev(liked_musics_copy);
            return FALSE;
        }
    }

    // Libera a cópia da lista de IDs curtidos e retorna TRUE
    g_strfreev(liked_musics_copy);
    return TRUE;
}

gboolean valida_user(Usuario *user, GestorMusicas *musics)
{
    if (user == NULL)
        return FALSE;

    if (validarDataUsuario(user) && valida_subscricao(user) && valida_liked_musics_id(user, musics) && validaEmail(user))
        return TRUE;
    else
        return FALSE;
}

gboolean tudoNum(gchar *str)
{
    for (gint i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit(str[i]))
        {
            return FALSE;
        }
    }
    return TRUE;
}

gboolean validaDuracao(Musica *musica)
{
    gchar *duracao = get_music_duration(musica);

    if (strlen(duracao) != 8 || duracao[2] != ':' || duracao[5] != ':')
        return FALSE;

    gchar horasStr[3] = {duracao[0], duracao[1], '\0'};
    gchar minutosStr[3] = {duracao[3], duracao[4], '\0'};
    gchar segundosStr[3] = {duracao[6], duracao[7], '\0'};

    if (!tudoNum(horasStr) || !tudoNum(minutosStr) || !tudoNum(segundosStr))
    {
        return FALSE;
    }

    gint horas = atoi(horasStr);
    gint minutos = atoi(minutosStr);
    gint segundos = atoi(segundosStr);

    if (horas < 0 || horas > 99 || minutos < 0 || minutos > 59 || segundos < 0 || segundos > 59)
    {
        return FALSE;
    }

    return TRUE;
}
//valida se a lista de artistas duma música começa e acaba em ['']
int valida_parenteses_lista_artistas(gchar *lista_artistas){
    int len = strlen(lista_artistas);
    if(lista_artistas[0] != '[' || lista_artistas[len-1] != ']') return 0;
    return 1;
}
//valida se o artista tem ''
int valida_single_quotes_lista_artistas(gchar *artista){
    int len = strlen(artista);
    if(artista[0] != '\'' || artista[len-1] != '\'') return 0;
    return 1;
}

gboolean valida_artista_individual(Artista *artista)
{
    if (artista == NULL)
    {
        return FALSE;
    }

    if (g_strcmp0(get_artist_type(artista), "individual") == 0)
    {
        if (get_artist_id_constituent(artista) != NULL)
        {
            return FALSE;
        }
    }

    if (get_artist_id(artista) == NULL || get_artist_name(artista) == NULL ||
        get_artist_recipe_per_stream(artista) <= 0 || get_artist_country(artista) == NULL ||
        get_artist_type(artista) == NULL)
    {
        return FALSE;
    }

    return TRUE;
}
