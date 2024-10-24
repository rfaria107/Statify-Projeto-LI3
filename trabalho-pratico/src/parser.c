#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "parser.h"
#include "../include/string_utils.h"
#include "../include/entidades/artists.h"
#include "../include/gestores/gestor_sistemas.h"

void parser_principal(FILE *file, GestorSistema *gestorsis, char tipo)
{
    char buffer[BUFSIZ];
    gboolean first_line = TRUE;
    while (fgets(buffer, sizeof(buffer), file)) // receber uma linha do ficheiro de cada vez, processando-a corretamente e evitando processar a primeira linha
    {
        if (first_line) // não copiar a primeira linha
        {
            first_line = FALSE;
            continue; // o continue faz com que o resto do loop seja ignorado, deste modo nao processamos a primeira linha e copiamos a segunda para o buffer
        }

        if (tipo == 'a') // se o parser estiver a ser usado para processar um artista deve dar malloc a um novo artista e processar a linha atualmente contida no buffer
        {
            Artista *artista = g_malloc(sizeof(artista));
            parse_csv_line_artista(&(gestorsis->gestor_artistas), buffer, artista);
        }

        if (tipo == 'u') // mesma coisa mas para um user
        {
        }

        if (tipo == 'm') // mesma coisa para uma musica
        {
        }
    }
}

void parse_csv_line_artista(GestorArtistas *gestorartistas, gchar *linha, Artista *artista)
{                                                                   // função para preencher o struct de um artista a partir de uma linha de texto
    gint numcampos = 7;                                             // campos a preencher
    gchar **tokens = g_strsplit(linha, ";", numcampos);             // dividir a linha em tokens pelos ";"
    GPtrArray *campostemp = g_ptr_array_new_with_free_func(g_free); // criar um array de pointers onde ficarão temporariamente os campos do struct artista

    for (gint i = 0; tokens[i] != NULL; i++)
    {                                                             // iterar sobre os tokens para preencher o array de pointers
        GString *campo = g_string_new(tokens[i]);                 // copiar o primeiro token
        trim_quotes(campo);                                       // retirar as aspas
        g_ptr_array_add(campostemp, g_string_free(campo, FALSE)); // guardar o token já processado num array de pointers(glib), gstring free limpa os campos da gstring mas retorna o campo com gchar*, assim podemos limpar a gstring da memoria sem perder o conteudo, que fica guardado em campostemp pra ser copiado para o artista
    }

    if (campostemp->len != numcampos)
    { // verificar se temos todos os campos antes de preencher o artista
        g_ptr_array_free(campostemp, TRUE);
        g_strfreev(tokens);
        return;
    }

    // preencher o artista
    (artista)->id = g_strdup(g_ptr_array_index(campostemp, 0));
    (artista)->name = g_strdup(g_ptr_array_index(campostemp, 1));
    (artista)->description = g_strdup(g_ptr_array_index(campostemp, 2));
    (artista)->recipe_per_stream = g_ascii_strtod(g_ptr_array_index(campostemp, 3), NULL);
    // falta o constituent count
    (artista)->country = g_strdup(g_ptr_array_index(campostemp, 5));
    (artista)->type = g_strdup(g_ptr_array_index(campostemp, 6));

    inserir_artista(gestorartistas, *artista);

    g_ptr_array_free(campostemp, TRUE);
    g_strfreev(tokens);
}
