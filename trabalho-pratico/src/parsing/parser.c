#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/parsing/parser.h"
#include "../include/parsing/string_utils.h"
#include "../include/entidades/artists.h"
#include "../include/entidades/musica.h"
#include "../include/entidades/usuario.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/gestores/gestor_artistas.h"
#include "../include/validacao/valida.h"
#include "../include/write/writer.h"
#include "../include/gestores/gestor_albuns.h"
#include "../include/queries/queries.h"

int open_file(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <path_to_csvs> <path_to_inputs>\n", argv[0]);
        return 1;
    }

    GestorSistema *gestor = criar_gestor_sistema(); // Inicializa o gestor
    char *path = g_strdup(argv[1]);

    // Processa os arquivos principais
    process_file(path, "artists.csv", gestor, 'a');
    process_file(path, "musics.csv", gestor, 'm');
    process_file(path, "users.csv", gestor, 'u');
    process_file(path, "history.csv", gestor, 'h');
    process_file(path, "albums.csv", gestor, 'l');

    // Processa o arquivo de inputs
    char *pathinputs = g_strdup(argv[2]);
    FILE *fileinputs = fopen(pathinputs, "r");
    if (fileinputs) {
        interpreter_inputs(fileinputs, gestor);
        fclose(fileinputs);
    } else {
        fprintf(stderr, "Error opening file: %s\n", pathinputs);
    }
    g_free(pathinputs);

    g_free(path);
    liberar_gestor_sistema(gestor);

    return 0;
}

void process_file(const char *base_path, const char *file_name, GestorSistema *gestor, char tipo) {
    char *file_path = g_strconcat(base_path, "/", file_name, NULL);
    FILE *file = fopen(file_path, "r");
    if (file) {
        parser_principal(file, gestor, tipo);
        fclose(file);
    } else {
        fprintf(stderr, "Error opening file: %s\n", file_path);
    }
    g_free(file_path);
}


