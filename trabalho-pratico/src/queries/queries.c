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
#include "../include/io/write/writer.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/utils/string_utils.h"
#include "../include/utils/utils.h"
#include "../include/utils/stats/stats.h"
#include "../include/utils/recomendador/recomendador.h"
#include "../include/queries/query3_aux.h"
#include "../include/queries/query4_aux.h"
#include "../include/queries/query5_aux.h"
#include "../include/queries/query6_aux.h"

void query_1(GestorSistema *gestorsis, gchar *token, int line_number, int n, int t)
{
    int size = snprintf(NULL, 0, "resultados/command%d_output.txt", line_number) + 1;
    char *output_file_name = malloc(size);
    snprintf(output_file_name, size, "resultados/command%d_output.txt", line_number);
    RowWriter *writer = initialize_row_writer(output_file_name, WRITE_MODE_CSV);

    // Flag para verificar se já escreveu algo
    int has_written = 0;

    if (token[0] == 'U')
    {
        Usuario *usuario = NULL;
        int username = atoi(token + 1);
        GestorUsuarios *gestoruser = get_gestor_usuarios(gestorsis);
        usuario = buscar_usuario_id(gestoruser, username);
        if (usuario)
        {
            // Define nomes e formatos dos campos para o RowWriter
            char *field_names[] = {"Email", "First Name", "Last Name", "Age", "Country"};
            char *formatting[] = {"%s", "%s", "%s", "%d", "%s"};
            row_writer_set_field_names(writer, field_names, 5);
            row_writer_set_formatting(writer, formatting);

            gint idade = calcularIdade(usuario);

            char *mail = user_get_email(usuario);
            char *first_name = user_get_first_name(usuario);
            char *last_name = user_get_last_name(usuario);
            char *country = user_get_country(usuario);

            // Escreve a linha com os dados do usuário no arquivo de saída
            if (n == 0)
            {
                if (t == 0)
                {
                    write_row(writer, ';', 5, mail, first_name, last_name, idade, country);
                }
                else if (t == 1)
                {
                    print_row(writer, ';', 5, mail, first_name, last_name, idade, country);
                }
            }
            else if (n == 1)
            {
                if (t == 0)
                {
                    write_row(writer, '=', 5, mail, first_name, last_name, idade, country);
                }
                else if (t == 1)
                {
                    print_row(writer, '=', 5, mail, first_name, last_name, idade, country);
                }
            }

            // Marca que algo foi escrito no arquivo
            has_written = 1;

            // Libera memória do usuário
            free(mail);
            free(first_name);
            free(last_name);
            free(country);
        }
    }
    else if (token[0] == 'A')
    {
        // Se não for usuário, verifica se é artista
        Artista *artista = NULL;
        int artist_id = atoi(token + 1);
        GestorArtistas *gestorartistas = get_gestor_artistas(gestorsis);
        artista = buscar_artista(gestorartistas, artist_id);

        if (artista)
        {
            GestorAlbuns *gestoralbuns = get_gestor_albuns(gestorsis);
            GestorMusicas *gestormusicas = get_gestor_musicas(gestorsis);

            char *field_names[] = {"Name", "Type", "Country", "Num Albums", "Total Recipe"};
            char *formatting[] = {"%s", "%s", "%s", "%d", "%.2f"};
            row_writer_set_field_names(writer, field_names, 5);
            row_writer_set_formatting(writer, formatting);

            char *name = get_artist_name(artista);
            char *type = get_artist_type(artista);
            char *country = get_artist_country(artista);
            int num_albums = calcula_artista_num_albuns_individual(artista, gestoralbuns);
            double total_recipe = calcular_receita_total_artista(artista, gestorartistas, gestormusicas);

            if (n == 0)
            {
                if (t == 0)
                {
                    write_row(writer, ';', 5, name, type, country, num_albums, total_recipe);
                }
                else if (t == 1)
                {
                    print_row(writer, ';', 5, name, type, country, num_albums, total_recipe);
                }
            }
            else if (n == 1)
            {
                if (t == 0)
                {
                    write_row(writer, '=', 5, name, type, country, num_albums, total_recipe);
                }
                else if (t == 1)
                {
                    print_row(writer, '=', 5, name, type, country, num_albums, total_recipe);
                }
            }

            // Marca que algo foi escrito no arquivo
            has_written = 1;

            // Libera memória do artista
            free(name);
            free(type);
            free(country);
        }
    }

    // Se não escreveu nada ainda, escreve a linha em branco
    if (!has_written)
    {
        char *field_names[] = {""};
        char *formatting[] = {"%s"};
        row_writer_set_field_names(writer, field_names, 1);
        row_writer_set_formatting(writer, formatting);

        write_row(writer, ';', 1, ""); // Linha em branco
    }

    // Finaliza a escrita
    free_and_finish_writing(writer);

    // Libera a memória do arquivo de saída
    free(output_file_name);
}

