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
#include "../include/parsing/rowreader.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/parsing/string_utils.h"

void interpreter_inputs(FILE *file, GestorSistema *gestorsis)
{
    char *buffer = "";
    size_t buffer_size = 0;
    // RowReader *reader = initialize_row_reader(buffer, ' ');
    int line_number = 1;

    while (getline(&buffer, &buffer_size, file) != -1)
    {
        g_strstrip(buffer);
        if (strlen(buffer) == 0)
            continue; // Skipar linhas vazias
        // reader_set_row(reader, buffer);
        char *token = procura_espaço(buffer);
        // querie 1
        if (strcmp(token, "1") == 0)
        {
            g_free(token);
            token = procura_espaço2(buffer);
            if (token != NULL)
            {
                GestorUsuarios *gestor_users = get_gestor_usuarios(gestorsis);
                querie_1(gestor_users, token, line_number);
                g_free(token);
            }
        }

        else if (strcmp(token, "2") == 0)
        {
            g_free(token);
            token = procura_espaço2(buffer);
            if (token != NULL)
            {
                int num = atoi(token);
                g_free(token);

                token = procura_espaço3(buffer);
            }
            if (token != NULL)
            {
                char *country = g_strdup(token);
                g_free(token);
                // querie2(gestorsis,num,country)
                g_free(country);
            }
            // querie2(gestorsis,num,country);
        }
        else if (strcmp(token, "3") == 0)
        {
            g_free(token);
            token = procura_espaço2(buffer);
            if (token != NULL)
            {
                int min_age = atoi(token);
                g_free(token);
                token = procura_espaço3(buffer);
                int max_age = 200;
                querie_3 (min_age,max_age,gestorsis,line_number);
                if (token != NULL)
                {
                    max_age = atoi(token);
                    g_free(token);
                    querie_3 (min_age,max_age,gestorsis,line_number);

                }
            }
        }
        line_number++;
    }
    g_free(buffer);
    // free_row_reader(reader);
}

void querie_1(GestorUsuarios *gestor, char *username, int line_number)
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

/*

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


gchar *calcular_discografia(GestorArtistas *gestorartistas, const Artista *artista)
{
    GHashTable *hash_musicas = get_hash_table(gestorartistas);
    gint duracao_total_segundos = 0;

    for (int i = 0; get_ar != NULL && artista->id_constituent[i] != NULL; i++)
    {
        gchar *id_musica = artista->id_constituent[i];
        Musica *musica = (Musica *)g_hash_table_lookup(hash_musicas, id_musica);

        if (musica != NULL)
        {
            duracao_total_segundos += duracao_para_segundos(musica ->duration);
        }
    }

    return segundos_para_duracao(duracao_total_segundos);
}

*/


// Função para criar uma nova instância de GenrePopularity
GenrePopularity *create_genre_popularity(const char *genre) {
    GenrePopularity *gp = malloc(sizeof( GenrePopularity));
    gp->genre = g_strdup(genre);  // Copia a string para evitar problemas de ponteiros
    gp->total_likes = 0;
    return gp;
}

// Função para liberar a memória de GenrePopularity
void free_genre_popularity(GenrePopularity *gp) {
    g_free(gp->genre);
    free(gp);
}

// Função de comparação para ordenar primeiro pela popularidade e depois alfabeticamente (em caso de empate)
int compare_genre_popularity(const void *a, const void *b) {
    //Converter 
    GenrePopularity *gp_a = (GenrePopularity *)a; 
    GenrePopularity *gp_b = (GenrePopularity *)b;

    if (gp_b->total_likes != gp_a->total_likes) {
        return gp_b->total_likes - gp_a->total_likes;  // Ordena por likes decrescentemente
    }
    return g_strcmp0(gp_a->genre, gp_b->genre);  // Ordena alfabeticamente em caso de empate (usando uma funcao pre-definida do Glib)
}
void querie_3(int min_age, int max_age, GestorSistema *gestor_sistema, int line_number) {
    if (!gestor_sistema) {
        return;
    }

    GestorUsuarios *gestor_usuarios = get_gestor_usuarios(gestor_sistema);
    GestorMusicas *gestor_musicas = get_gestor_musicas(gestor_sistema);
    if (!gestor_usuarios || !gestor_musicas) {
        return;
    }

    GHashTable *usuarios = get_hash_usuarios(gestor_usuarios);
    if (!usuarios) {
        return;
    }

    GHashTable *generos_likes = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify)free_genre_popularity);

   // Parte que vai iterar sobre a hash criada onde ira armazena os generos verificados em cada musica
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, usuarios);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Usuario *usuario = (Usuario *)value;
        
        int idade = calcularIdade(usuario); // clcular a idade de cada usuario

        if (idade >= min_age && idade <= max_age) { // a idade_min e a idade_max inclusive
            gchar **liked_musics = user_get_liked_musics_id(usuario); // Em caso positivo acedemos ás liked_musics do artista

            if (liked_musics == NULL || *liked_musics == NULL) {
                continue; // Caso o usuário não tenha músicas curtidas, ignora
            }

            for (gchar **music_id = liked_musics; *music_id != NULL; music_id++) {
                Musica *musica = buscar_musicas(gestor_musicas, *music_id);
                if (musica) {
                    char *genre = get_music_genre(musica);

                    GenrePopularity *gp = (GenrePopularity *)g_hash_table_lookup(generos_likes, genre);
                    if (!gp) {
                        gp = create_genre_popularity(genre);
                        g_hash_table_insert(generos_likes, g_strdup(genre), gp);
                    }
                    gp->total_likes++;
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

    // Define nomes e formatos dos campos para o RowWriter
    char *field_names[] = {"Genre", "Total Likes"};
    char *formatting[] = {"%s", "%d"};
    row_writer_set_field_names(writer, field_names, 2);
    row_writer_set_formatting(writer, formatting);
    
    // Mesmo que o output seja vazio, o ficheiro tem que ser criado
    if (generos_lista == NULL) {
        char *field_names_empty[] = {""};
        char *formatting_empty[] = {"%s"};
        row_writer_set_field_names(writer, field_names_empty, 1);
        row_writer_set_formatting(writer, formatting_empty);
        write_row(writer, 1, "");
    } else {
        // Imprime os gêneros e likes no arquivo 
            for (GList *node = generos_lista; node != NULL; node = node->next) {
            GenrePopularity *gp = (GenrePopularity *)node->data;
            write_row(writer, 2, gp->genre, gp->total_likes);
        }
    }

    free_and_finish_writing(writer);
    free(output_file_name);

    g_list_free(generos_lista);
    g_hash_table_destroy(generos_likes);
}
