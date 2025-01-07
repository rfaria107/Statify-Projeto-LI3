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

// Função para calcular os domingos relativos à data inicial e final
void calcular_domingos_inicial_e_final(char *data_inicial, char *data_final, char **domingo_inicial, char **domingo_final)
{
    *domingo_inicial = calcular_domingo(data_inicial);
    *domingo_final = calcular_domingo(data_final);
}

// Calcula o domingo da semana de uma data
gchar *calcular_domingo(const gchar *data)
{
    int ano, mes, dia;
    if (sscanf(data, "%d/%d/%d", &ano, &mes, &dia) != 3)
    {
        fprintf(stderr, "Erro ao fazer o parse da data.\n");
        return NULL;
    }

    struct tm tm_data = {0};
    tm_data.tm_year = ano - 1900;
    tm_data.tm_mon = mes - 1;
    tm_data.tm_mday = dia;
    mktime(&tm_data);

    int dias_para_domingo = tm_data.tm_wday;
    tm_data.tm_mday -= dias_para_domingo;
    mktime(&tm_data);

    gchar *domingo = malloc(11 * sizeof(gchar));
    if (domingo == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para a data de domingo.\n");
        return NULL;
    }

    strftime(domingo, 11, "%Y/%m/%d", &tm_data);
    return domingo;
}

// Calcula o sábado seguinte de uma data
gchar *calcular_sabado_seguinte(const gchar *data)
{
    int ano, mes, dia;
    if (sscanf(data, "%d/%d/%d", &ano, &mes, &dia) != 3)
    {
        fprintf(stderr, "Erro ao fazer o parse da data.\n");
        return NULL;
    }

    struct tm tm_data = {0};
    tm_data.tm_year = ano - 1900;
    tm_data.tm_mon = mes - 1;
    tm_data.tm_mday = dia;
    mktime(&tm_data);

    int dias_para_sabado = (6 - tm_data.tm_wday + 7) % 7;
    tm_data.tm_mday += dias_para_sabado;
    mktime(&tm_data);

    gchar *sabado = malloc(11 * sizeof(gchar));
    if (sabado == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para a data de sábado.\n");
        return NULL;
    }

    strftime(sabado, 11, "%Y/%m/%d", &tm_data);
    return sabado;
}

time_t get_timestamp(const char *data)
{
    struct tm tm_data = {0};
    if (sscanf(data, "%d/%d/%d", &tm_data.tm_year, &tm_data.tm_mon, &tm_data.tm_mday) != 3)
    {
        fprintf(stderr, "Erro: Formato de data inválido.\n");
        return -1;
    }
    tm_data.tm_year -= 1900;
    tm_data.tm_mon -= 1;
    return mktime(&tm_data);
}