void query_2(GestorSistema *gestorsis, int num, gchar *country, int line_number, int n, int t)
{
    int size = snprintf(NULL, 0, "resultados/command%d_output.txt", line_number) + 1;
    char *output_file_name = malloc(size);
    snprintf(output_file_name, size, "resultados/command%d_output.txt", line_number);
    RowWriter *writer = initialize_row_writer(output_file_name, WRITE_MODE_CSV);

    if (num == 0)
    {
        char *field_names_empty[] = {""};
        char *formatting_empty[] = {"%s"};
        row_writer_set_field_names(writer, field_names_empty, 1);
        row_writer_set_formatting(writer, formatting_empty);
        write_row(writer, ';', 1, "");
        free_and_finish_writing(writer);
        free(output_file_name);
        return;
    }

    GestorArtistas *gestorartistas = get_gestor_artistas(gestorsis);
    GestorMusicas *gestormusicas = get_gestor_musicas(gestorsis);
    GHashTable *hashartistas = get_hash_artistas(gestorartistas);
    if (country != NULL)
        trim_quotes_gchar(country);
    if (!hashartistas || !gestormusicas)
        return;

    GList *lista_artistas = NULL;
    if (country != NULL) // se existir filtro de país
    {
        GHashTableIter iter;
        gpointer key, value;
        g_hash_table_iter_init(&iter, hashartistas);

        while (g_hash_table_iter_next(&iter, &key, &value))
        {
            int pos = 0;
            Artista *artista = (Artista *)value;
            gchar *artist_country = get_artist_country(artista);
            // se pertencer ao país vai para a lista
            if (g_strcmp0(artist_country, country) == 0)
            {
                lista_artistas = g_list_insert(lista_artistas, artista, pos);
            }
            g_free(artist_country);
        }
    }
    else
    {
        lista_artistas = g_hash_table_get_values(hashartistas);
    }

    lista_artistas = g_list_sort(lista_artistas, compara_duracoes_discografia);

    // copiar só os top n artistas para uma lista final
    GList *lista_top_n_artistas = NULL;
    GList *node = lista_artistas;
    for (int i = 0; i < num && node != NULL; i++, node = node->next)
    {
        lista_top_n_artistas = g_list_append(lista_top_n_artistas, node->data);
    }

    char *field_names[] = {"Artist Name", "Type", "Total Duration", "Country"};
    char *formatting[] = {"%s", "%s", "%s", "%s"};
    row_writer_set_field_names(writer, field_names, 4);
    row_writer_set_formatting(writer, formatting);

    for (GList *iter = lista_top_n_artistas; iter != NULL; iter = iter->next)
    {
        Artista *artista = (Artista *)iter->data;
        gchar *name = get_artist_name(artista);
        gchar *type = get_artist_type(artista);
        gint duracao_total = get_artist_duracao_disco(artista);
        gchar *country_artista = get_artist_country(artista);
        // Converter de volta para string
        gchar *duration_str = segundos_para_duracao(duracao_total);
        if (n == 0)
        {
            if (t == 0)
            {
                write_row(writer, ';', 4, name, type, duration_str, country_artista);
            }
            else if (t == 1)
            {
                print_row(writer, ';', 4, name, type, duration_str, country_artista);
            }
        }
        if (n == 1)
        {
            if (t == 0)
            {
                write_row(writer, '=', 4, name, type, duration_str, country_artista);
            }
            else if (t == 1)
            {
                print_row(writer, '=', 4, name, type, duration_str, country_artista);
            }
        }

        g_free(name);
        g_free(type);
        g_free(duration_str);
        g_free(country_artista);
    }

    free_and_finish_writing(writer);
    free(output_file_name);
    g_list_free(lista_top_n_artistas);
    g_list_free(lista_artistas);
}

