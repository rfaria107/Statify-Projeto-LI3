// Função para converter "HH:MM:SS" para segundos
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
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


// Funcao para calcular o num_albums_individual (Possivel erro se tiverem 2 como grupo)

int get_artist_num_albuns_individual(Artista *artista, GestorAlbuns *gestor_albuns) {

    GHashTable *hash_albuns = get_hash_albuns(gestor_albuns);

    int count = 0;
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, hash_albuns);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Album *album = (Album *)value;

        // Itera pelos IDs dos artistas associados ao álbum
        gchar **artist_ids = get_album_artist_ids(album);
        if (artist_ids) {
            for (int i = 0; artist_ids[i] != NULL; i++) {
                if (strcmp(artist_ids[i], get_artist_id(artista)) == 0) {
                    count++;
                    break; // Já encontramos uma correspondência, podemos sair do loop
                }
            }
        }
    }

    return count;
}

double calcular_receita_total_artista(Artista *artista, GHashTable *hash_musicas, GHashTable *hash_history, GHashTable *hash_artistas) {
    double receita_artista = 0.0;
    double receita_participacao = 0.0;

    // Cache para evitar iterações repetidas
    GHashTable *musica_reproducoes = g_hash_table_new(g_str_hash, g_str_equal);

    // Itera pelas músicas no hash para encontrar as músicas do artista diretamente
    GHashTableIter music_iter;
    gpointer music_key, music_value;
    g_hash_table_iter_init(&music_iter, hash_musicas);

    while (g_hash_table_iter_next(&music_iter, &music_key, &music_value)) {
        Musica *musica = (Musica *)music_value;
        const char *musica_id = get_music_id(musica);

        // Verifica se a música pertence ao artista
        gchar **artist_ids = get_music_artist_ids(musica); 
        int artista_pertence = 0;
        for (int i = 0; artist_ids && artist_ids[i] != NULL; i++) {
            if (g_strcmp0(artist_ids[i], get_artist_id(artista)) == 0) {
                artista_pertence = 1;
                break;
            }
        }

        if (artist_ids) {
            g_strfreev(artist_ids); // Libera memória associada aos IDs
        }

        // Se o artista pertence à música, conta as reproduções
        if (artista_pertence) {
            // Usando cache para evitar iteração múltipla em hash_history
            if (!g_hash_table_contains(musica_reproducoes, musica_id)) {
                GHashTableIter history_iter;
                gpointer history_key, history_value;
                double contagem_reproducoes = 0.0;

                g_hash_table_iter_init(&history_iter, hash_history);
                while (g_hash_table_iter_next(&history_iter, &history_key, &history_value)) {
                    History *history = (History *)history_value;
                    if (g_strcmp0(get_history_music_id(history), musica_id) == 0) {
                        contagem_reproducoes++;
                    }
                }

                // Armazena o número de reproduções para essa música
                g_hash_table_insert(musica_reproducoes, (gpointer)musica_id, GINT_TO_POINTER(contagem_reproducoes));
            }

            // Calcula a receita do artista diretamente
            receita_artista += get_artist_recipe_per_stream(artista) * GPOINTER_TO_INT(g_hash_table_lookup(musica_reproducoes, musica_id));
        }

        // Calcula a receita de participação
        if (g_strcmp0(get_artist_type(artista), "individual") != 0) {
            gchar **artist_ids = get_music_artist_ids(musica);
            for (int i = 0; artist_ids && artist_ids[i] != NULL; i++) {
                Artista *grupo = g_hash_table_lookup(hash_artistas, artist_ids[i]);
                if (grupo && grupo != artista) {
                    gchar **constituents = get_artist_id_constituent(grupo);

                    int is_member = 0;
                    for (int j = 0; constituents && constituents[j] != NULL; j++) {
                        if (g_strcmp0(constituents[j], get_artist_id(artista)) == 0) {
                            is_member = 1;
                            break;
                        }
                    }

                    if (is_member) {
                        double receita_grupo = get_artist_recipe_per_stream(grupo) / get_num_constituents(grupo);
                        receita_participacao += receita_grupo * GPOINTER_TO_INT(g_hash_table_lookup(musica_reproducoes, musica_id));
                    }

                    if (constituents) {
                        g_strfreev(constituents);
                    }
                }
            }

            if (artist_ids) {
                g_strfreev(artist_ids); // Libera memória associada aos IDs
            }
        }
    }

    g_hash_table_destroy(musica_reproducoes); // Libera memória do cache

    // Soma as receitas diretas e de participação
    return receita_artista + receita_participacao;
}

// Função para contar o número de constituintes (membros) de um artista/grupo
int get_num_constituents(Artista *artista) {
    gchar **constituents = get_artist_id_constituent(artista);
    int num_constituents = 0;

    // Conta o número de membros
    if (constituents) {
        while (constituents[num_constituents] != NULL) {
            num_constituents++;
        }
        g_strfreev(constituents); // Libera memória associada aos IDs
    }

    return num_constituents;
}
