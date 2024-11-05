#include <glib.h>
#include <stdio.h>
#include <string.h>
#include "../include/parsing/parser.h"
#include "../include/parsing/string_utils.h"
#include "../include/entidades/artists.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/parsing/rowreader.h"
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
            Usuario *usuario = g_malloc (sizeof (usuario));
            parse_csv_line_usuario (&(gestorsis->gestor_usuarios),buffer, usuario);
        }

        if (tipo == 'm') // mesma coisa para uma musica
        {
        }
    }
}

// Parser para usuarios
int parse_csv_line_usuario(GestorUsuarios *gestorusuarios, RowReader *reader, Usuario *usuario) {
    int numcampos = 8;
    char *cell;

    GPtrArray *campostemp = g_ptr_array_new_with_free_func(g_free); // Array temporario

    for (int i = 0; i < numcampos; i++) {
        cell = reader_next_cell(reader);
        if (cell == NULL) {
            g_ptr_array_free(campostemp, TRUE);
            return 0; // Falha no parsing se o númerio de células for menor que o esperado
        }
        GString *campo = g_string_new(cell);
        trim_quotes(campo);  // Remove aspas ao redor dos valores
        g_ptr_array_add(campostemp, g_string_free(campo, FALSE));
    }

    // Verifica se o número de campos é o esperado
    if (campostemp->len != numcampos) {
        g_ptr_array_free(campostemp, TRUE);
        return 0;
    }

    preencher_usuario(usuario, campostemp);
    inserir_usuario(gestorusuarios, usuario);

    g_ptr_array_free(campostemp, TRUE);
    return 1;  // Retorna 1 se o parsing foi bem-sucedido
}


void parse_csv_line_artista(GestorArtistas *gestorartistas, gchar *linha, Artista *artista) {
    gint numcampos = 7; // Número de campos a preencher
    gchar **tokens = g_strsplit(linha, ";", numcampos); // Divide a linha em tokens usando ";"
    GPtrArray *campostemp = g_ptr_array_new_with_free_func(g_free); // Array temporário para campos do artista

    for (gint i = 0; tokens[i] != NULL; i++) {
        GString *campo = g_string_new(tokens[i]);
        trim_quotes(campo); // Retira aspas de cada token
        g_ptr_array_add(campostemp, g_string_free(campo, FALSE));
    }

    if (campostemp->len != numcampos) {
        g_ptr_array_free(campostemp, TRUE);
        g_strfreev(tokens);
        return;
    }

//Funcao auxiliar que Preenche o artista   
  preencher_artista(artista, campostemp);

    // Insere o artista no gestor de artistas
    inserir_artista(gestorartistas, *artista);

    // Libera memória usada
    g_ptr_array_free(campostemp, TRUE);
    g_strfreev(tokens);
}



int parse_usuario_and_add_to_catalog(RowReader* reader, void* catalog, void* database) {
    // Ler o nome de usuário
    char* username = reader_next_cell(reader);
    if (is_empty_value(username)) return 1;

    // Ler o email
    char* email = reader_next_cell(reader);
    if (is_empty_value(email)) return 1;

    // Ler o primeiro nome
    char* first_name = reader_next_cell(reader);
    if (is_empty_value(first_name)) return 1;

    // Ler o sobrenome
    char* last_name = reader_next_cell(reader);
    if (is_empty_value(last_name)) return 1;

    // Ler a data de nascimento
    char* birth_date = reader_next_cell(reader);
    if (is_empty_value(birth_date)) return 1;

    // Ler o país
    char* country = reader_next_cell(reader);
    if (is_empty_value(country)) return 1;

    // Ler o tipo de assinatura
    char* subscription_type = reader_next_cell(reader);
    if (is_empty_value(subscription_type)) return 1;

    // Ler os IDs das músicas curtidas
    char* liked_musics_str = reader_next_cell(reader);
    gchar** liked_musics_id = NULL; // Inicializa como NULL

    if (!is_empty_value(liked_musics_str)) {
        // Divide a string de IDs de músicas usando a vírgula como delimitador
        liked_musics_id = g_strsplit(liked_musics_str, ",", 0);
    }

    // Criar um novo usuário
    Usuario* new_user = create_usuario(username, email, first_name, last_name, birth_date, country, subscription_type);
    
    // Adicionar IDs de músicas curtidas
    if (liked_musics_id) {
        for (int i = 0; liked_musics_id[i] != NULL; i++) {
            // Aqui, você deve criar uma função que adiciona cada música à lista de músicas curtidas do usuário
            add_liked_music(new_user, liked_musics_id[i]);
        }
        g_strfreev(liked_musics_id); // Liberar memória após uso
    }

    // Adicionar o novo usuário ao catálogo
    add_usuario_to_catalog(catalog, new_user);
    
    return 0;
}