void query_3(int min_age, int max_age, GestorSistema *gestor_sistema, int line_number, int n, int t)
{
    if (!gestor_sistema)
    {
        return;
    }

    GestorUsuarios *gestor_usuarios = get_gestor_usuarios(gestor_sistema);
    GestorMusicas *gestor_musicas = get_gestor_musicas(gestor_sistema);
    if (!gestor_usuarios || !gestor_musicas)
    {
        return;
    }

    GHashTable *usuarios = get_hash_usuarios(gestor_usuarios);
    if (!usuarios)
    {
        return;
    }

    GHashTable *generos_likes = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify)free_genre_popularity);

    // Parte que vai iterar sobre a hash criada onde ira armazena os generos verificados em cada musica
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, usuarios);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        Usuario *usuario = (Usuario *)value;

        int idade = calcularIdade(usuario); // clcular a idade de cada usuario

        if (idade >= min_age && idade <= max_age)
        {                                                             // a idade_min e a idade_max inclusive
            gchar **liked_musics = user_get_liked_musics_id(usuario); // Em caso positivo acedemos ás liked_musics do artista

            if (liked_musics == NULL || *liked_musics == NULL)
            {
                continue; // Caso o usuário não tenha músicas curtidas, ignora
            }

            for (int i = 0; liked_musics[i] != NULL; i++)
            {
                gchar *music_id_str = liked_musics[i];
                int music_id_int = atoi(music_id_str + 1);
                Musica *musica = buscar_musicas(gestor_musicas, music_id_int);
                if (musica)
                {
                    char *genre = get_music_genre(musica);

                    GenrePopularity *gp = (GenrePopularity *)g_hash_table_lookup(generos_likes, genre);
                    if (!gp)
                    {
                        gp = create_genre_popularity(genre);
                        g_hash_table_insert(generos_likes, g_strdup(genre), gp);
                    }
                    int total_likes = get_gp_total_likes(gp);
                    total_likes++;
                    set_gp_total_likes(gp, total_likes);
                    g_free(genre);
                }
            }

            g_strfreev(liked_musics); // Libera o array de strings após o uso
        }
    }

    //  Tranformar a hastable dos generos e likes, numa lista ligada
    GList *generos_lista = g_hash_table_get_values(generos_likes);
    generos_lista = g_list_sort(generos_lista, (GCompareFunc)compare_genre_popularity);

    // Aloca dinamicamente o nome do arquivo baseado no número da linha
    int size = snprintf(NULL, 0, "resultados/command%d_output.txt", line_number) + 1;
    char *output_file_name = malloc(size);
    snprintf(output_file_name, size, "resultados/command%d_output.txt", line_number);
    RowWriter *writer = initialize_row_writer(output_file_name, WRITE_MODE_CSV);

    // Mesmo que o output seja vazio, o ficheiro tem que ser criado
    if (generos_lista == NULL)
    {
        char *field_names_empty[] = {""};
        char *formatting_empty[] = {"%s"};
        row_writer_set_field_names(writer, field_names_empty, 1);
        row_writer_set_formatting(writer, formatting_empty);
        if (n == 0)
        {
            write_row(writer, ';', 1, "");
        }
        if (n == 1)
        {
            write_row(writer, '=', 1, "");
        }
    }
    else
    {
        // Define nomes e formatos dos campos para o RowWriter

        char *field_names[] = {"Genre", "Total Likes"};
        char *formatting[] = {"%s", "%d"};
        row_writer_set_field_names(writer, field_names, 2);
        row_writer_set_formatting(writer, formatting);
        // Imprime os gêneros e likes no arquivo
        for (GList *node = generos_lista; node != NULL; node = node->next)
        {
            GenrePopularity *gp = (GenrePopularity *)node->data;
            char *genre = get_gp_genre(gp);
            int total_likes = get_gp_total_likes(gp);
            if (n == 0)
            {
                if (t == 0)
                {
                    write_row(writer, ';', 2, genre, total_likes);
                }
            }
            if (n == 1)
            {
                if (t == 1)
                {
                    write_row(writer, '=', 2, genre, total_likes);
                }
            }
            g_free(genre);
        }
    }

    free_and_finish_writing(writer);
    free(output_file_name);

    g_list_free(generos_lista);
    g_hash_table_destroy(generos_likes);
}
void query_4(char *data_inicial, char *data_final, GestorSistema *gestor_sistema, int line_number, int n, ResultadoProcessamento *resultado, int l)
{

    if (data_final == NULL)
    {
        // Se data_final for NULL, chama a função para processar todo o histórico
        all_historico(gestor_sistema, line_number, n, resultado, l);
    }
    else
    {
        // Caso contrário, chama a função para processar o intervalo de datas
        intervalos_historico(gestor_sistema, line_number, n, data_inicial, data_final, resultado, l);
    }
}