void parser_principal(FILE *file, GestorSistema *gestor, char tipo)
{
    char *buffer = NULL;
    size_t buffer_size = 0;
    gboolean first_line = TRUE;
    RowWriter *writer_error_artists;
    RowWriter *writer_error_musics;
    RowWriter *writer_error_users;
    RowWriter *writer_error_history;
    RowWriter *writer_error_album;
    if (tipo == 'a')
    {
        const char *error_file_artists = "resultados/artists_errors.csv";
        writer_error_artists = initialize_error_writer(error_file_artists);
        escrever_cabecalho_artists_erro(writer_error_artists);
    }
    if (tipo == 'm')
    {
        const char *error_file_musics = "resultados/musics_errors.csv";
        writer_error_musics = initialize_error_writer(error_file_musics);
        escrever_cabecalho_musics_erro(writer_error_musics);
    }
    if (tipo == 'u')
    {
        const char *error_file_users = "resultados/users_errors.csv";
        writer_error_users = initialize_error_writer(error_file_users);
        escrever_cabecalho_users_erro(writer_error_users);
    }
    if (tipo== 'h')
    {
        const char *error_files_history= "resultados/history_errors.csv";
        writer_error_history = initialize_error_writer (error_files_history);
        escrever_cabecalho_history_erro (writer_error_history);
    }
    if (tipo== 'l')
    {
        const char *error_file_album = "resultados/albums.csv";
        writer_error_album = initialize_error_writer (error_file_album);
        escrever_cabecalho_album_erro (writer_error_album);
    }
    // Inicializa os writers de erro apenas uma vez no início

    // Escreve os cabeçalhos uma vez para cada tipo de erro (Diz no enunciado que é necessário)

    while (getline(&buffer, &buffer_size, file) != -1) // receber uma linha do ficheiro de cada vez, processando-a corretamente e evitando processar a primeira linha
    {

        if (first_line) // não copiar a primeira linha
        {
            first_line = FALSE;
            continue; // o continue faz com que o resto do loop seja ignorado, deste modo nao processamos a primeira linha e copiamos a segunda para o buffer
        }
        g_strstrip(buffer);

        if (tipo == 'a') // se o parser estiver a ser usado para processar um artista deve dar malloc a um novo artista e processar a linha atualmente contida no buffer
        {
            GestorArtistas *gestorartistas = get_gestor_artistas(gestor);
            Artista *artista = parse_csv_line_artista(buffer);
            // Artista *artista = inicializar_artista();
            if (artista)
            {
                inserir_artista(gestorartistas, artista); 
            }
            else
            {
                log_error(writer_error_artists, buffer);
                free_artista(artista);
            }
        }

        if (tipo == 'm') // mesma coisa para uma musica
        {
            GestorArtistas *gestorartistas = get_gestor_artistas(gestor);
            GestorMusicas *gestormusicas = get_gestor_musicas(gestor);
            GestorAlbuns *gestoralbuns = get_gestor_albuns (gestor);
            Musica *musica = parse_csv_line_musica(buffer, gestorartistas,gestoralbuns);
            if (musica)
            {
                inserir_musica(gestormusicas, musica);
            }
            else
            {
                log_error(writer_error_musics, buffer);
                free_musica(musica);
            }
        }

        if (tipo == 'u') // mesma coisa mas para um user
        {
            GestorUsuarios *gestorusuarios = get_gestor_usuarios(gestor);
            GestorMusicas *gestormusicas = get_gestor_musicas(gestor);
            Usuario *usuario = parse_csv_line_usuario(buffer, gestormusicas);
            if (usuario)
            {
                inserir_usuario(gestorusuarios, usuario);
            }
            else
            {

                log_error(writer_error_users, buffer);
                free_usuario(usuario);
            }
        }
        if (tipo== 'h')
        {
            GestorHistories *gestorhistories = get_gestor_histories (gestor);
            History *history = parse_csv_line_history (buffer, gestorhistories);
            if (history) {
                inserir_history(gestorhistories, history);
            }
            else {
                log_error (writer_error_history, buffer);
                free_history(history);
            }
        }
        if (tipo == 'l') {
            GestorAlbuns *gestoralbuns = get_gestor_albuns (gestor);
            Album *album = parse_csv_line_album (buffer, gestoralbuns);
            if (album) {
                inserir_album (gestoralbuns,album);
            } 
            else {
                log_error (writer_error_album, buffer); 
                free (album);
            }
        }
    }

    free(buffer);
    if (tipo == 'a')
        free_and_finish_writing(writer_error_artists);
    if (tipo == 'm')

        free_and_finish_writing(writer_error_musics);
    if (tipo == 'u')

        free_and_finish_writing(writer_error_users);
    if (tipo == 'h') 
        free_and_finish_writing (writer_error_history);
    if (tipo == 'l')
        free_and_finish_writing (writer_error_album);
}

Artista *parse_csv_line_artista(gchar *line)
{
    int numcampos = 7; // Número de campos a preencher
    gchar **tokens = g_strsplit(line, ";", numcampos);
    GPtrArray *campostemp = g_ptr_array_new_with_free_func(g_free); // Array temporario

    for (int i = 0; i < numcampos; i++)
    {

        GString *campo = g_string_new(tokens[i]); // copiar o primeiro token
        trim_quotes(campo);                       // retirar as aspas
        g_ptr_array_add(campostemp, g_string_free(campo, FALSE));
    }

    if (campostemp->len != numcampos)
    {
        g_ptr_array_free(campostemp, TRUE);
        g_strfreev(tokens);

        return NULL;
    }

    // Funcao auxiliar que Preenche o artista
    Artista *artista = preenche_artista(campostemp);
    // Libera memória usada
    if (!artista)
    {
        g_strfreev(tokens);
        g_ptr_array_free(campostemp, TRUE);
        return NULL;
    }
    g_strfreev(tokens);

    g_ptr_array_free(campostemp, TRUE);

    return artista;
}

