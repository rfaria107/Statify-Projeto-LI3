#include "../include/gestores/gestor_sistemas.h"
#include "../include/gestores/gestor_artistas.h"
#include "../include/gestores/gestor_musicas.h"
#include "../include/gestores/gestor_albuns.h"
#include "../include/entidades/artists.h"
#include "../include/utils/utils.h"

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

        gchar **artist_ids = get_music_artist_ids(musica);
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

    // Verifica se o artista é individual
    if (g_strcmp0(artist_type, "individual") != 0)
    {
        free(artist_type);
        // Arredondar o valor diretamente antes de retornar
        return receita_artista;
    }

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

    // Arredondar para duas casas decimais
    receita_artista = arredondar_para_casas_decimais(receita_artista, 4);
    receita_participacao = arredondar_para_casas_decimais(receita_participacao,4);
    double receita_total = receita_artista + receita_participacao;
    receita_total = arredondar_para_casas_decimais(receita_total, 2);

    return receita_total;
}

int calcula_artista_num_albuns_individual(Artista *artista, GestorAlbuns *gestor_albuns)
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