void query_5(char *user_id, int **matrizClassificacaoMusicas, char **idsUtilizadores, char **nomesGeneros,
             int numUtilizadores, int numGeneros, int numRecomendacoes, int line_number, int n, GestorSistema *gestorsis, int t)
{

    int size = snprintf(NULL, 0, "resultados/command%d_output.txt", line_number) + 1;
    char *output_file_name = malloc(size);
    snprintf(output_file_name, size, "resultados/command%d_output.txt", line_number);
    RowWriter *writer = initialize_row_writer(output_file_name, WRITE_MODE_CSV);
    GestorUsuarios *gestoruser = get_gestor_usuarios(gestorsis);
    int user_id_int = atoi(user_id + 1);
    Usuario *user = buscar_usuario_id(gestoruser, user_id_int);

    if (numRecomendacoes > 0 && user != NULL)
    {
        char **utilizadores = recomendaUtilizadores(user_id, matrizClassificacaoMusicas, idsUtilizadores, nomesGeneros,
                                                    numUtilizadores, numGeneros, numRecomendacoes);

        // Define nomes e formatos dos campos para o RowWriter

        char *field_names[] = {"Utilizador recomendado"};
        char *formatting[] = {"%s"};
        row_writer_set_field_names(writer, field_names, 1);
        row_writer_set_formatting(writer, formatting);

        for (int i = 0; i < numRecomendacoes; i++)
        {
            if (n == 0)
            {
                if (t == 0)
                {
                    write_row(writer, ';', 1, utilizadores[i]);
                }
                else if (t == 1)
                {
                    print_row(writer, ';', 1, utilizadores[i]);
                }
            }
            if (n == 1)
            {
                if (t == 0)
                {
                    write_row(writer, '=', 1, utilizadores[i]);
                }
                else if (t == 1)
                {
                    print_row(writer, '=', 1, utilizadores[i]);
                }
            }
        }
        free(utilizadores);
    }
    else
    {
        char *field_names_empty[] = {""};
        char *formatting_empty[] = {"%s"};
        row_writer_set_field_names(writer, field_names_empty, 1);
        row_writer_set_formatting(writer, formatting_empty);
        if (n == 0)
        {
            write_row(writer, ';', 1, "");
        }
        if (n == 1)
        {
            write_row(writer, '=', 1, "");
        }
    }
    free_and_finish_writing(writer);
    free(output_file_name);
}

