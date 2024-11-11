#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include "../include/parsing/queries.h"
#include "../include/entidades/artists.h"
#include "../include/entidades/musica.h"
#include "../include/entidades/usuario.h"
#include "../include/gestores/gestor_artistas.h"
#include "../include/gestores/gestor_musicas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/parsing/writer.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/parsing/string_utils.h"

struct GenrePopularity
{
    char *genre;
    int total_likes;
};

void interpreter_inputs(FILE *file, GestorSistema *gestorsis)
{
    char *buffer = "";
    size_t buffer_size = 0;
    int line_number = 1;
    // necessário para a query 2 no entanto apenas deve executar uma vez
    calcular_discografia_artistas(gestorsis);

    while (getline(&buffer, &buffer_size, file) != -1)
    {
        g_strstrip(buffer);
        if (strlen(buffer) == 0)
            continue; // Skipar linhas vazias

        char *token = procura_espaço(buffer);
        // query 1
        if (strcmp(token, "1") == 0)
        {
            g_free(token);
            token = procura_espaço2(buffer);
            if (token != NULL)
            {
                GestorUsuarios *gestor_users = get_gestor_usuarios(gestorsis);
                query_1(gestor_users, token, line_number);
                g_free(token);
            }
        }
        // query 2
        else if (strcmp(token, "2") == 0)
        {
            g_free(token);
            token = procura_espaço2(buffer);
            int num = 0;
            gchar *country = NULL;
            if (token != NULL)
            {
                num = atoi(token);
                g_free(token);
                token = procura_espaço3(buffer);
            }
            if (token != NULL)
            {
                country = g_strdup(token);
                g_free(token);
            }
            query_2(gestorsis, num, country, line_number);
            if (country != NULL)
                g_free(country);
        }
        // query 3
        else if (strcmp(token, "3") == 0)
        {
            g_free(token);
            token = procura_espaço2(buffer);
            int min_age = 0;
            if (token != NULL)
            {
                min_age = atoi(token);
                g_free(token);
                token = procura_espaço3(buffer);
                int max_age = 0;
                if (token != NULL)
                {
                    max_age = atoi(token);
                    g_free(token);
                }
                query_3(min_age, max_age, gestorsis, line_number);
            }
        }
        line_number++;
    }
    g_free(buffer);
}

void query_1(GestorUsuarios *gestor, char *username, int line_number)
{
    Usuario *usuario = buscar_usuario_id(gestor, username);
    // Agora com caminho para a pasta de resultados
    int size = snprintf(NULL, 0, "resultados/command%d_output.txt", line_number) + 1;
    char *output_file_name = malloc(size);
    snprintf(output_file_name, size, "resultados/command%d_output.txt", line_number);
    RowWriter *writer = initialize_row_writer(output_file_name, WRITE_MODE_CSV);

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
        write_row(writer, 5, mail, first_name, last_name, idade, country);

        free_and_finish_writing(writer);
        free(mail);
        free(first_name);
        free(last_name);
        free(country);
        free(output_file_name);
    }
    else
    {

        char *field_names[] = {""};
        char *formatting[] = {"%s"};
        row_writer_set_field_names(writer, field_names, 1);
        row_writer_set_formatting(writer, formatting);

        write_row(writer, 1, "");
        free_and_finish_writing(writer);
        free(output_file_name);
    }
}

void query_2(GestorSistema *gestorsis, int num, gchar *country, int line_number)
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
        write_row(writer, 1, "");
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

        write_row(writer, 4, name, type, duration_str, country_artista);

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

// Função para converter "HH:MM:SS" para segundos
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

void query_3(int min_age, int max_age, GestorSistema *gestor_sistema, int line_number)
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
        write_row(writer, 1, "");
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
            write_row(writer, 2, gp->genre, gp->total_likes);
        }
    }

    free_and_finish_writing(writer);
    free(output_file_name);

    g_list_free(generos_lista);
    g_hash_table_destroy(generos_likes);
}
