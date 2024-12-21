// Função para converter "HH:MM:SS" para segundos
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include "../include/queries/queries.h"
#include "../include/entidades/artists.h"
#include "../include/entidades/musica.h"
#include "../include/entidades/usuario.h"
#include "../include/gestores/gestor_artistas.h"
#include "../include/gestores/gestor_musicas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/write/writer.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/parsing/string_utils.h"
#include "../include/queries/queries_aux.h"

struct GenrePopularity
{
    char *genre;
    int total_likes;
};

struct Semana {
    char *inicio;
    char *fim;
    GPtrArray *historico;
    GHashTable *top10;
};

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

void calcular_discografia_artistas(GestorSistema *gestorsis)
{
    GestorArtistas *gestorartistas = get_gestor_artistas(gestorsis);
    GestorMusicas *gestormusicas = get_gestor_musicas(gestorsis);

    GHashTable *hashartistas = get_hash_artistas(gestorartistas);
    GHashTable *hashmusicas = get_hash_musicas(gestormusicas);

    GList *lista_musicas = g_hash_table_get_values(hashmusicas);
    for (GList *node = lista_musicas; node != NULL; node = node->next)
    {
        Musica *musica = (Musica *)node->data;
        if (!musica)
            continue;

        gchar *duracao_musica = get_music_duration(musica);
        int duracao_musica_seg = duracao_para_segundos(duracao_musica);
        gchar **artist_ids = get_music_artist_ids(musica);

        for (int i = 0; artist_ids[i] != NULL; i++)
        {
            gchar *artist_id = g_strdup(artist_ids[i]);
            Artista *artista = (Artista *)g_hash_table_lookup(hashartistas, artist_id);
            g_free(artist_id);
            if (artista != NULL)
            {
                int duracao_atual = get_artist_duracao_disco(artista);
                int nova_duracao = duracao_atual + duracao_musica_seg;
                set_artista_duracao_disco(artista, nova_duracao);
            }
        }
        g_free(duracao_musica);
        g_strfreev(artist_ids);
    }
    g_list_free(lista_musicas);
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
// Função para criar uma nova instância de GenrePopularity
GenrePopularity *create_genre_popularity(const char *genre)
{
    GenrePopularity *gp = malloc(sizeof(GenrePopularity));
    gp->genre = g_strdup(genre); // Copia a string para evitar problemas de ponteiros
    gp->total_likes = 0;
    return gp;
}

// Função para liberar a memória de GenrePopularity
void free_genre_popularity(GenrePopularity *gp)
{
    g_free(gp->genre);
    free(gp);
}

// Função de comparação para ordenar primeiro pela popularidade e depois alfabeticamente (em caso de empate)
int compare_genre_popularity(const void *a, const void *b)
{
    // Converter
    GenrePopularity *gp_a = (GenrePopularity *)a;
    GenrePopularity *gp_b = (GenrePopularity *)b;

    if (gp_b->total_likes != gp_a->total_likes)
    {
        return gp_b->total_likes - gp_a->total_likes; // Ordena por likes decrescentemente
    }
    return g_strcmp0(gp_a->genre, gp_b->genre); // Ordena alfabeticamente em caso de empate (usando uma funcao pre-definida do Glib)
}


