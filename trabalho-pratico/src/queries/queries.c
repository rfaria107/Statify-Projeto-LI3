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
#include "../include/utils/string_utils.h"
#include "../include/queries/queries_aux.h"

struct GenrePopularity
{
    char *genre;
    int total_likes;
};

void query_1(GestorSistema *gestorsis, char *username, int line_number, int n)
{
    int size = snprintf(NULL, 0, "resultados/command%d_output.txt", line_number) + 1;
    char *output_file_name = malloc(size);
    snprintf(output_file_name, size, "resultados/command%d_output.txt", line_number);
    RowWriter *writer = initialize_row_writer(output_file_name, WRITE_MODE_CSV);

    // Flag para verificar se já escreveu algo
    int has_written = 0;

    // Inicializa variáveis para identificar se é usuário ou artista
    Usuario *usuario = NULL;
    Artista *artista = NULL;

    // Verifica se o username pertence a um usuário
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
            write_row(writer, ';', 5, mail, first_name, last_name, idade, country);
        }
        else if (n == 1)
        {
            write_row(writer, '=', 5, mail, first_name, last_name, idade, country);
        }

        // Marca que algo foi escrito no arquivo
        has_written = 1;

        // Libera memória do usuário
        free(mail);
        free(first_name);
        free(last_name);
        free(country);
    }
    else
    {
        // Se não for usuário, verifica se é artista
        GestorArtistas *gestorartistas = get_gestor_artistas(gestorsis);
        artista = buscar_artista(gestorartistas, username);

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
            int num_albums = get_artist_num_albuns_individual(artista, gestoralbuns);
            double total_recipe = calcular_receita_total_artista(artista, gestorartistas, gestormusicas);

            if (n == 0)
            {
                write_row(writer, ';', 5, name, type, country, num_albums, total_recipe);
            }
            else if (n == 1)
            {
                write_row(writer, '=', 5, name, type, country, num_albums, total_recipe);
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

void query_2(GestorSistema *gestorsis, int num, gchar *country, int line_number,int n)
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
        if (n == 0){
        write_row(writer, ';', 4, name, type, duration_str, country_artista);
        }
        if (n == 1){
         write_row(writer, '=', 4, name, type, duration_str, country_artista);
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

void query_3(int min_age, int max_age, GestorSistema *gestor_sistema, int line_number,int n)
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

            for (gchar **music_id = liked_musics; *music_id != NULL; music_id++)
            {
                Musica *musica = buscar_musicas(gestor_musicas, *music_id);
                if (musica)
                {
                    char *genre = get_music_genre(musica);

                    GenrePopularity *gp = (GenrePopularity *)g_hash_table_lookup(generos_likes, genre);
                    if (!gp)
                    {
                        gp = create_genre_popularity(genre);
                        g_hash_table_insert(generos_likes, g_strdup(genre), gp);
                    }
                    gp->total_likes++;
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
        if (n == 0) {
        write_row(writer, ';', 1, "");
        }
        if (n == 1) {
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
            if (n == 0) {
            write_row(writer, ';', 2, gp->genre, gp->total_likes);
            }
            if (n == 1) {
            write_row(writer, '=', 2, gp->genre, gp->total_likes);
            }
        }
    }

    free_and_finish_writing(writer);
    free(output_file_name);

    g_list_free(generos_lista);
    g_hash_table_destroy(generos_likes);
}
