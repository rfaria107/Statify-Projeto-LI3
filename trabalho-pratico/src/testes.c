#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <dirent.h>
#include <sys/resource.h>
#include <time.h>
#include "../include/gestores/gestor_sistemas.h"
#include "../include/queries/queries.h"
#include "../include/queries/queries_aux.h"
#include "../include/queries/query5_aux.h"
#include "../include/parsing/parser.h"
#include "../include/utils/string_utils.h"
#include "../include/io/io.h"

#define MAX_PATH 1024

// Função para medir o uso de memória (em megabytes)
long get_memory_usage_mb() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss / 1024;  // Convertendo para megabytes
}

// Função para comparar os arquivos
int compare_txt_files(const char *file1, const char *file2, GList **discrepancies) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");

    if (!f1 || !f2) {
        perror("Erro ao abrir arquivos");
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return -1;
    }

    int line_number = 1;
    char line1[1024], line2[1024];

    // Comparar linha por linha
    while (fgets(line1, sizeof(line1), f1) != NULL && fgets(line2, sizeof(line2), f2) != NULL) {
        // Remover novas linhas e espaços extras nas extremidades
        line1[strcspn(line1, "\n")] = 0;
        line2[strcspn(line2, "\n")] = 0;

        if (strcmp(line1, line2) != 0) {
            // Se as linhas são diferentes, registra a discrepância
            char *discrepancy = g_strdup_printf("Descrepância na query: linha %d de \"%s\"", line_number, file1);
            *discrepancies = g_list_append(*discrepancies, discrepancy);
            fclose(f1);
            fclose(f2);
            return 0;
        }

        line_number++;
    }

    fclose(f1);
    fclose(f2);
    return 1;
}

// Função para imprimir os resultados da comparação
void print_discrepancies(GList *discrepancies) {
    GList *node = discrepancies;
    while (node != NULL) {
        printf("%s\n", (char *)node->data);
        node = node->next;
    }
}

// Função para comparar todos os arquivos e reportar as discrepâncias
void compare_all_files(const char *dir1, const char *dir2) {
    struct dirent *entry1;
    DIR *dp1 = opendir(dir1);

    if (!dp1) {
        perror("Erro ao abrir o diretório");
        exit(1);
    }

    GList *discrepancies = NULL;
    int total_files = 0;
    int differing_files = 0;

    // Iterar sobre os arquivos no diretório "resultados"
    while ((entry1 = readdir(dp1)) != NULL) {
        if (entry1->d_type == DT_REG) {  // Verifica se é um arquivo regular
            const char *extension = strrchr(entry1->d_name, '.');
            if (extension && strcmp(extension, ".txt") == 0) {
                total_files++;

                // Construir os caminhos dos arquivos a serem comparados
                char path1[MAX_PATH], path2[MAX_PATH];
                snprintf(path1, sizeof(path1), "%s/%s", dir1, entry1->d_name);
                snprintf(path2, sizeof(path2), "%s/%s", dir2, entry1->d_name); // Mesmos nomes de arquivos em dir1 e dir2

                // Comparar os arquivos
                int result = compare_txt_files(path1, path2, &discrepancies);
                if (result == 0) {
                    differing_files++;
                }
            }
        }
    }

    // Exibe o número total de arquivos e as discrepâncias
    printf("%d de %d testes são diferentes!\n", differing_files, total_files);
    if (discrepancies != NULL) {
        print_discrepancies(discrepancies);
    }

    closedir(dp1);
}

