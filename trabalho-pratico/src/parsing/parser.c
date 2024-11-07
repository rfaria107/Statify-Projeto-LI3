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

void parser_principal(FILE *file, GestorSistema *gestor, char tipo)
{
    GestorArtistas *gestorartistas = get_gestor_artistas(gestor);
    GestorUsuarios *gestorusuarios = get_gestor_usuarios(gestor);
    GestorMusicas *gestormusicas = get_gestor_musicas(gestor);

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
            Artista *artista = NULL;
            // Artista *artista = inicializar_artista();
            if ((parse_csv_line_artista(reader, artista)) == 0)
            {
                // escreve erro
                free_artista(artista);
            }
            else
                inserir_artista(gestorartistas, artista);
        }

        if (tipo == 'u') // mesma coisa mas para um user
        {
            Usuario *usuario = NULL;
            // Usuario *usuario = inicializar_usuario();
            parse_csv_line_usuario(reader, usuario, gestormusicas);
            inserir_usuario(gestorusuarios, usuario);
        }

        if (tipo == 'm') // mesma coisa para uma musica
        {
            Musica *musica = NULL;
            // Musica *musica = inicializar_musica();
            parse_csv_line_musica(reader, musica);
            inserir_musica(gestormusicas, musica);
        }
    }
    free_row_reader(reader);
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
            // NOTA adicionar writer aqui
            return 0; // Falha no parsing se alguma celula estiver vazia
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
    artista = preenche_artista(campostemp, artista);

    if (!artista)
        return 0;

    // Libera memória usada
    g_ptr_array_free(campostemp, TRUE);

    return 1;
}

Artista *preenche_artista(GPtrArray *campostemp, Artista *artista)
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

    artista = create_artista(id_str, name, description, recipe_per_stream, id_constituent, country, type);

    if (!artista)
        return NULL;

    return artista;
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
            return 0; // Falha no parsing se alguma celula estiver vazia
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

    musica = preenche_musica(campostemp, musica);

    if (!musica)
        return 0;

    g_ptr_array_free(campostemp, TRUE);

    return 1; // Retorna 1 se o parsing foi bem-sucedido
}

int preenche_musica(GPtrArray *campostemp, Musica *musica)
{

    gchar *id_str = g_ptr_array_index(campostemp, 0);

    int id = atoi(id_str);

    gchar *title = g_ptr_array_index(campostemp, 1);

    gchar *artist_ids_str = g_ptr_array_index(campostemp, 2);

    gchar **artist_ids = g_strsplit(artist_ids_str, ",", -1);

    gchar *duration = g_ptr_array_index(campostemp, 3);

    gchar *genre = g_ptr_array_index(campostemp, 4);

    gchar *year_str = g_ptr_array_index(campostemp, 5);

    int year = atoi(year_str);

    gchar *lyrics = g_ptr_array_index(campostemp, 6);

    if (validaDuracao(duration) == FALSE)
        return NULL;

    musica = create_musica(id, title, artist_ids, duration, genre, year, lyrics);

    if (!musica)
        return NULL;

    return musica;
}

// Parser para usuarios
int parse_csv_line_usuario(RowReader *reader, Usuario *usuario, GestorMusicas *gestormusicas)
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
            return 0; // Falha no parsing se alguma celula estiver vazia
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

    // Funcao auxiliar que Preenche o artista
    usuario = preenche_usuario(campostemp, gestormusicas);

    if (preenche_usuario == NULL)
        return 0;
    g_ptr_array_free(campostemp, TRUE);

    return 1; // Retorna 1 se o parsing foi bem-sucedido
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
