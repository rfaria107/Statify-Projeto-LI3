#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "../include/parsing/parser.h"
#include "../include/parsing/string_utils.h"
#include "../include/entidades/artists.h"
#include "../include/entidades/musica.h"
#include "../include/entidades/usuario.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/parsing/rowreader.h"

void parser_principal(FILE *file, char tipo)
{
    char buffer[BUFSIZ];
    gboolean first_line = TRUE;
    RowReader *reader = initialize_row_reader(buffer, ';');
    while (fgets(buffer, sizeof(buffer), file)) // receber uma linha do ficheiro de cada vez, processando-a corretamente e evitando processar a primeira linha
    {
        if (first_line) // não copiar a primeira linha
        {
            first_line = FALSE;
            continue; // o continue faz com que o resto do loop seja ignorado, deste modo nao processamos a primeira linha e copiamos a segunda para o buffer
        }

        reader_set_row(reader, buffer); // colocar o reader na linha atual

        if (tipo == 'a') // se o parser estiver a ser usado para processar um artista deve dar malloc a um novo artista e processar a linha atualmente contida no buffer
        {
            Artista *artista = g_malloc(sizeof(Artista));
            parse_csv_line_artista(reader, artista);
        }

        if (tipo == 'u') // mesma coisa mas para um user
        {
            Usuario *usuario = g_malloc(sizeof(Usuario));
            parse_csv_line_usuario(reader, usuario);
        }

        if (tipo == 'm') // mesma coisa para uma musica
        {
            Musica *musica = g_malloc(sizeof(Musica));
            parse_csv_line_musica(reader, musica);
        }
    }
    free_row_reader(reader);
}

// Parser para usuarios
int parse_csv_line_usuario(RowReader *reader, Usuario *usuario)
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
            return 0; // Falha no parsing se o número de células for menor que o esperado
        }
        GString *campo = g_string_new(cell);
        trim_quotes(campo); // Remove aspas ao redor dos valores
        g_ptr_array_add(campostemp, g_string_free(campo, FALSE));
    }

    // Verifica se o número de campos é o esperado
    if (campostemp->len != numcampos)
    {
        g_ptr_array_free(campostemp, TRUE);
        return 0;
    }

    g_ptr_array_free(campostemp, TRUE);

    return 1; // Retorna 1 se o parsing foi bem-sucedido
}

int parse_csv_line_artista(RowReader *reader, Artista *artista)
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
            return 0; // Falha no parsing se o número de células for menor que o esperado
        }
        GString *campo = g_string_new(cell);
        trim_quotes(campo); // Remove aspas ao redor dos valores
        g_ptr_array_add(campostemp, g_string_free(campo, FALSE));
    }

    if (campostemp->len != numcampos)
    {
        g_ptr_array_free(campostemp, TRUE);
        return 0;
    }

    // Funcao auxiliar que Preenche o artista
    preencher_artista(artista, campostemp);

    // Insere o artista no gestor de artistas
    inserir_artista(gestorartistas, *artista);

    // Libera memória usada
    g_ptr_array_free(campostemp, TRUE);

    return 1;
}

int parse_csv_line_musica(RowReader *reader, Musica *musica)
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
            return 0; // Falha no parsing se o númerio de células for menor que o esperado
        }
        GString *campo = g_string_new(cell);
        trim_quotes(campo); // Remove aspas ao redor dos valores
        g_ptr_array_add(campostemp, g_string_free(campo, FALSE));
    }

    // Verifica se o número de campos é o esperado
    if (campostemp->len != numcampos)
    {
        g_ptr_array_free(campostemp, TRUE);
        return 0;
    }

    *musica = preenche_musica(campostemp);

    g_ptr_array_free(campostemp, TRUE);
    
    return 1; // Retorna 1 se o parsing foi bem-sucedido
}

Musica *preenche_musica(GPtrArray *campostemp)
{

    gchar *id_str = g_ptr_array_index(campostemp, 0);
    if (is_empty_value(id_str))
        return NULL;
    int id = atoi(id_str);

    gchar *title = g_ptr_array_index(campostemp, 1);
    if (invalid_email(title))
        return NULL;

    gchar *artist_ids_str = g_ptr_array_index(campostemp, 2);
    if (is_empty_value(artist_ids_str))
        return NULL;
    gchar **artist_ids = g_strsplit(artist_ids_str, ",", -1);

    gchar *duration = g_ptr_array_index(campostemp, 3);
    if (is_empty_value(duration))
        return NULL;

    gchar *genre = g_ptr_array_index(campostemp, 4);
    if (invalid_date(genre))
        return NULL;

    gchar *year_str = g_ptr_array_index(campostemp, 5);
    if (is_empty_value(year_str))
        return NULL;
    int year = atoi(year_str);

    gchar *lyrics = g_ptr_array_index(campostemp, 6);
    if (is_empty_value(lyrics))
        return NULL;

    Musica *musica = create_musica(id, title, artist_ids, duration, genre, year, lyrics);

    if (!musica)
        return NULL;

    // inicializar_musica(gestorMusic, musica);

    // free_musica(musica);

    return musica;
}