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

    if (!hashmusicas || !hashartistas)
    {
        return;
    }

    GHashTableIter iter_musicas;
    gpointer key_musica, value_musica;
    g_hash_table_iter_init(&iter_musicas, hashmusicas);

    while (g_hash_table_iter_next(&iter_musicas, &key_musica, &value_musica))
    {
        Musica *musica = (Musica *)value_musica;
        if (!musica)
            continue;

        gchar *duracao_musica = get_music_duration(musica);

        if (!duracao_musica)
        {
            continue;
        }

        int duracao_musica_seg = duracao_para_segundos(duracao_musica);
        g_free(duracao_musica);

        gchar **artist_ids = get_music_artist_ids(musica);
        if (!artist_ids)
        {
            continue; // Skip if no artist IDs
        }

        for (int i = 0; artist_ids[i] != NULL; i++)
        {
            gchar *artist_id = g_strdup(artist_ids[i]);
            int artist_id_int = atoi(artist_id + 1);
            Artista *artista = buscar_artista(gestorartistas, artist_id_int);
            g_free(artist_id);

            if (artista != NULL)
            {
                int duracao_atual = get_artist_duracao_disco(artista);
                if (duracao_atual < 0)
                {
                    continue;
                }
                int nova_duracao = duracao_atual + duracao_musica_seg;
                set_artista_duracao_disco(artista, nova_duracao);
            }
        }
        g_strfreev(artist_ids);
    }
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
    int artist_id = get_artist_id(artista);
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
                int artist_ids_int = atoi(artist_ids[i] + 1);
                if (artist_ids_int == artist_id)
                {
                    count++;
                    break; // Já encontramos uma correspondência, podemos sair do loop
                }
            }
            g_strfreev(artist_ids);
        }
    }
    // free(artist_id);
    return count;
}

void calcula_streams(GestorSistema *gestorsis)
{ // atualiza o campo streams das músicas encontradas no histórico
    GestorHistories *gestorhistory = get_gestor_histories(gestorsis);
    GestorMusicas *gestormusicas = get_gestor_musicas(gestorsis);
    GHashTable *hash_history = get_hash_histories(gestorhistory);

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, hash_history);

    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        History *history = (History *)value;
        int music_id = get_history_music_id(history);
        Musica *musica = buscar_musicas(gestormusicas, music_id);
        if (musica)
        {
            int stream_count = get_music_streams(musica);
            stream_count++;
            set_music_streams(musica, stream_count);
        }
        // free(music_id);
    }
}

double calcular_receita_total_artista(Artista *artista, GestorArtistas *gestorartistas, GestorMusicas *gestormusicas)
{
    GHashTable *hash_musicas = get_hash_musicas(gestormusicas);
    int artist_id = get_artist_id(artista);

    double receita_artista = 0.0;

    // Iterar pelas músicas para calcular receita direta
    GHashTableIter music_iter;
    gpointer music_key, music_value;
    g_hash_table_iter_init(&music_iter, hash_musicas);
    while (g_hash_table_iter_next(&music_iter, &music_key, &music_value))
    {
        Musica *musica = (Musica *)music_value;

        // Verifica se a música pertence ao artista
        gchar **artist_ids = get_music_artist_ids(musica); // Obtém os IDs de artistas associados à música
        for (int i = 0; artist_ids && artist_ids[i] != NULL; i++)
        {
            int artist_id_int = atoi(artist_ids[i] + 1);
            if (artist_id_int == artist_id)
            {
                int stream_count = get_music_streams(musica);
                receita_artista += stream_count * get_artist_recipe_per_stream(artista);
                break;
            }
        }
        g_strfreev(artist_ids);
    }
    gchar *artist_type = get_artist_type(artista);

    // Verifica se o artista é individual; caso contrário, retorna apenas receita direta
    if (g_strcmp0(artist_type, "individual") != 0)
    {
        free(artist_type);
        // free(artist_id);
        return receita_artista;
    }

    // Calcular receita por participação em grupos
    double receita_participacao = 0.0;
    g_hash_table_iter_init(&music_iter, hash_musicas);
    while (g_hash_table_iter_next(&music_iter, &music_key, &music_value))
    {
        Musica *musica = (Musica *)music_value;
        gchar **artist_ids = get_music_artist_ids(musica);

        for (int i = 0; artist_ids && artist_ids[i]; i++)
        {
            int artist_id_int = atoi(artist_ids[i] + 1);
            Artista *grupo = buscar_artista(gestorartistas, artist_id_int);
            if (grupo && grupo != artista)
            {
                gchar **constituents = get_artist_id_constituent(grupo);
                int is_member = 0;
                for (int j = 0; constituents && constituents[j] != NULL; j++)
                {
                    int constituent_int = atoi(constituents[j] + 1);
                    if (constituent_int == artist_id)
                    {
                        is_member = 1;
                        break;
                    }
                }

                if (is_member)
                {
                    int stream_count = get_music_streams(musica);
                    receita_participacao += (stream_count * get_artist_recipe_per_stream(grupo)) / get_num_constituents(grupo);
                }
                g_strfreev(constituents);
            }
        }
        g_strfreev(artist_ids);
    }
    free(artist_type);
    // free(artist_id);
    //  Soma as receitas diretas e de participação
    return (receita_artista + receita_participacao);
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

gchar *find_top_entry_with_tiebreaker(GHashTable *table, gboolean is_numeric, gboolean alphabetical)
{
    GHashTableIter iter;
    gpointer key, value;
    gchar *top_key = NULL;
    gint64 top_value = -1;

    g_hash_table_iter_init(&iter, table);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        gchar *current_key = (gchar *)key;
        gint64 current_value = GPOINTER_TO_INT(value);

        if (current_value > top_value ||
            (current_value == top_value &&
             ((alphabetical && g_strcmp0(current_key, top_key) < 0) ||
              (!alphabetical && is_numeric && g_ascii_strcasecmp(current_key, top_key) < 0))))
        {
            top_key = current_key;
            top_value = current_value;
        }
    }
    return top_key ? g_strdup(top_key) : NULL;
}

gint compare_by_value_with_tiebreaker(gconstpointer a, gconstpointer b, gpointer user_data)
{
        // Converte os ponteiros de valores (que são inteiros) para inteiros
    gint value_a = GPOINTER_TO_INT(a);
    gint value_b = GPOINTER_TO_INT(b);

    // Caso os valores sejam iguais, a comparação será feita pela chave (ID do artista)

    gint key_a = (gchar *)g_hash_table_lookup(user_data, a);
    gint key_b = (gchar *)g_hash_table_lookup(user_data, b);

    // Se os valores são diferentes, retorna a diferença para ordenar
    if (value_a != value_b)
    {
        return value_b - value_a;
    }

    // Para o desempate, caso seja alfabético
    return key_a - key_b;
}

// Função para ordenar a GHashTable por valor com desempate
GList *sort_hash_table_by_value_with_tiebreaker(GHashTable *table, gboolean reverse, gboolean alphabetical)
{
    // Lista para armazenar as chaves da tabela
    GList *keys = g_hash_table_get_keys(table);

    // Criando um ponteiro para os dados do usuário
    gpointer user_data = GINT_TO_POINTER(reverse);

    // Ordena a lista de chaves
    keys = g_list_sort_with_data(keys, compare_by_value_with_tiebreaker, user_data);

    return keys;
}