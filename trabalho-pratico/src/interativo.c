#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "../include/gestores/gestor_sistemas.h"
#include "../include/queries/queries.h"
#include "../include/queries/queries_aux.h"
#include "../include/parsing/parser.h"
#include "../include/queries/query5_aux.h"
#include "../include/io/io.h"
#include "../include/recomendador/recomendador.h"
#include "../include/validacao/valida.h"

// Protótipos das funções
void mostrar_menu_principal();
void processar_dados(const char *diretorio, GestorSistema *gestor);
void escolher_query(GestorSistema *gestor);
void executar_query(int query_numero, GestorSistema *gestor);

// Função Principal
int main()
{
    int opcao = 0;
    GestorSistema *gestor = criar_gestor_sistema(); // Inicializa o sistema

    do
    {
        mostrar_menu_principal();
        printf("Escolha uma opção: ");
        if (scanf("%d", &opcao) != 1)
        {
            printf("Erro: Entrada inválida. Por favor insira um número.\n");
            while (getchar() != '\n')
                ; // Limpa o buffer de entrada
            continue;
        }

        switch (opcao)
        {
        case 1:
        {
            char *diretorio = readline("Introduza o caminho para o dataset (Tab para completar): ");
            if (!diretorio || diretorio[0] == '\0')
            {
                printf("Nenhum caminho foi inserido.\n");
            }
            else
            {
                processar_dados(diretorio, gestor);
                free(diretorio);
            }
            break;
        }
        case 2:
            escolher_query(gestor);
            break;
        case 3:
            printf("\n[Sobre o Programa]\n");
            printf("Projeto de Sistema Interativo de Streaming de Música\n");
            printf("Desenvolvido por: Seu Nome\n");
            break;
        case 4:
            printf("\nSaindo do programa... Até logo!\n");
            liberar_gestor_sistema(gestor);
            break;
        default:
            printf("Opção inválida. Por favor, tente novamente.\n");
        }
    } while (opcao != 4);

    return 0;
}

// Mostra o menu principal
void mostrar_menu_principal()
{
    printf("\n========================================\n");
    printf("           MODO INTERATIVO              \n");
    printf("========================================\n");
    printf("| 1. Processar Dados                   |\n");
    printf("|--------------------------------------|\n");
    printf("| 2. Escolher Query                    |\n");
    printf("|--------------------------------------|\n");
    printf("| 3. Sobre o Programa                  |\n");
    printf("|--------------------------------------|\n");
    printf("| 4. Sair                              |\n");
    printf("========================================\n");
}

void processar_dados(const char *diretorio, GestorSistema *gestor)
{
    DIR *dir = opendir(diretorio);
    if (!dir)
    {
        perror("Erro ao abrir o diretório");
        return;
    }

    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL)
    {
        if (entry->d_type == DT_REG && strstr(entry->d_name, ".csv"))
        {
            char caminho_completo[512];
            if (snprintf(caminho_completo, sizeof(caminho_completo), "%s/%s", diretorio, entry->d_name) >= sizeof(caminho_completo))
            {
                printf("Erro: Caminho do arquivo excede o tamanho máximo permitido.\n");
                continue;
            }

            FILE *file = fopen(caminho_completo, "r");
            if (!file)
            {
                printf("Erro ao abrir o arquivo: %s\n", caminho_completo);
                continue;
            }

            printf("Processando arquivo: %s\n", entry->d_name);

            // Processar os arquivos específicos, agora inserindo dados no gestor
            if (strcmp(entry->d_name, "artists.csv") == 0)
            {
                process_file(diretorio, "artists.csv", gestor, 'a');
            }
            else if (strcmp(entry->d_name, "musics.csv") == 0)
            {
                process_file(diretorio, "musics.csv", gestor, 'm');
            }
            else if (strcmp(entry->d_name, "users.csv") == 0)
            {
                process_file(diretorio, "users.csv", gestor, 'u');
            }
            else if (strcmp(entry->d_name, "history.csv") == 0)
            {
                process_file(diretorio, "history.csv", gestor, 'h');
            }
            else if (strcmp(entry->d_name, "albums.csv") == 0)
            {
                process_file(diretorio, "albums.csv", gestor, 'l');
            }
            else
            {
                printf("Arquivo não reconhecido: %s\n", entry->d_name);
            }

            fclose(file);
        }
    }

    closedir(dir);
    printf("Todos os arquivos foram processados com sucesso.\n");

    // Após o processamento, chama o menu interativo
    printf("[INFO] Iniciando menu interativo...\n");
    escolher_query(gestor);
}