Artista *preenche_artista(GPtrArray *campostemp)
{

    gchar *id_str = g_ptr_array_index(campostemp, 0);
    gchar *name = g_ptr_array_index(campostemp, 1);
    gchar *description = g_ptr_array_index(campostemp, 2);
    gchar *recipe_str = g_ptr_array_index(campostemp, 3);
    gchar *artist_ids_str = g_ptr_array_index(campostemp, 4);
    if (valida_parenteses_lista_artistas(artist_ids_str) == 0)
    {
        return NULL;
    }

    gchar *country = g_ptr_array_index(campostemp, 5);
    gchar *type = g_ptr_array_index(campostemp, 6);

    gdouble recipe_per_stream = atof(recipe_str);
    if ((strcmp(type, "individual") == 0) && strlen(artist_ids_str) != 2)
        return NULL;
    gchar **id_constituent = NULL;

    if (strcmp(type, "individual") != 0)
    {
        trim_parenteses_gchar(artist_ids_str);
        id_constituent = g_strsplit(artist_ids_str, ",", -1);
        for (int i = 0; id_constituent[i] != NULL; i++)
        {
            if (valida_single_quotes_lista_artistas(id_constituent[i]) == 0)
            {
                g_strfreev(id_constituent);
                return NULL;
            }
            trim_single_quotes_gchar(id_constituent[i]);
        }
    }

    Artista *artista = create_artista(id_str, name, description, recipe_per_stream, id_constituent, country, type);

    if (!artista || valida_artista_individual(artista) == FALSE || valida_artista_tipo (artista)== FALSE)
    {
        g_strfreev(id_constituent);
        return NULL;
    }

    g_strfreev(id_constituent);

    return artista;
}

Musica *parse_csv_line_musica(gchar *line, GestorArtistas *gestorartistas, GestorAlbuns *gestoralbuns)
{
    int numcampos = 8;
    gchar **tokens = g_strsplit(line, ";", numcampos);

    GPtrArray *campostemp = g_ptr_array_new_with_free_func(g_free); // Array temporario

    for (int i = 0; i < numcampos; i++)
    {

        GString *campo = g_string_new(tokens[i]); // copiar o primeiro token
        trim_quotes(campo);                       // retirar as aspas
        g_ptr_array_add(campostemp, g_string_free(campo, FALSE));
    }

    if (campostemp->len != numcampos)
    {
        g_ptr_array_free(campostemp, TRUE);
        g_strfreev(tokens);

        return NULL;
    }

    Musica *musica = preenche_musica(campostemp, gestorartistas,gestoralbuns);

    if (!musica)
    {
        g_ptr_array_free(campostemp, TRUE);
        g_strfreev(tokens);

        return NULL;
    }

    g_ptr_array_free(campostemp, TRUE);
    g_strfreev(tokens);

    return musica; // Retorna 1 se o parsing foi bem-sucedido
}

Musica *preenche_musica(GPtrArray *campostemp, GestorArtistas *gestorartistas, GestorAlbuns *gestoralbuns)
{

    gchar *id = g_ptr_array_index(campostemp, 0);

    gchar *title = g_ptr_array_index(campostemp, 1);

    gchar *artist_ids_str = g_ptr_array_index(campostemp, 2);
    if (valida_parenteses_lista_artistas(artist_ids_str) == 0)
    {
        return NULL;
    }

    trim_parenteses_gchar(artist_ids_str);
    gchar **artist_ids = g_strsplit(artist_ids_str, ",", -1);
    for (int i = 0; artist_ids[i] != NULL; i++)
    {
        if (valida_single_quotes_lista_artistas(artist_ids[i]) == 0)
        {
            g_strfreev(artist_ids);
            return NULL;
        }
        trim_single_quotes_gchar(artist_ids[i]);
    }
    gchar *album_id = g_ptr_array_index (campostemp,3);

    gchar *duration = g_ptr_array_index(campostemp, 4);

    gchar *genre = g_ptr_array_index(campostemp, 5);

    gchar *year_str = g_ptr_array_index(campostemp, 6);

    int year = atoi(year_str);

    gchar *lyrics = g_ptr_array_index(campostemp, 7);

    Musica *musica = create_musica(id, title, artist_ids, album_id,duration, genre, year, lyrics);

    if (!musica)
    {
        free_musica(musica);
        g_strfreev(artist_ids);
        return NULL;
    }

    if (validaDuracao(musica) == FALSE || valida_ano_lançamento(musica) == 0 || valida_artistids_musica(musica, gestorartistas) == 0 || valida_album (musica, gestoralbuns))
    {
        free_musica(musica);
        g_strfreev(artist_ids);
        return NULL;
    }

    g_strfreev(artist_ids);
    return musica;
}

