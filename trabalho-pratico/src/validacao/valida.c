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
#include "../include/utils/string_utils.h"
#include "../include/validacao/valida.h"
#include "../include/gestores/gestor_albuns.h"

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
        free(birth_date);
        return FALSE;
    }

    gint ano, mes, dia;
    sscanf(birth_date, "%4d/%2d/%2d", &ano, &mes, &dia);

    if (!validarMesEDia(mes, dia) || !validarDataFutura(ano, mes, dia))
    {
        free(birth_date);

        return FALSE;
    }
    free(birth_date);

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
                g_free(email);
                return FALSE;
            }
        }
        else
        {
            if (!g_ascii_islower(email[i]))
            {
                g_free(email);
                return FALSE;
            }
        }
    }

    if (arroba_count != 1 || ponto_count != 1)
    {
        g_free(email);
        return FALSE;
    }
    if (a == 0 || p == a + 1 || p == -1 || (p - a <= 1))
    {
        g_free(email);
        return FALSE;
    }
    gint tamanho_apos_ponto = strlen(email) - p - 1;
    if (tamanho_apos_ponto < 2 || tamanho_apos_ponto > 3)
    {
        g_free(email);
        return FALSE;
    }

    g_free(email);
    return TRUE;
}

// Função que valida a subscrição do usuário
gboolean valida_subscricao(Usuario *usuario)
{
    gchar *subscription_type = user_get_subscription_type(usuario);
    if (g_strcmp0(subscription_type, "normal") != 0 && g_strcmp0(subscription_type, "premium") != 0)
    {
        g_free(subscription_type);
        return FALSE;
    }
    g_free(subscription_type);
    return TRUE;
}

// Função que valida se as músicas que têm liked_musics_copygosto existem
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
    if (str == NULL || str[0] == '\0')
        return FALSE;
    for (gint i = 0; str[i] != '\0'; i++)
    {
        if (!g_ascii_isdigit(str[i]))
        {
            return FALSE;
        }
    }
    return TRUE;
}

gboolean validaDuracao(Musica *musica)
{
    gchar *duracao = get_music_duration(musica);
    if (duracao == NULL)
        return FALSE;
    if (strlen(duracao) != 8 || duracao[2] != ':' || duracao[5] != ':')
    {
        g_free(duracao);
        return FALSE;
    }
    gchar horasStr[3] = {duracao[0], duracao[1], '\0'};
    gchar minutosStr[3] = {duracao[3], duracao[4], '\0'};
    gchar segundosStr[3] = {duracao[6], duracao[7], '\0'};

    if (!tudoNum(horasStr) || !tudoNum(minutosStr) || !tudoNum(segundosStr))
    {
        g_free(duracao);
        return FALSE;
    }

    gint horas = atoi(horasStr);
    gint minutos = atoi(minutosStr);
    gint segundos = atoi(segundosStr);

    if (horas < 0 || horas > 99 || minutos < 0 || minutos > 59 || segundos < 0 || segundos > 59)
    {
        g_free(duracao);
        return FALSE;
    }
    g_free(duracao);
    return TRUE;
}
// valida se a lista de artistas duma música ou banda começa e acaba em []
int valida_parenteses_lista(gchar *string)
{
    int len = strlen(string);
    if (string == NULL || len == 0 || string[0] != '[' || string[len - 1] != ']')
        return 0;
    return 1;
}

// valida se o artista tem ''
int valida_single_quotes_lista_artistas(gchar *artista)
{
    g_strstrip(artista);
    int len = strlen(artista);
    if (artista == NULL || len == 0 || artista[0] != '\'' || artista[len - 1] != '\'')
        return 0;
    return 1;
}

gboolean valida_artista_individual(Artista *artista)
{
    if (artista == NULL)
    {
        return FALSE;
    }
    gchar *tipo = get_artist_type(artista);
    if (g_strcmp0(tipo, "individual") == 0)
    {
        gchar **id_constituent = get_artist_id_constituent(artista);
        if (id_constituent != NULL)
        {
            g_free(tipo);
            g_strfreev(id_constituent);
            return FALSE;
        }
        else
        {
            g_free(tipo);
            g_strfreev(id_constituent);
            return TRUE;
        }
    }
    g_free(tipo);
    return TRUE;
}

int valida_ano_lançamento(Musica *musica)
{
    int ano = get_music_year(musica);
    if (ano > 2024)
        return 0;
    else
        return 1;
}

// falta validar se artist_ids de uma musica existem

int valida_artistids_musica(Musica *musica, GestorArtistas *gestorartistas)
{
    gchar **artist_ids = get_music_artist_ids(musica);
    for (int i = 0; artist_ids[i] != NULL; i++)
    {
        int artist_id_int = atoi(artist_ids[i]+1); 
        if (buscar_artista(gestorartistas, artist_id_int) == NULL)
        {
            g_strfreev(artist_ids);
            return 0;
        }
    }
    g_strfreev(artist_ids);
    return 1;
}

gboolean valida_album(Musica *musica, GestorAlbuns *gestor_albuns)
{

    gchar *album_copy = get_music_album(musica);

    if (album_copy == NULL)
    {
        return FALSE;
    }

    if (valida_parenteses_lista(album_copy) == 1)
    {
        g_free(album_copy);
        return FALSE;
    }

    GHashTable *hash_albuns = get_hash_albuns(gestor_albuns);

    Album *album_encontrado = (Album *)g_hash_table_lookup(hash_albuns, album_copy);

    if (album_encontrado == NULL)
    {
        g_free(album_copy);
        return FALSE;
    }

    // Libera a cópia da lista de IDs curtidos e retorna TRUE
    g_free(album_copy);
    return TRUE;
}

gboolean valida_plataforma(History *historico)
{

    gchar *historico_plataform = get_history_platform(historico);

    if (!(historico_plataform))
    {
        return FALSE;
    }

    gchar *platform_lower = g_ascii_strdown(historico_plataform, -1); // Converte para minúsculas
    g_free(historico_plataform);

    if (strcmp(platform_lower, "mobile") == 0 || strcmp(platform_lower, "desktop") == 0)
    {
        g_free(platform_lower);
        return TRUE;
    }

    g_free(platform_lower);
    return FALSE;
}

gboolean valida_artista_tipo(Artista *artista)
{

    gchar *artista_tipo = get_artist_type(artista);

    if (!(artista_tipo))
    {
        return FALSE;
    }

    gchar *artista_tipo_lower = g_ascii_strdown(artista_tipo, -1); // Converte para minúsculas
    g_free(artista_tipo);

    if (strcmp(artista_tipo_lower, "individual") == 0 || strcmp(artista_tipo_lower, "group") == 0)
    {
        g_free(artista_tipo_lower);
        return TRUE;
    }

    g_free(artista_tipo_lower);
    return TRUE;
}