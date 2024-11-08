#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "../include/parsing/parser.h"
#include "../include/parsing/string_utils.h"
#include "../include/entidades/artists.h"
#include "../include/entidades/musica.h"
#include "../include/entidades/usuario.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/gestores/gestor_artistas.h"
#include "../include/parsing/rowreader.h"
#include "../include/validacao/valida.h"
#include "../include/parsing/writer.h"

void parser_principal(FILE *file, GestorSistema *gestor, char tipo)
{
    char *buffer = NULL;
    size_t buffer_size = 0;
    gboolean first_line = TRUE;
    RowReader *reader = initialize_row_reader(buffer, ';');
    const char *error_file_artists = "artists_errors.csv";
    const char *error_file_musics = "musics_errors.csv";
    const char *error_file_users = "users_errors.csv";

    // Inicializa os writers de erro apenas uma vez no início
    RowWriter *writer_error_artists = initialize_error_writer(error_file_artists);
    RowWriter *writer_error_musics = initialize_error_writer(error_file_musics);
    RowWriter *writer_error_users = initialize_error_writer(error_file_users);

    // Escreve os cabeçalhos uma vez para cada tipo de erro (Diz no enunciado que é necessário)
    escrever_cabecalho_artists_erro(writer_error_artists);
    escrever_cabecalho_musics_erro(writer_error_musics);
    escrever_cabecalho_users_erro(writer_error_users);

    while (getline(&buffer, &buffer_size, file) != -1) // receber uma linha do ficheiro de cada vez, processando-a corretamente e evitando processar a primeira linha
    {

        if (first_line) // não copiar a primeira linha
        {
            first_line = FALSE;
            continue; // o continue faz com que o resto do loop seja ignorado, deste modo nao processamos a primeira linha e copiamos a segunda para o buffer
        }
        buffer = g_strstrip(buffer);
        reader_set_row(reader, buffer); // colocar o reader na linha atual

        if (tipo == 'a') // se o parser estiver a ser usado para processar um artista deve dar malloc a um novo artista e processar a linha atualmente contida no buffer
        {
            GestorArtistas *gestorartistas = get_gestor_artistas(gestor);
            Artista *artista = parse_csv_line_artista(reader);
            // Artista *artista = inicializar_artista();
            if (artista)
            {
                inserir_artista(gestorartistas, artista);
            }
            else
            {
            
                log_error (writer_error_artists, buffer);
                free_artista(artista);    
            }
        }

        if (tipo == 'm') // mesma coisa para uma musica
        {

            GestorMusicas *gestormusicas = get_gestor_musicas(gestor);
            Musica *musica = parse_csv_line_musica(reader);
            if (musica)
            {
                inserir_musica(gestormusicas, musica);
            }
            else
            {
                
                log_error (writer_error_musics, buffer);                
                free_musica(musica);
                
            }
        }

        if (tipo == 'u') // mesma coisa mas para um user
        {
            GestorUsuarios *gestorusuarios = get_gestor_usuarios(gestor);
            GestorMusicas *gestormusicas = get_gestor_musicas(gestor);
            Usuario *usuario = parse_csv_line_usuario(reader, gestormusicas);
            if (usuario)
            {
                inserir_usuario(gestorusuarios, usuario);
            }
            else
            {
               
                log_error (writer_error_users, buffer);                
                free_usuario(usuario);
                
            }
        }
    }
    free(buffer);
    free_row_reader(reader);
    free_and_finish_writing(writer_error_artists);
    free_and_finish_writing(writer_error_musics);
    free_and_finish_writing(writer_error_users);
}

Artista *parse_csv_line_artista(RowReader *reader)
{
    int numcampos = 7; // Número de campos a preencher
    char *cell;

    GPtrArray *campostemp = g_ptr_array_new_with_free_func(g_free); // Array temporario

    for (int i = 0; i < numcampos; i++)
    {
        cell = reader_next_cell(reader);
        if (cell == NULL || *cell == '\0')
        {
            g_ptr_array_free(campostemp, TRUE);
            // NOTA adicionar writer aqui
            return NULL; // Falha no parsing se alguma celula estiver vazia
        }
        GString *campo = g_string_new(cell);
        trim_quotes(campo); // Remove aspas ao redor dos valores
        g_ptr_array_add(campostemp, g_string_free(campo, FALSE));
    }

    if (campostemp->len != numcampos)
    {
        g_ptr_array_free(campostemp, TRUE);
        return NULL;
    }

    // Funcao auxiliar que Preenche o artista
    Artista *artista = preenche_artista(campostemp);
    // Libera memória usada
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
    gchar *country = g_ptr_array_index(campostemp, 5);
    gchar *type = g_ptr_array_index(campostemp, 6);

    gdouble recipe_per_stream = atof(recipe_str);

    gchar **id_constituent = g_strsplit(artist_ids_str, ",", -1);

    Artista *artista = create_artista(id_str, name, description, recipe_per_stream, id_constituent, country, type);

    if (!artista)
    {
        g_strfreev(id_constituent);
        return NULL;
    }
    return artista;
}