void query_6(int user_id, int year, int N, GestorSistema *gestorsis, int line_number, int n, int t)
{
    char output_file_name[256];
    snprintf(output_file_name, sizeof(output_file_name), "resultados/command%d_output.txt", line_number);
    RowWriter *writer = initialize_row_writer(output_file_name, WRITE_MODE_CSV);

    // Configura os nomes e formatos dos campos
    char *field_names[] = {"Total Time", "Music Count", "Top Artist ID", "Top Day", "Top Genre", "Top Album", "Top Hour"};
    char *formatting[] = {"%s", "%d", "A%07d", "%s", "%s", "AL%06d", "%s"};
    row_writer_set_field_names(writer, field_names, 7);
    row_writer_set_formatting(writer, formatting);

    // Obtém o gestor de históricos
    GestorHistories *gestorhistories = get_gestor_histories(gestorsis);
    GHashTable *hash_histories = get_hash_histories(gestorhistories);
    GHashTableIter iter;
    gpointer key, value;
    gboolean user_found = FALSE;

    GHashTable *artist_time = g_hash_table_new(g_direct_hash, g_direct_equal);
    GHashTable *day_count = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    GHashTable *hour_time = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    GHashTable *genre_popularity = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    GHashTable *album_time = g_hash_table_new(g_direct_hash, g_direct_equal);
    GHashTable *artist_music_count = g_hash_table_new(g_direct_hash, g_direct_equal);

    int total_time = 0, music_count = 0;

    g_hash_table_iter_init(&iter, hash_histories);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        History *history = (History *)value;
        int history_user_id = get_history_user_id(history);
        if (history_user_id == user_id)
        {
            user_found = TRUE;

            char year_str[5];
            snprintf(year_str, sizeof(year_str), "%d", year);
            gchar *timestamp = get_history_timestamp(history);
            if (strncmp(timestamp, year_str, 4) == 0)
            {
                gchar *history_duration = get_history_duration(history);
                int duration = duracao_para_segundos(history_duration);
                g_free(history_duration);
                total_time += duration;
                music_count++;

                // Atualiza dados de dia e hora
                gchar date[11], hour[3];
                strncpy(date, timestamp, 10);
                date[10] = '\0';
                strncpy(hour, timestamp + 11, 2);
                hour[2] = '\0';

                g_free(timestamp);

                // Contabiliza o número de músicas por dia
                gpointer current_day_count = g_hash_table_lookup(day_count, date);
                int day_total = current_day_count ? GPOINTER_TO_INT(current_day_count) + 1 : 1;
                g_hash_table_insert(day_count, g_strdup(date), GINT_TO_POINTER(day_total));

                // Contabiliza o tempo por hora
                gpointer current_hour_time = g_hash_table_lookup(hour_time, hour);
                int hour_total = current_hour_time ? GPOINTER_TO_INT(current_hour_time) + duration : duration;
                g_hash_table_insert(hour_time, g_strdup(hour), GINT_TO_POINTER(hour_total));

                // Processa dados da música, artista, gênero e álbum
                int history_music_id = get_history_music_id(history);
                if (history_music_id)
                {
                    GestorMusicas *gestormusicas = get_gestor_musicas(gestorsis);
                    Musica *musica = buscar_musicas(gestormusicas, history_music_id);
                    if (musica)
                    {
                        // Atualiza gêneros
                        char *genre = get_music_genre(musica);
                        if (genre)
                        {
                            // Incrementa a contagem de execuções do gênero
                            gpointer current_genre_time = g_hash_table_lookup(genre_popularity, genre);
                            int genre_total_time = current_genre_time ? GPOINTER_TO_INT(current_genre_time) + duration : duration;
                            g_hash_table_insert(genre_popularity, g_strdup(genre), GINT_TO_POINTER(genre_total_time));
                            g_free(genre);
                        }

                        // Atualiza artistas
                        gchar **artist_ids = get_music_artist_ids(musica);
                        for (int i = 0; artist_ids && artist_ids[i] != NULL; i++)
                        {
                            gchar *artist_id = artist_ids[i];
                            gint artist_id_int = atoi(artist_id + 1);
                            gpointer current_artist_time = g_hash_table_lookup(artist_time, GINT_TO_POINTER(artist_id_int));
                            int artist_total = current_artist_time ? GPOINTER_TO_INT(current_artist_time) + duration : duration;
                            g_hash_table_insert(artist_time, GINT_TO_POINTER(artist_id_int), GINT_TO_POINTER(artist_total));

                            // Contagem de músicas distintas por artista
                            gpointer current_music_count = g_hash_table_lookup(artist_music_count, GINT_TO_POINTER(artist_id_int));
                            int music_total = current_music_count ? GPOINTER_TO_INT(current_music_count) + 1 : 1;
                            g_hash_table_insert(artist_music_count, GINT_TO_POINTER(artist_id_int), GINT_TO_POINTER(music_total));
                        }
                        if (artist_ids)
                        {
                            g_strfreev(artist_ids);
                        }

                        // Atualiza álbuns
                        gchar *album_id = get_music_album(musica);
                        if (album_id)
                        {
                            gint album_id_int = atoi(album_id + 2);
                            gpointer current_album_time = g_hash_table_lookup(album_time, GINT_TO_POINTER(album_id_int));
                            int album_total = current_album_time ? GPOINTER_TO_INT(current_album_time) + duration : duration;
                            g_hash_table_insert(album_time, GINT_TO_POINTER(album_id_int), GINT_TO_POINTER(album_total));
                            g_free(album_id);
                        }
                    }
                }
            }
            else
                g_free(timestamp);
        }
    }

    if (!user_found)
    {
        // Se o usuário não foi encontrado
        write_row(writer, ';', 1, "");
        g_hash_table_destroy(artist_time);
        g_hash_table_destroy(day_count);
        g_hash_table_destroy(hour_time);
        g_hash_table_destroy(genre_popularity);
        g_hash_table_destroy(album_time);
        g_hash_table_destroy(artist_music_count);
        free_and_finish_writing(writer);
        return;
    }

    // Encontrar os mais populares
    gint top_artist_id = find_top_entry_with_tiebreaker(artist_time, TRUE); // Alfabético (ID menor)
    gchar *top_day = find_top_day_with_tiebreaker(day_count);
    gchar *top_hour = find_top_entry_with_tiebreaker_str(hour_time, FALSE);        // Mais cedo
    gchar *top_genre = find_top_entry_with_tiebreaker_str(genre_popularity, TRUE); // Alfabético
    gint top_album = find_top_entry_with_tiebreaker(album_time, TRUE);             // Alfabético

    if (top_artist_id == -1 || !top_day || !top_hour || !top_genre || top_album == -1)
    {
        // Caso algum dos valores seja inválido, escreve linha vazia no arquivo
        write_row(writer, ';', 1, "");

        // Libera as strings e recursos alocados dinamicamente
        if (top_day)
            g_free(top_day);
        if (top_hour)
            g_free(top_hour);
        if (top_genre)
            g_free(top_genre);

        // Libera as hash tables usadas
        g_hash_table_destroy(artist_time);
        g_hash_table_destroy(day_count);
        g_hash_table_destroy(hour_time);
        g_hash_table_destroy(genre_popularity);
        g_hash_table_destroy(album_time);
        g_hash_table_destroy(artist_music_count);

        // Finaliza a escrita do arquivo
        free_and_finish_writing(writer);
        return;
    }

    gchar *total_time_duracao = segundos_para_duracao(total_time);
    // Escrever os resultados no arquivo
    if (t == 0)
    {
        // Escrever os resultados no arquivo
        write_row(writer, (n == 0 ? ';' : '='), 7, total_time_duracao, music_count, top_artist_id, top_day, top_genre, top_album, top_hour);
    }
    else if (t == 1)
    {
        print_row(writer, (n == 0 ? ';' : '='), 7, total_time_duracao, music_count, top_artist_id, top_day, top_genre, top_album, top_hour);
    }
    free(total_time_duracao);

    GList *sorted_list = NULL;
    GHashTableIter artist_iter;
    gpointer key2, value2;

    // Preenche a lista com os IDs dos artistas
    g_hash_table_iter_init(&artist_iter, artist_time);
    while (g_hash_table_iter_next(&artist_iter, &key2, &value2))
    {
        sorted_list = g_list_prepend(sorted_list, key2); // Adiciona os IDs na lista
    }

    // Ordena a lista com os artistas
    sorted_list = g_list_sort_with_data(sorted_list, (GCompareDataFunc)compare_artist_values, artist_time);

    int displayed_artists = 0;

    if (sorted_list != NULL) // Alterado para 'sorted_list'
    {
        for (GList *iter = sorted_list; iter && displayed_artists < N; iter = iter->next)
        {
            gint artist_id = GPOINTER_TO_INT(iter->data);

            // Verifica se o artist_id existe nas tabelas de hash
            gpointer duration_ptr = g_hash_table_lookup(artist_time, GINT_TO_POINTER(artist_id));
            gpointer distinct_musics_ptr = g_hash_table_lookup(artist_music_count, GINT_TO_POINTER(artist_id));

            // Se algum valor for NULL, saltamos este artista
            if (duration_ptr != NULL && distinct_musics_ptr != NULL)
            {
                // Converte os ponteiros para inteiros de forma segura
                int duration = GPOINTER_TO_INT(duration_ptr);
                int distinct_musics = GPOINTER_TO_INT(distinct_musics_ptr);

                // Converte o tempo total (em segundos) para o formato de duração
                gchar *duration_string = segundos_para_duracao(duration);

                // Chama write_row com os parâmetros necessários
                char *field_names[] = {"Top Artist ID", "Music Count", "Top Hour"};
                char *formatting[] = {"A%07d", "%d", "%s"};
                row_writer_set_field_names(writer, field_names, 3);
                row_writer_set_formatting(writer, formatting);
                
                // Escreve os dados no arquivo
                if (t == 0)
                {
                    // Escrever os resultados no arquivo
                    write_row(writer, (n == 0 ? ';' : '='), 3, artist_id, distinct_musics, duration_string);
                }
                else if (t == 1)
                {
                    print_row(writer, (n == 0 ? ';' : '='), 3, artist_id, distinct_musics, duration_string);
                }

                // Libera a memória alocada para duration_string
                g_free(duration_string);

                // Contabiliza os artistas exibidos
                displayed_artists++;
            }
        }
    }

    // Libera a lista de artistas ordenados após o uso
    g_list_free(sorted_list); // Alterado para 'sorted_list'

    // Limpeza
    free(top_day);
    free(top_genre);
    free(top_hour);
    g_hash_table_destroy(artist_time);
    g_hash_table_destroy(day_count);
    g_hash_table_destroy(hour_time);
    g_hash_table_destroy(genre_popularity);
    g_hash_table_destroy(album_time);
    g_hash_table_destroy(artist_music_count);

    free_and_finish_writing(writer);
}
