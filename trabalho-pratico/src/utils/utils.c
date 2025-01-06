#include <glib.h>
#include <math.h>
#include "../include/utils/utils.h"
#include "../include/entidades/artists.h"

gint duracao_para_segundos(const gchar *duracao)
{
    gint horas, minutos, segundos;
    sscanf(duracao, "%d:%d:%d", &horas, &minutos, &segundos);
    return horas * 3600 + minutos * 60 + segundos;
}

// Função para converter segundos em "HH:MM:SS"
gchar *segundos_para_duracao(gint total_segundos)
{
    gint horas = total_segundos / 3600;
    gint minutos = (total_segundos % 3600) / 60;
    gint segundos = total_segundos % 60;

    return g_strdup_printf("%02d:%02d:%02d", horas, minutos, segundos);
}

// Função para arredondar para um número específico de casas decimais
double arredondar_para_casas_decimais(double valor, int casas)
{
    double fator = pow(10, casas);
    // Adiciona um pequeno valor antes de arredondar (ajuste para corrigir o arredondamento)
    double ajuste = 1e-9;
    return round((valor + ajuste) * fator) / fator;
}

gint compara_duracoes_discografia(gconstpointer a, gconstpointer b)
{
    Artista *artista_a = (Artista *)a;
    Artista *artista_b = (Artista *)b;

    gint duracao_a = get_artist_duracao_disco(artista_a);
    gint duracao_b = get_artist_duracao_disco(artista_b);

    // compara por duracao
    if (duracao_a < duracao_b)
    {
        return 1;
    }
    else if (duracao_a > duracao_b)
    {
        return -1;
    }

    // se a duracao for igual comparar ordem alfabetica do nome
    gchar *nome_a = get_artist_name(artista_a);
    gchar *nome_b = get_artist_name(artista_b);
    int r = g_strcmp0(nome_a, nome_b);
    g_free(nome_a);
    g_free(nome_b);
    return r;
}