Musica *parse_csv_line_musica(RowReader *reader)
{
    int numcampos = 7;
    char *cell;

    GPtrArray *campostemp = g_ptr_array_new_with_free_func(g_free); // Array temporario

    for (int i = 0; i < numcampos; i++)
    {
        cell = reader_next_cell(reader);
        if (cell == NULL)
        {
            g_ptr_array_free(campostemp, TRUE);
            return NULL; // Falha no parsing se alguma celula estiver vazia
        }
        GString *campo = g_string_new(cell);
        trim_quotes(campo); // Remove aspas ao redor dos valores
        g_ptr_array_add(campostemp, g_string_free(campo, FALSE));
    }

    // Verifica se o número de campos é o esperado
    if (campostemp->len != numcampos)
    {
        g_ptr_array_free(campostemp, TRUE);
        return NULL;
    }

    Musica *musica = preenche_musica(campostemp);

    if (!musica)
        return NULL;

    g_ptr_array_free(campostemp, TRUE);

    return musica; // Retorna 1 se o parsing foi bem-sucedido
}

Musica *preenche_musica(GPtrArray *campostemp)
{

    gchar *id_str = g_ptr_array_index(campostemp, 0);

    int id = atoi(id_str);

    gchar *title = g_ptr_array_index(campostemp, 1);

    gchar *artist_ids_str = g_ptr_array_index(campostemp, 2);
    if (valida_parenteses_lista_artistas(artist_ids_str) == 0)
        return NULL;

    trim_parenteses_gchar(artist_ids_str);
    gchar **artist_ids = g_strsplit(artist_ids_str, ",", -1);
    for (int i = 0; artist_ids[i] != NULL; i++)
    {
        if (valida_single_quotes_lista_artistas(artist_ids[i]))
            return NULL;
        trim_single_quotes_gchar(artist_ids[i]);
        printf("%s\n",artist_ids[i]);
    }
    gchar *duration = g_ptr_array_index(campostemp, 3);

    gchar *genre = g_ptr_array_index(campostemp, 4);

    gchar *year_str = g_ptr_array_index(campostemp, 5);

    int year = atoi(year_str);

    gchar *lyrics = g_ptr_array_index(campostemp, 6);

    Musica *musica = create_musica(id, title, artist_ids, duration, genre, year, lyrics);

    if (validaDuracao(musica) == FALSE)
        return NULL;

    if (!musica)
        return NULL;

    return musica;
}

// Parser para usuarios
Usuario *parse_csv_line_usuario(RowReader *reader, GestorMusicas *gestormusicas)
{
    int numcampos = 8;
    char *cell;

    GPtrArray *campostemp = g_ptr_array_new_with_free_func(g_free); // Array temporario

    for (int i = 0; i < numcampos; i++)
    {
        cell = reader_next_cell(reader);
        if (cell == NULL || *cell == '\0')
        {
            g_ptr_array_free(campostemp, TRUE);
            return NULL; // Falha no parsing se alguma celula estiver vazia
        }
        GString *campo = g_string_new(cell);
        trim_quotes(campo); // Remove aspas ao redor dos valores
        g_ptr_array_add(campostemp, g_string_free(campo, FALSE));
    }

    // Verifica se o número de campos é o esperado
    if (campostemp->len != numcampos)
    {
        g_ptr_array_free(campostemp, TRUE);
        return NULL;
    }

    // Funcao auxiliar que Preenche o artista

    Usuario *usuario = preenche_usuario(campostemp, gestormusicas);
    
    if (!usuario) return NULL;
    g_ptr_array_free(campostemp, TRUE);

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

    gchar **liked_musics_id = g_strsplit(liked_musics_str, ",", -1);

    Usuario *usuario = create_usuario(username, email, first_name, last_name, birth_date, country, subscription_type, liked_musics_id);

    if (valida_user(usuario, gestormusicas) == FALSE)
        return NULL;

    if (!usuario)
        return NULL;

    return usuario;
}