void interpreter_inputs2(FILE *file, GestorSistema *gestorsis)
{
    char *buffer = NULL;
    size_t buffer_size = 0;
    int line_number = 1;

    // Acumuladores para tempo e memória de cada query
    double total_time[6] = {0};
    long total_memory[6] = {0};
    int query_counts[6]={0};
    // Pré-processamento
    calcular_discografia_artistas(gestorsis);
    calcula_streams(gestorsis);

    // Pré-processamento para Query 5
    int numUtilizadores = 0;
    int numGeneros = 0;
    char **idsUtilizadores = preprocessIdsUtilizadores(gestorsis, &numUtilizadores);
    char **nomesGeneros = preprocessNomesGeneros(gestorsis, &numGeneros);
    int **matriz = createMatrizClassificacaoMusicas(numUtilizadores, numGeneros);
    calculaMatrizClassificacaoMusicas(matriz, idsUtilizadores, nomesGeneros, numUtilizadores, numGeneros, gestorsis);

    while (getline(&buffer, &buffer_size, file) != -1)
    {
        g_strstrip(buffer);
        if (strlen(buffer) == 0)
            continue; // Ignorar linhas vazias

        char *token = procura_espaço(buffer);
        clock_t query_start_time;
        long query_initial_memory;

        query_start_time = clock();
        query_initial_memory = get_memory_usage_mb();

        int query_index = -1; // Para determinar qual query foi executada

        if (strcmp(token, "1") == 0)
        {
            g_free(token);
            query_index = 0;
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
            query_index = 0;
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
            query_index = 1;
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
            query_2(gestorsis, num, country, line_number, 0);
            if (country != NULL)
                g_free(country);
        }
        else if (strcmp(token, "2S") == 0)
        {
            g_free(token);
            query_index = 1;
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
            query_2(gestorsis, num, country, line_number, 1);
            if (country != NULL)
                g_free(country);
        }
        // query 3
        else if (strcmp(token, "3") == 0)
        {
            g_free(token);
            query_index = 2;
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
                query_3(min_age, max_age, gestorsis, line_number, 0);
            }
        }
        else if (strcmp(token, "3S") == 0)
        {
            g_free(token);
            query_index = 2;
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
                query_3(min_age, max_age, gestorsis, line_number, 1);
            }
        }
        
                else if (strcmp(token, "4") == 0)
        {
            g_free(token);
            token = procura_espaço2(buffer);
            query_index = 3;
            char *data_inicial = NULL;
            if (token != NULL)
            {
                g_free(data_inicial);
                data_inicial = g_strdup(token);
                g_free(token);
                token = procura_espaço3(buffer);
            }

            char *data_final = NULL;
            if (token != NULL)
            {
                g_free(data_final);
                data_final = g_strdup(token);
                g_free(token);
            }

            querie_4(data_inicial ? data_inicial : NULL, data_final ? data_final : NULL, gestorsis, line_number, 0);

            g_free(data_inicial);
            g_free(data_final);
        }

        else if (strcmp(token, "4S") == 0)
        {
            g_free(token);
            query_index = 3;
            token = procura_espaço2(buffer);

            char *data_inicial = NULL; // Supondo que DATA_INICIAL_DEFAULT seja uma string
            if (token != NULL)
            {
                g_free(data_inicial);
                data_inicial = g_strdup(token);
                g_free(token);
                token = procura_espaço3(buffer);
            }

            char *data_final = NULL; // Supondo que DATA_FINAL_DEFAULT seja uma string
            if (token != NULL)
            {
                g_free(data_final);
                data_final = g_strdup(token);
                g_free(token);
            }

            querie_4(data_inicial ? data_inicial : NULL, data_final ? data_final : NULL, gestorsis, line_number, 1);

            g_free(data_inicial);
            g_free(data_final);
        }
        
        else if (strcmp(token, "5") == 0)
        {
            g_free(token);
            query_index = 4;
            token = procura_espaço2(buffer);
            if (token != NULL)
            {
                char *user_id_str = g_strdup(token);
                g_free(token);
                token = procura_espaço3(buffer);
                if (token != NULL)
                {
                    int numRecomendacoes = atoi(token);
                    g_free(token);
                    query_5(user_id_str, matriz, idsUtilizadores, nomesGeneros, numUtilizadores, numGeneros, numRecomendacoes, line_number, 0, gestorsis);
                    g_free(user_id_str);
                }
            }
        }
        else if (strcmp(token, "5S") == 0)
        {
            g_free(token);
            query_index = 4;
            token = procura_espaço2(buffer);
            if (token != NULL)
            {
                char *user_id_str = g_strdup(token);
                g_free(token);
                token = procura_espaço3(buffer);
                if (token != NULL)
                {
                    int numRecomendacoes = atoi(token);
                    g_free(token);
                    query_5(user_id_str, matriz, idsUtilizadores, nomesGeneros, numUtilizadores, numGeneros, numRecomendacoes, line_number, 1, gestorsis);
                    g_free(user_id_str);
                }
            }
        }
        
        else if (strcmp(token, "6") == 0)
        {
            g_free(token);
            query_index = 5;
            token = procura_espaço2(buffer);
            if (token != NULL)
            {
                char *user_id_str = g_strdup(token);
                int user_id = atoi(user_id_str + 1);
                g_free(token);
                g_free(user_id_str);
                token = procura_espaço3(buffer);
                int year = 0, N = 0;

                if (token != NULL)
                {
                    year = atoi(token);
                    g_free(token);
                    token = procura_espaço4(buffer);

                    if (token != NULL)
                    {
                        N = atoi(token);
                        g_free(token);
                    }
                }
                query_6(user_id, year, N, gestorsis, line_number, 0);
                // g_free(user_id);
            }
        }
        else if (strcmp(token, "6S") == 0)
        {
            g_free(token);
            query_index = 5;
            token = procura_espaço2(buffer);
            if (token != NULL)
            {
                char *user_id_str = g_strdup(token);
                int user_id = atoi(user_id_str + 1);
                g_free(token);
                g_free(user_id_str);
                token = procura_espaço3(buffer);
                int year = 0, N = 0;

                if (token != NULL)
                {
                    year = atoi(token);
                    g_free(token);
                    token = procura_espaço4(buffer);

                    if (token != NULL)
                    {
                        N = atoi(token);
                        g_free(token);
                    }
                }
                query_6(user_id, year, N, gestorsis, line_number, 1);
                // g_free(user_id);
            }
        }

        clock_t query_end_time = clock();
        long query_final_memory = get_memory_usage_mb();

        double query_elapsed_time = ((double)(query_end_time - query_start_time)) / CLOCKS_PER_SEC;
        long query_memory_used = query_final_memory - query_initial_memory;

        if (query_index >= 0 && query_index < 6) {
            total_time[query_index] += query_elapsed_time;
            total_memory[query_index] += query_memory_used;
            query_counts[query_index]++;
        }

        line_number++;
    }

    g_strfreev(idsUtilizadores);
    g_strfreev(nomesGeneros);
    freeMatrizClassificacaoMusicas(matriz, numUtilizadores);
    g_free(buffer);

    // Exibir totais por query
    for (int i = 0; i < 6; i++) {
        printf("Query %d: Tempo: %.2f, Memória: %ld, Execuções: %d\n",
               i + 1, total_time[i], total_memory[i], query_counts[i]);
    }
}

int main(int argc, char *argv[])
{
    GestorSistema *gestor = criar_gestor_sistema(); // Inicializa o gestor
    char *path = g_strdup(argv[1]);

    clock_t total_start_time = clock();
    long total_memory_start = get_memory_usage_mb();

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
        interpreter_inputs2(fileinputs, gestor);
        fclose(fileinputs);
    }
    else
    {
        fprintf(stderr, "Error opening file: %s\n", pathinputs);
    }

    // Calcular o tempo e a memória total
    clock_t total_end_time = clock();
    long total_memory_end = get_memory_usage_mb();

    double total_time_elapsed = ((double)(total_end_time - total_start_time)) / CLOCKS_PER_SEC;
    long total_memory_used = total_memory_end - total_memory_start;

    // Exibir o tempo total e memória total
    printf("\nTempo total: %.2f segundos\n", total_time_elapsed);
    printf("Memória total: %ld MB\n", total_memory_used);
    int query_counts2[] = {30, 10, 5, 10, 10, 10}; // Total de arquivos por query

    // Passa query_counts para compare_all_files
    compare_all_files("resultados", "resultados-esperados");
    
    g_free(pathinputs);
    g_free(path);
    liberar_gestor_sistema(gestor);

    return 0;
}
