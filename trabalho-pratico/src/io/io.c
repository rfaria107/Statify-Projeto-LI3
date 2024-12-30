#include <stdio.h>
#include "../include/io/io.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/queries/queries.h"
#include "../include/queries/queries_aux.h"
#include "../include/parsing/parser.h"
#include "../include/utils/string_utils.h"

int open_file(int argc, char *argv[])
{

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
    if (fileinputs)
    {
        interpreter_inputs(fileinputs, gestor);
        fclose(fileinputs);
    }
    else
    {
        fprintf(stderr, "Error opening file: %s\n", pathinputs);
    }
    g_free(pathinputs);

    g_free(path);
    liberar_gestor_sistema(gestor);

    return 0;
}

void process_file(const char *base_path, const char *file_name, GestorSistema *gestor, char tipo)
{
    char *file_path = g_strconcat(base_path, "/", file_name, NULL);
    FILE *file = fopen(file_path, "r");
    if (file)
    {
        parser_principal(file, gestor, tipo);
        fclose(file);
    }
    else
    {
        fprintf(stderr, "Error opening file: %s\n", file_path);
    }
    g_free(file_path);
}

void interpreter_inputs(FILE *file, GestorSistema *gestorsis)
{
    char *buffer = "";
    size_t buffer_size = 0;
    int line_number = 1;
    // necessário para a query 2 no entanto apenas deve executar uma vez
    calcular_discografia_artistas(gestorsis);
    calcula_streams(gestorsis);

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
                query_1(gestorsis, token, line_number, 0);
                g_free(token);
            }
        }

        else if (strcmp(token, "1S") == 0)
        {
            g_free(token);
            token = procura_espaço2(buffer);
            if (token != NULL)
            {
                query_1(gestorsis, token, line_number, 1);
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
        else
            g_free(token);
        line_number++;
    }
    g_free(buffer);
}