// Parser para usuarios
Usuario *parse_csv_line_usuario(gchar *line, GestorMusicas *gestormusicas)
{
    int numcampos = 8;
    gchar **tokens = g_strsplit(line, ";", numcampos);

    GPtrArray *campostemp = g_ptr_array_new_with_free_func(g_free); // Array temporario

    for (int i = 0; i < numcampos; i++)
    {

        GString *campo = g_string_new(tokens[i]); // copiar o primeiro token
        trim_quotes(campo);                       // retirar as aspas
        g_ptr_array_add(campostemp, g_string_free(campo, FALSE));
    }

    if (campostemp->len != numcampos)
    {
        g_ptr_array_free(campostemp, TRUE);
        g_strfreev(tokens);

        return NULL;
    }

    // Funcao auxiliar que Preenche o artista

    Usuario *usuario = preenche_usuario(campostemp, gestormusicas);

    if (!usuario)
    {
        g_ptr_array_free(campostemp, TRUE);
        g_strfreev(tokens);

        return NULL;
    }

    g_ptr_array_free(campostemp, TRUE);
    g_strfreev(tokens);

    return usuario; // Retorna 1 se o parsing foi bem-sucedido
}

Usuario *preenche_usuario(GPtrArray *campostemp, GestorMusicas *gestormusicas)
{
    // Recupera os valores de cada campo do GPtrArray
    gchar *username = g_ptr_array_index(campostemp, 0);
    gchar *email = g_ptr_array_index(campostemp, 1);
    gchar *first_name = g_ptr_array_index(campostemp, 2);
    gchar *last_name = g_ptr_array_index(campostemp, 3);
    gchar *birth_date = g_ptr_array_index(campostemp, 4);
    gchar *country = g_ptr_array_index(campostemp, 5);
    gchar *subscription_type = g_ptr_array_index(campostemp, 6);
    gchar *liked_musics_str = g_ptr_array_index(campostemp, 7);
    if (valida_parenteses_lista_artistas(liked_musics_str) == 0)
    {
        printf("erro nos []\n");
        return NULL;
    }
    trim_parenteses_gchar(liked_musics_str);
    gchar **liked_musics_id = g_strsplit(liked_musics_str, ",", -1);
    for (int i = 0; liked_musics_id[i] != NULL; i++)
    {
        if (valida_single_quotes_lista_artistas(liked_musics_id[i]) == 0)
        {
            printf("erro nos ''\n");
            g_strfreev(liked_musics_id);
            return NULL;
        }
        trim_single_quotes_gchar(liked_musics_id[i]);
    }

    Usuario *usuario = create_usuario(username, email, first_name, last_name, birth_date, country, subscription_type, liked_musics_id);

    g_strfreev(liked_musics_id);

    if (valida_user(usuario, gestormusicas) == FALSE)
    {
        free_usuario(usuario);
        return NULL;
    }

    return usuario;
}


Album *parse_csv_line_album(gchar *line, GestorAlbuns *gestor_albuns)
{
    int numcampos = 5;
    gchar **tokens = g_strsplit(line, ";", numcampos);

    GPtrArray *campostemp = g_ptr_array_new_with_free_func(g_free); // Array temporario

    for (int i = 0; i < numcampos; i++)
    {

        GString *campo = g_string_new(tokens[i]); // copiar o primeiro token
        trim_quotes(campo);                       // retirar as aspas
        g_ptr_array_add(campostemp, g_string_free(campo, FALSE));
    }

    if (campostemp->len != numcampos)
    {
        g_ptr_array_free(campostemp, TRUE);
        g_strfreev(tokens);

        return NULL;
    }

    // Funcao auxiliar que Preenche o artista

    Album *album = preenche_album(campostemp, gestor_albuns);

    if (!album)
    {
        g_ptr_array_free(campostemp, TRUE);
        g_strfreev(tokens);

        return NULL;
    }

    g_ptr_array_free(campostemp, TRUE);
    g_strfreev(tokens);

    return album; // Retorna 1 se o parsing foi bem-sucedido
}