void escolher_query(GestorSistema *gestor)
{
    int query_numero;

    printf("\nEscolha uma query para executar:\n");
    printf("1 - Listar o resumo de um utilizador ou artista\n");
    printf("2 - Quais são os top N artistas com maior discografia?\n");
    printf("3 - Quais são os géneros de música mais populares numa determinada faixa etária?\n");
    printf("4 - Qual é o artista que esteve no top 10 mais vezes?\n");
    printf("5 - Recomendação de utilizadores com gostos parecidos\n");
    printf("6 - Resumo anual para um utilizador\n");
    printf("> ");

    if (scanf("%d", &query_numero) != 1)
    {
        printf("Entrada inválida. Por favor insira um número.\n");
        while (getchar() != '\n')
            ; // Limpa o buffer
        return;
    }

    // Chama a execução da query escolhida
    executar_query(query_numero, gestor);
}

void executar_query(int query_numero, GestorSistema *gestor)
{

    printf("[INFO] Iniciando pré-processamento necessário...\n");

    calcular_discografia_artistas(gestor);
    calcula_streams(gestor);
    // pré-processeamento da query 5
    int numUtilizadores = 0;
    int numGeneros = 0;
    char **idsUtilizadores = preprocessIdsUtilizadores(gestor, &numUtilizadores);
    char **nomesGeneros = preprocessNomesGeneros(gestor, &numGeneros);
    int **matriz = createMatrizClassificacaoMusicas(numUtilizadores, numGeneros);
    calculaMatrizClassificacaoMusicas(matriz, idsUtilizadores, nomesGeneros, numUtilizadores, numGeneros, gestor);
    ResultadoProcessamento *resultado = processar_semanas_e_contar_artistas(gestor);
    printf("[INFO] Pré-processamento concluído.\n");

    // Switch para executar a query selecionada
    switch (query_numero)
    {
    case 1:
    {
        char id[256]; // Buffer para armazenar o ID do terminal
        printf("Insira o ID do artista para a query 1:\n> ");
        if (scanf("%s", id) == 1)
        {
            query_1(gestor, id, 0, 0, 1); // Passa o ID diretamente para a função
        }
        else
        {
            printf("Erro: ID inválido ou não fornecido.\n");
        }
        break;
    }
    case 2:
    {
        char country[256] = ""; // Inicializa com string vazia
        int num = 0;            // Variável para o número de artistas

        // Solicita o número de artistas
        printf("Insira o nº de artistas:\n> ");
        if (scanf("%d", &num) == 1)
        {
            // Solicita o país (opcional)
            printf("Insira um país (Opcional):\n> ");
            getchar(); // Limpa o buffer após o scanf
            if (fgets(country, sizeof(country), stdin))
            {
                // Remove o newline (\n) caso exista no final
                country[strcspn(country, "\n")] = '\0';

                // Chama a função query_2 com os valores fornecidos
                query_2(gestor, num, country[0] != '\0' ? country : NULL, 0, 0, 1);
            }
            else
            {
                printf("Erro ao ler o país.\n");
            }
        }
        else
        {
            printf("Erro: Número de artistas inválido ou não fornecido.\n");
            while (getchar() != '\n')
                ; // Limpa o buffer de entrada
        }
        break;
    }
    case 3:
    {
        int min_idade = 0;   // Idade mínima padrão
        int max_idade = 100; // Idade máxima padrão

        printf("Insira a idade mínima (ou pressione Enter para usar o padrão 0):\n> ");
        if (scanf("%d", &min_idade) != 1)
        {
            printf("[INFO] Usando idade mínima padrão (0).\n");
            min_idade = 0;
            while (getchar() != '\n')
                ; // Limpa o buffer de entrada
        }

        printf("Insira a idade máxima (ou pressione Enter para usar o padrão 100):\n> ");
        if (scanf("%d", &max_idade) != 1)
        {
            printf("[INFO] Usando idade máxima padrão (100).\n");
            max_idade = 100;
            while (getchar() != '\n')
                ; // Limpa o buffer de entrada
        }

        // Chama a função query_3 com os valores definidos
        query_3(min_idade, max_idade, gestor, 0, 0, 1);
        break;
    }
    case 4:
    {
        int valido = 0;
        char data_inicial[12], data_final[12]; // Buffer maior para leitura segura

        // Limpar buffer de entrada antes de começar
        while (getchar() != '\n' && !feof(stdin))
            ;

        // Solicitar a data inicial
        while (!valido)
        {
            printf("Insira a data inicial (aaaa/mm/dd) ou pressione Enter para todo o histórico: ");
            if (fgets(data_inicial, sizeof(data_inicial), stdin) == NULL)
            {
                printf("Erro ao ler a entrada. Tente novamente.\n");
                continue;
            }

            data_inicial[strcspn(data_inicial, "\n")] = '\0'; // Remover o '\n'

            // Se o usuário pressionou Enter sem inserir uma data
            if (strlen(data_inicial) == 0)
            {
                printf("Nenhuma data inserida. Histórico completo será processado.\n");
                strcpy(data_inicial, "0000/00/00"); // Representa que não há data inicial
                valido = 1;
            }
            else if (validarFormatoData(data_inicial))
            {
                valido = 1;
            }
            else
            {
                printf("Data inválida. Por favor, insira uma data válida no formato aaaa/mm/dd.\n");
            }
        }

        // Solicitar a data final
        valido = 0;
        while (!valido)
        {
            printf("Insira a data final (aaaa/mm/dd) ou pressione Enter para todo o histórico: ");
            if (fgets(data_final, sizeof(data_final), stdin) == NULL)
            {
                printf("Erro ao ler a entrada. Tente novamente.\n");
                continue;
            }

            data_final[strcspn(data_final, "\n")] = '\0'; // Remover o '\n'

            // Se o usuário pressionou Enter sem inserir uma data
            if (strlen(data_final) == 0)
            {
                printf("Nenhuma data inserida. Histórico completo será processado.\n");
                strcpy(data_final, "9999/12/31"); // Representa que não há data final
                valido = 1;
            }
            else if (validarFormatoData(data_final))
            {
                valido = 1;
            }
            else
            {
                printf("Data inválida. Por favor, insira uma data válida no formato aaaa/mm/dd.\n");
            }
        }

        // Chamar a função para processar a query com as datas fornecidas
        querie_4(data_inicial, data_final, gestor, 0, 0, resultado, 1);

        break;
    }

        /*    Case 5: {
         char username[256];
         int numero;

         printf("Insira o nome de utilizador (username):\n> ");
         if (scanf("%s", username) != 1) {
             printf("[ERRO] Username inválido ou não fornecido.\n");
             break;
         }

         printf("Insira um número:\n> ");
         if (scanf("%d", &numero) != 1) {
             printf("[ERRO] Número inválido ou não fornecido.\n");
             break;
         }

         if (matriz != NULL && idsUtilizadores != NULL && nomesGeneros != NULL) {
             printf("[INFO] Executando Query 5 com os parâmetros fornecidos.\n");
             query_5(username, matriz, idsUtilizadores, nomesGeneros, numUtilizadores, numGeneros, numero, 0,0,gestor,1);
         } else {
             printf("[ERRO] Ambiente de pré-processamento para Query 5 não está configurado corretamente.\n");
         }
         break;
     }
     */
    case 6:
    {
        char id_utilizador[10];
        int ano;
        int n = 0; // Valor padrão para N (opcional)
        char escolha;

        // Solicitar o ID do utilizador
        printf("Insira o ID do utilizador: ");
        while (scanf("%s", &id_utilizador) != 1)
        {
            printf("Entrada inválida. Por favor, insira um número válido para o ID do utilizador: ");
            while (getchar() != '\n')
                ; // Limpar buffer
        }

        // Solicitar o ano
        printf("Insira o ano (aaaa): ");
        while (scanf("%d", &ano) != 1 || ano < 0)
        {
            printf("Entrada inválida. Por favor, insira um ano válido (aaaa): ");
            while (getchar() != '\n')
                ; // Limpar buffer
        }

        // Perguntar se deseja inserir o argumento opcional N
        printf("Deseja inserir o valor de N? (s/n): ");
        getchar(); // Consumir o '\n' deixado pelo scanf anterior
        escolha = getchar();

        if (escolha == 's' || escolha == 'S')
        {
            printf("Insira o valor de N: ");
            while (scanf("%d", &n) != 1 || n < 0)
            {
                printf("Entrada inválida. Por favor, insira um número válido para N: ");
                while (getchar() != '\n')
                    ; // Limpar buffer
            }
        }
        else
        {
            printf("N não especificado. Usando valor padrão (0).\n");
        }

        // Chamada da query com os valores fornecidos
        query_6(id_utilizador, ano, n, gestor, 0, 0, 1);

        printf("[INFO] Query 6 processada para o utilizador %s no ano %d com N = %d.\n", id_utilizador, ano, n);
        break;
    }

    default:
        printf("Query inválida. Escolha um número entre 1 e 6.\n");
    }

    /*   // Limpeza de memória após uso
        if (idsUtilizadores) {
            for (int i = 0; i < numUtilizadores; i++) {
                free(idsUtilizadores[i]);
            }
            free(idsUtilizadores);
        }

        if (nomesGeneros) {
            for (int i = 0; i < numGeneros; i++) {
                free(nomesGeneros[i]);
            }
            free(nomesGeneros);
        }

        if (matriz) {
            for (int i = 0; i < numUtilizadores; i++) {
                free(matriz[i]);
            }
            free(matriz);
        }

        */
}