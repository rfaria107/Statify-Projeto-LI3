#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <dirent.h>
#include <sys/resource.h>
#include <time.h>
#include "../include/entidades/artists.h"
#include "../include/entidades/musica.h"
#include "../include/entidades/usuario.h"
#include "../include/gestores/gestor_artistas.h"
#include "../include/gestores/gestor_musicas.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/parsing/parser.h"
#include "../include/parsing/string_utils.h"
#include "../include/validacao/valida.h"
#include "../include/queries/queries.h"

// Função para medir o uso de memória (em megabytes)
long get_memory_usage_mb() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss / 1024;  // Convertendo para megabytes
}

// Função para comparar dois arquivos .txt
int compare_txt_files(const char *file1, const char *file2) {
    FILE *f1 = fopen(file1, "r");
    FILE *f2 = fopen(file2, "r");

    if (!f1 || !f2) {
        perror("Erro ao abrir arquivos");
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return -1;
    }

    int ch1 = 0, ch2 = 0;
    // Ignorar linhas vazias
    while (1) {
        // Pular linhas vazias no arquivo 1
        do {
            ch1 = fgetc(f1);
        } while (ch1 == '\n' || ch1 == ' ' || ch1 == '\t');  // Pula '\n' ou espaços

        // Pular linhas vazias no arquivo 2
        do {
            ch2 = fgetc(f2);
        } while (ch2 == '\n' || ch2 == ' ' || ch2 == '\t');  // Pula '\n' ou espaços

        // Se ambos arquivos chegaram ao fim, são iguais
        if (ch1 == EOF && ch2 == EOF) {
            fclose(f1);
            fclose(f2);
            return 1;  // Arquivos iguais
        }

        // Se um arquivo terminou mas o outro não, são diferentes
        if (ch1 == EOF || ch2 == EOF) {
            fclose(f1);
            fclose(f2);
            return 0;  // Arquivos diferentes
        }

        // Se os caracteres não são iguais
        if (ch1 != ch2) {
            fclose(f1);
            fclose(f2);
            return 0;  // Arquivos diferentes
        }
    }

    fclose(f1);
    fclose(f2);
    return 1;  // Arquivos iguais
}

// Função para comparar todos os arquivos nas pastas e calcular tempo e memória
void compare_all_files(const char *dir1, const char *dir2) {
    struct dirent *entry;
    DIR *dp1 = opendir(dir1);
    DIR *dp2 = opendir(dir2);

    if (!dp1 || !dp2) {
        perror("Erro ao abrir os diretórios");
        exit(1);
    }

    int total_files = 0;
    int matching_files = 0;
    int missing_files_in_dir2 = 0;
    int total_expected_files = 0;

    GHashTable *expected_files = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    
    // Adicionar todos os arquivos do diretório `resultados-esperados` a uma hash table
    while ((entry = readdir(dp2)) != NULL) {
        if (entry->d_type == DT_REG) {  // Apenas arquivos regulares
            const char *extension = strrchr(entry->d_name, '.');
            if (extension && strcmp(extension, ".txt") == 0) {
                g_hash_table_insert(expected_files, g_strdup(entry->d_name), NULL);
                total_expected_files++;
            }
        }
    }

    // Para cada arquivo no diretório `resultados`, verificamos se ele existe em `resultados-esperados`
    rewinddir(dp1); // Reiniciar a leitura do diretório `resultados`

    while ((entry = readdir(dp1)) != NULL) {
        if (entry->d_type == DT_REG) {  // Apenas arquivos regulares
            const char *extension = strrchr(entry->d_name, '.');
            if (extension && strcmp(extension, ".txt") == 0) {
                total_files++;
                char path1[1024], path2[1024];
                snprintf(path1, sizeof(path1), "%s/%s", dir1, entry->d_name);
                snprintf(path2, sizeof(path2), "%s/%s", dir2, entry->d_name);

                // Verificar se o arquivo existe em `resultados-esperados`
                if (g_hash_table_contains(expected_files, entry->d_name)) {
                    // Compara os arquivos .txt
                    int result = compare_txt_files(path1, path2);
                    if (result == 1) {
                        matching_files++;
                    } else if (result == 0) {
                        printf("Os arquivos %s e %s são diferentes.\n", path1, path2);
                    }
                    g_hash_table_remove(expected_files, entry->d_name);
                }
            }
        }
    }

    // Arquivos faltando em `resultados`
    missing_files_in_dir2 = total_expected_files - matching_files;

    // Fechar os diretórios
    closedir(dp1);
    closedir(dp2);

    // Exibe os resultados
    printf("Arquivos .txt corretos: %d de %d\n", matching_files, total_files);
    printf("Arquivos .txt faltando em 'resultados': %d de %d\n", missing_files_in_dir2, total_expected_files);
    g_hash_table_destroy(expected_files);
}

// Função principal
int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <diretório de entrada> <diretório de resultados>\n", argv[0]);
        return 1;
    }

    // Inicia a medição de tempo e memória
    clock_t start_time = clock();
    long initial_memory = get_memory_usage_mb();

    // Processo principal: parsing e execução
    GestorSistema *gestor = criar_gestor_sistema();
    char *path = g_strdup(argv[1]);

    // Parse dos arquivos de entrada
    char *pathartistas = g_strconcat(path, "/artists.csv", NULL);
    FILE *fileartistas = fopen(pathartistas, "r");
    if (fileartistas) {
        parser_principal(fileartistas, gestor, 'a');
        fclose(fileartistas);
    }
    g_free(pathartistas);

    char *pathmusicas = g_strconcat(path, "/musics.csv", NULL);
    FILE *filemusicas = fopen(pathmusicas, "r");
    if (filemusicas) {
        parser_principal(filemusicas, gestor, 'm');
        fclose(filemusicas);
    }
    g_free(pathmusicas);

    char *pathusers = g_strconcat(path, "/users.csv", NULL);
    FILE *fileusers = fopen(pathusers, "r");
    if (fileusers) {
        parser_principal(fileusers, gestor, 'u');
        fclose(fileusers);
    }
    g_free(pathusers);

    g_free(path);

    char *pathinputs = g_strdup(argv[2]);
    FILE *fileinputs = fopen(pathinputs, "r");
    if (fileinputs) {
        interpreter_inputs(fileinputs, gestor);
        fclose(fileinputs);
    }
    g_free(pathinputs);

    liberar_gestor_sistema(gestor);

    // Medição de tempo e memória após a execução
    clock_t end_time = clock();
    long final_memory = get_memory_usage_mb();

    // Calcula o tempo de execução e a memória utilizada
    double elapsed_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    long memory_used = final_memory - initial_memory;

    // Exibe os resultados de tempo e memória
    printf("Tempo de execução: %.2f segundos\n", elapsed_time);
    printf("Memória utilizada: %ld MB\n", memory_used);

    // Comparação dos arquivos
    compare_all_files("resultados", "resultados-esperados");

    return 0;
}