Album *preenche_album(GPtrArray *campostemp, GestorAlbuns *gestoralbuns)
{
    // Recupera os valores de cada campo do GPtrArray
    gchar *id = g_ptr_array_index(campostemp, 0);
    gchar *tittle = g_ptr_array_index(campostemp, 1);
     gchar *artist_ids_str = g_ptr_array_index(campostemp, 2);
    if (valida_parenteses_lista_artistas(artist_ids_str) == 0)
    {
        return NULL;
    }

    trim_parenteses_gchar(artist_ids_str);
    gchar **artist_ids = g_strsplit(artist_ids_str, ",", -1);
    for (int i = 0; artist_ids[i] != NULL; i++)
    {
        if (valida_single_quotes_lista_artistas(artist_ids[i]) == 0)
        {
            g_strfreev(artist_ids);
            return NULL;
        }
        trim_single_quotes_gchar(artist_ids[i]);
    }

    gchar *year = g_ptr_array_index(campostemp, 3);

    gchar *producers = g_ptr_array_index(campostemp, 4);
  
    if (valida_parenteses_lista_artistas(producers) == 0)
    {
        printf("erro nos []\n");
        return NULL;
    }
    trim_parenteses_gchar(producers);
    gchar **producers_name = g_strsplit(producers, ",", -1);
    for (int i = 0; producers_name[i] != NULL; i++)
    {
        if (valida_single_quotes_lista_artistas(producers_name[i]) == 0)
        {
            printf("erro nos ''\n");
            g_strfreev(producers_name);
            return NULL;
        }
        trim_single_quotes_gchar(producers_name[i]);
    }

    Album *album = create_album(id, tittle, artist_ids, year, producers_name);

    g_strfreev(producers_name);
    g_strfreev (artist_ids);


   // faltam as validacoes dos albuns

    return album;
}



History *parse_csv_line_history(gchar *line, GestorHistories *gestorhistory)
{
    int numcampos = 6;
    gchar **tokens = g_strsplit(line, ";", numcampos);

    GPtrArray *campostemp = g_ptr_array_new_with_free_func(g_free); // Array temporario

    for (int i = 0; i < numcampos; i++)


    {

        GString *campo = g_string_new(tokens[i]); // copiar o primeiro token
        trim_quotes(campo);                       // retirar as aspas
        g_ptr_array_add(campostemp, g_string_free(campo, FALSE));
    }

    if (campostemp->len != numcampos)
    {
        g_ptr_array_free(campostemp, TRUE);
        g_strfreev(tokens);

        return NULL;
    }

    // Funcao auxiliar que Preenche o artista

    History *history = preenche_history(campostemp, gestorhistory);

    if (!history)
    {
        g_ptr_array_free(campostemp, TRUE);
        g_strfreev(tokens);

        return NULL;
    }

    g_ptr_array_free(campostemp, TRUE);
    g_strfreev(tokens);

    return history; // Retorna 1 se o parsing foi bem-sucedido
}

History *preenche_history(GPtrArray *campostemp, GestorHistories *gestorhistory)
{
    // Recupera os valores de cada campo do GPtrArray
    gchar *id = g_ptr_array_index(campostemp, 0);
    gchar *user_id = g_ptr_array_index(campostemp, 1);
    gchar *music_id = g_ptr_array_index(campostemp, 2);
    gchar *timestamp = g_ptr_array_index(campostemp, 3);
    gchar *duration = g_ptr_array_index(campostemp, 4);
    gchar *plataform = g_ptr_array_index(campostemp, 5);
    
    History *history = create_history(id, user_id, music_id, timestamp, duration, plataform);


    if (valida_plataforma(history) == FALSE)
    {
        free_history(history);
        return NULL;
    }

    return history;
}
