// Função para converter "HH:MM:SS" para segundos
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <math.h>
#include "../include/queries/queries.h"
#include "../include/entidades/artists.h"
#include "../include/entidades/musica.h"
#include "../include/entidades/usuario.h"
#include "../include/entidades/albuns.h"
#include "../include/entidades/history.h"
#include "../include/gestores/gestor_artistas.h"
#include "../include/gestores/gestor_musicas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/gestores/gestor_histories.h"
#include "../include/gestores/gestor_albuns.h"
#include "../include/write/writer.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/utils/string_utils.h"
#include "../include/queries/queries_aux.h"

struct GenrePopularity
{
    char *genre;
    int total_likes;
};

struct Semana
{
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

// Funcao para calcular o num_albums_individual (Possivel erro se tiverem 2 como grupo)

int get_artist_num_albuns_individual(Artista *artista, GestorAlbuns *gestor_albuns)
{

    GHashTable *hash_albuns = get_hash_albuns(gestor_albuns);

    int count = 0;
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, hash_albuns);

    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        Album *album = (Album *)value;

        // Itera pelos IDs dos artistas associados ao álbum
        gchar **artist_ids = get_album_artist_ids(album);
        if (artist_ids)
        {
            for (int i = 0; artist_ids[i] != NULL; i++)
            {
                if (strcmp(artist_ids[i], get_artist_id(artista)) == 0)
                {
                    count++;
                    break; // Já encontramos uma correspondência, podemos sair do loop
                }
            }
            g_strfreev(artist_ids);
        }
    }

    return count;
}

void calcula_streams(GestorSistema *gestorsis)
{ // atualiza o campo streams das músicas encontradas no histórico
    GestorHistories *gestorhistory = get_gestor_histories(gestorsis);
    GestorMusicas *gestormusicas = get_gestor_musicas(gestorsis);
    GHashTable *hash_history = get_hash_histories(gestorhistory);
    GHashTable *hash_musicas = get_hash_musicas(gestormusicas);

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, hash_history);

    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        History *history = (History *)value;
        gchar *music_id = get_history_music_id(history);

        Musica *musica = g_hash_table_lookup(hash_musicas, music_id);
        if (musica)
        {
            int stream_count = get_music_streams(musica);
            stream_count++;
            set_music_streams(musica, stream_count);
        }
    }
}

double calcular_receita_total_artista(Artista *artista, GestorArtistas *gestorartistas, GestorMusicas *gestormusicas) {
    GHashTable *hash_musicas = get_hash_musicas(gestormusicas);
    GHashTable *hash_artistas = get_hash_artistas(gestorartistas);
    double receita_artista = 0.0;

    // Iterar pelas músicas para calcular receita direta
    GHashTableIter music_iter;
    gpointer music_key, music_value;
    g_hash_table_iter_init(&music_iter, hash_musicas);
    while (g_hash_table_iter_next(&music_iter, &music_key, &music_value)) {
        Musica *musica = (Musica *)music_value;
        gchar **artist_ids = get_music_artist_ids(musica);

        for (int i = 0; artist_ids && artist_ids[i]; i++) {
            if (g_strcmp0(artist_ids[i], get_artist_id(artista)) == 0) {
                receita_artista += get_music_streams(musica) * get_artist_recipe_per_stream(artista);
                break;
            }
        }
        g_strfreev(artist_ids);
    }

    // Caso o artista seja coletivo, não calcular participação
    if (g_strcmp0(get_artist_type(artista), "individual") != 0) {
        return round(receita_artista * 100) / 100.0;
    }

    // Calcular receita por participação em grupos
    double receita_participacao = 0.0;
    g_hash_table_iter_init(&music_iter, hash_musicas);
    while (g_hash_table_iter_next(&music_iter, &music_key, &music_value)) {
        Musica *musica = (Musica *)music_value;
        gchar **artist_ids = get_music_artist_ids(musica);

        for (int i = 0; artist_ids && artist_ids[i]; i++) {
            Artista *grupo = g_hash_table_lookup(hash_artistas, artist_ids[i]);
            if (grupo && grupo != artista) {
                gchar **constituents = get_artist_id_constituent(grupo);
                int is_member = 0;

                for (int j = 0; constituents && constituents[j]; j++) {
                    if (g_strcmp0(constituents[j], get_artist_id(artista)) == 0) {
                        is_member = 1;
                        break;
                    }
                }

                if (is_member) {
                    int stream_count = get_music_streams(musica);
                    receita_participacao += (stream_count * get_artist_recipe_per_stream(grupo)) / get_num_constituents(grupo);
                }
                g_strfreev(constituents);
            }
        }
        g_strfreev(artist_ids);
    }

    return round((receita_artista + receita_participacao) * 100) / 100.0;
}


// Função para contar o número de constituintes (membros) de um artista/grupo
int get_num_constituents(Artista *artista)
{
    gchar **constituents = get_artist_id_constituent(artista);
    int num_constituents = 0;

    // Conta o número de membros
    if (constituents)
    {
        while (constituents[num_constituents] != NULL)
        {
            num_constituents++;
        }
        g_strfreev(constituents); // Libera memória associada aos IDs
    }

    return num_constituents;
}

char **lista_user_ids(GestorUsuarios *gestorusers)
{
    GHashTable *hashusers = get_hash_usuarios(gestorusers);
    if (!hashusers)
        return NULL;

    GList *user_ids_glist = g_hash_table_get_keys(hashusers); // os ids dos utilizadores são utilizados como keys na hashtable, basta extrair as keys com a funcção da glib e converter a GList em char**
    int num_users = g_list_length(user_ids_glist);

    char **user_ids = malloc((num_users + 1) * sizeof(char *));

    if (!user_ids)
    {
        g_list_free(user_ids_glist);
        return NULL;
    }

    int i = 0;
    for (GList *node = user_ids_glist; node != NULL; node = node->next, i++)
    {
        user_ids[i] = g_strdup((char *)node->data);
    }
    user_ids[i] = NULL;

    g_list_free(user_ids_glist);

    return user_ids;
}