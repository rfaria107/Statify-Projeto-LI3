#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "../include/queries/query4_aux.h"
#include "../include/utils/utils.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/io/write/writer.h"

struct ArtistPopularity
{
    guint id;
    char *type;
    int total_vezes_no_top;
};

struct Semana
{
    char domingo[11];     // Data do domingo no formato YYYY/MM/DD
    char sabado[11];      // Data do sábado no formato YYYY/MM/DD
    GHashTable *artistas; // Hash table de artista_id -> duração total
    GList *top_artistas;  // Lista dos IDs dos top 10 artistas
};

struct ResultadoProcessamento
{
    GHashTable *semanas;
    GHashTable *top_10_count;
};

void all_historico(GestorSistema *gestorsis, int line_number, int n, ResultadoProcessamento *resultado, int t)
{

    int size = snprintf(NULL, 0, "resultados/command%d_output.txt", line_number) + 1;
    char *output_file_name = malloc(size);
    snprintf(output_file_name, size, "resultados/command%d_output.txt", line_number);
    RowWriter *writer = initialize_row_writer(output_file_name, 0);

    char *field_names[] = {"Artist_Id", "Type", "Count"};
    char *formatting[] = {"A%07d", "%s", "%d"}; // Formatação das colunas
    row_writer_set_field_names(writer, field_names, 3);
    row_writer_set_formatting(writer, formatting);
    // Certifique-se de que a tabela está disponível
    if (!resultado || !resultado->top_10_count)
    {
        write_row(writer, ';', 1, "");
        return;
    }

    GHashTable *semanas_top_10 = resultado->top_10_count;

    // Arquivo de saída

    int top_artist_id = 0;
    int max_count = 0;

    // Iterador sobre a tabela semanas_top_10
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, semanas_top_10);

    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        int artist_id = GPOINTER_TO_INT(key); // ID do artista como chave
        int count = *(int *)value;            // Contagem como valor

        // Atualiza o artista com mais aparições
        if (count > max_count || (count == max_count && artist_id < top_artist_id))
        {
            max_count = count;
            top_artist_id = artist_id;
        }
    }

    // Verifica se existe um artista mais frequente e registra no arquivo
    if (top_artist_id)
    {
        GestorArtistas *gestorartistas = get_gestor_artistas(gestorsis);
        Artista *artista = buscar_artista(gestorartistas, top_artist_id);
        if (artista)
        {
            char *artist_type = get_artist_type(artista);
            // Escreve a linha no arquivo
             if (t==0){ // Escreve a linha no arquivo
            write_row(writer, (n == 0) ? ';' : '=', 3, top_artist_id, artist_type, max_count);}
            else if (t==1){
            print_row(writer, (n == 0) ? ';' : '=', 3, top_artist_id, artist_type, max_count);}

            free(artist_type);
        }
    }
    else
    {
        char *field_names[] = {"Nada"};
        char *formatting[] = {""};
        row_writer_set_field_names(writer, field_names, 1);
        row_writer_set_formatting(writer, formatting);
        write_row(writer, ';', 1, "");
    }

    // Finaliza o processo
    free_and_finish_writing(writer);
    free(output_file_name);
}

void intervalos_historico(GestorSistema *gestorsis, int line_number, int n, char *data_inicial, char *data_final, ResultadoProcessamento *resultado, int t)
{
    // Inicializa o arquivo de saída
    int size = snprintf(NULL, 0, "resultados/command%d_output.txt", line_number) + 1;
    char *output_file_name = malloc(size);
    snprintf(output_file_name, size, "resultados/command%d_output.txt", line_number);
    RowWriter *writer = initialize_row_writer(output_file_name, 0);

    char *field_names[] = {"Artist_Id", "Type", "Count"};
    char *formatting[] = {"A%07d", "%s", "%d"};
    row_writer_set_field_names(writer, field_names, 3);
    row_writer_set_formatting(writer, formatting);

    // Verifica se o ResultadoProcessamento está disponível
    if (!resultado || !resultado->semanas)
    {
        write_row(writer, ';', 1, "");
        return;
    }

    // Calcula os domingos associados às datas inicial e final
    char *domingo_inicial = calcular_domingo(data_inicial);
    char *domingo_final = calcular_domingo(data_final);
    if (!domingo_inicial || !domingo_final)
    {
        write_row(writer, ';', 1, "");
        free(domingo_inicial);
        free(domingo_final);
        return;
    }

    // Tabela temporária para contagem de aparições no intervalo
    GHashTable *contagem_intervalo = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_free);

    // Itera pelas semanas no intervalo
    GHashTableIter iter;
    gpointer semana_key, semana_value;
    g_hash_table_iter_init(&iter, resultado->semanas);

    while (g_hash_table_iter_next(&iter, &semana_key, &semana_value))
    {
        char *domingo_key = (char *)semana_key;

        // Verifica se a semana está dentro do intervalo
        if (strcmp(domingo_key, domingo_inicial) >= 0 && strcmp(domingo_key, domingo_final) <= 0)
        {
            Semana *semana = (Semana *)semana_value;

            // Obtém os artistas do top 10 dessa semana
            GList *top_artistas = get_top_artistas_na_semana(semana);

            for (GList *node = top_artistas; node != NULL; node = node->next)
            {
                int artist_id = GPOINTER_TO_INT(node->data);

                // Atualiza a contagem do artista na tabela temporária
                int *count_ptr = (int *)g_hash_table_lookup(contagem_intervalo, GINT_TO_POINTER(artist_id));
                if (count_ptr)
                {
                    (*count_ptr)++;
                }
                else
                {
                    int *new_count = malloc(sizeof(int));
                    *new_count = 1;
                    g_hash_table_insert(contagem_intervalo, GINT_TO_POINTER(artist_id), new_count);
                }
            }
        }
    }

    // Determina o artista com mais aparições no intervalo
    int top_artist_id = 0;
    int max_count = 0;

    GHashTableIter count_iter;
    gpointer artist_key, artist_value;
    g_hash_table_iter_init(&count_iter, contagem_intervalo);

    while (g_hash_table_iter_next(&count_iter, &artist_key, &artist_value))
    {
        int artist_id = GPOINTER_TO_INT(artist_key);
        int count = *(int *)artist_value;

        if (count > max_count || (count == max_count && artist_id < top_artist_id))
        {
            max_count = count;
            top_artist_id = artist_id;
        }
    }

    // Registra o artista com mais aparições no arquivo de saída
    if (top_artist_id)
    {
        GestorArtistas *gestorartistas = get_gestor_artistas(gestorsis);
        Artista *artista = buscar_artista(gestorartistas, top_artist_id);
        if (artista)
        {
            char *artist_type = get_artist_type(artista);
            // Escreve a linha no arquivo
            if (t==0){
            write_row(writer, (n == 0) ? ';' : '=', 3, top_artist_id, artist_type, max_count);}
            if (t==1){           
             print_row(writer, (n == 0) ? ';' : '=', 3, top_artist_id, artist_type, max_count);}

            free(artist_type);
        }
    }
    else
    {
        char *field_names[] = {"Nada"};
        char *formatting[] = {""};
        row_writer_set_field_names(writer, field_names, 1);
        row_writer_set_formatting(writer, formatting);
        write_row(writer, ';', 1, "");
    }

    // Libera recursos
    g_hash_table_destroy(contagem_intervalo);
    free_and_finish_writing(writer);
    free(output_file_name);
    free(domingo_inicial);
    free(domingo_final);
}
ResultadoProcessamento *processar_semanas_e_contar_artistas(GestorSistema *gestor_sistema)
{
    // Inicializa a estrutura para os resultados
    ResultadoProcessamento *resultado = malloc(sizeof(ResultadoProcessamento));
    resultado->top_10_count = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_free);
    resultado->semanas = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, (GDestroyNotify)destruir_semana);

    // Obtém os históricos
    GestorHistories *gestorhistories = get_gestor_histories(gestor_sistema);
    GHashTable *historicos = get_hash_histories(gestorhistories);

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, historicos);

    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        History *history = (History *)value;

        // Obtém a data do histórico
        char *data = get_history_date(history);

        // Calcula o "domingo" correspondente à data
        char *domingo = calcular_domingo(data);
        if (domingo == NULL)
        {
            free(data);
            continue; // Se não for possível calcular o domingo, pula para o próximo histórico
        }
        free(data);
        // Duplica o valor de domingo para uso como chave na tabela hash
        char *domingo_key = g_strdup(domingo);
        free(domingo); // Libera a memória do valor original de domingo

        // Obtenção ou criação da semana correspondente
        Semana *semana = g_hash_table_lookup(resultado->semanas, domingo_key);
        if (!semana)
        {
            semana = criar_semana(domingo_key);
            g_hash_table_insert(resultado->semanas, domingo_key, semana);
        }
        else
        {
            free(domingo_key); // Libera a cópia da chave, pois já existe na tabela hash
        }

        // Processamento da música
        int musica_id = get_history_music_id(history);
        GestorMusicas *gestormusicas = get_gestor_musicas(gestor_sistema);
        Musica *musica = buscar_musicas(gestormusicas, musica_id);
        if (musica)
        {
            gchar **artist_ids = get_music_artist_ids(musica);
            gchar *duration = get_history_duration(history);
            int duracao = duracao_para_segundos(duration);
            free(duration);

            // Adiciona os artistas na semana correspondente
            for (int i = 0; artist_ids[i] != NULL; i++)
            {
                int artist_id_int = atoi(artist_ids[i] + 1);
                adicionar_artista_na_semana(semana, artist_id_int, duracao);
            }
            g_strfreev(artist_ids);
        }
    }

    // Processa cada semana e calcula a contagem dos artistas no top 10
    GHashTableIter semanas_iter;
    gpointer semana_key, semana_value;
    g_hash_table_iter_init(&semanas_iter, resultado->semanas);

    while (g_hash_table_iter_next(&semanas_iter, &semana_key, &semana_value))
    {
        Semana *semana = (Semana *)semana_value;

        // Atualiza o top artistas para a semana
        atualizar_top_artistas_na_semana(semana);

        // Obtém a lista de top artistas da semana
        GList *top_artistas = get_top_artistas_na_semana(semana);
        if (!top_artistas)
        {
            continue; // Se não houver artistas na semana, pula para o próximo
        }

        // Conta as aparições dos artistas no top 10
        for (GList *node = top_artistas; node != NULL; node = node->next)
        {
            int artist_id = GPOINTER_TO_INT(node->data);

            // Atualiza a contagem de aparições do artista
            int *count = (int *)g_hash_table_lookup(resultado->top_10_count, GINT_TO_POINTER(artist_id));
            if (!count)
            {
                count = malloc(sizeof(int));
                *count = 1;
                g_hash_table_insert(resultado->top_10_count, GINT_TO_POINTER(artist_id), count);
            }
            else
            {
                (*count)++;
            }
        }
    }

    // Retorna a estrutura contendo as duas hash tables
    return resultado;
}

// Função para calcular os domingos relativos à data inicial e final
void calcular_domingos_inicial_e_final(char *data_inicial, char *data_final, char **domingo_inicial, char **domingo_final)
{
    *domingo_inicial = calcular_domingo(data_inicial);
    *domingo_final = calcular_domingo(data_final);
}

int parse_data(const char *data_str, struct tm *tm)
{
    if (data_str == NULL || tm == NULL)
    {
        return -1; // Erro se os parâmetros forem inválidos
    }

    // A função sscanf divide a string em componentes de ano, mês e dia
    int year, month, day;
    if (sscanf(data_str, "%4d/%2d/%2d", &year, &month, &day) != 3)
    {
        return -1; // Se o formato não for correto, retorna erro
    }

    // Preenche a estrutura tm com os valores de ano, mês e dia
    tm->tm_year = year - 1900; // O tm_year é contado a partir de 1900
    tm->tm_mon = month - 1;    // O tm_mon é contado a partir de 0 (janeiro)
    tm->tm_mday = day;         // O tm_mday já é o valor correto para o dia

    // Inicializa as outras variáveis da estrutura tm com valores padrão
    tm->tm_hour = 0; // Define a hora como 00:00
    tm->tm_min = 0;
    tm->tm_sec = 0;
    tm->tm_isdst = -1; // O mktime irá determinar automaticamente se é horário de verão

    return 0; // Sucesso
}

int calcular_domingo_anterior(const char *data_str, struct tm *domingo_tm)
{
    if (!data_str || !domingo_tm)
    {
        printf("Erro: parâmetros inválidos em calcular_domingo_anterior.\n");
        return -1; // Verifica se os parâmetros são válidos
    }

    // Variáveis para ano, mês e dia
    int year, month, day;

    // Converte a string de data manualmente
    if (data_str[4] != '/' || data_str[7] != '/')
    {
        printf("Erro: formato de data inválido. Esperado 'yyyy/mm/dd'.\n");
        return -1; // Verifica se a string tem o formato esperado "yyyy/mm/dd"
    }

    // Extrai o ano, mês e dia
    int parsed = sscanf(data_str, "%4d/%2d/%2d", &year, &month, &day);
    if (parsed != 3)
    {
        printf("Erro: falha ao fazer o parsing da data. Resultados de sscanf: %d\n", parsed);
        return -1; // Verifica se o parsing foi bem-sucedido
    }

    // Exibe os valores de ano, mês e dia extraídos
    printf("Data extraída: Ano = %d, Mês = %d, Dia = %d\n", year, month, day);

    // Verifica se a conversão foi bem-sucedida
    if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31)
    {
        printf("Erro: valores inválidos de data. Ano: %d, Mês: %d, Dia: %d\n", year, month, day);
        return -1;
    }

    // Preenche a estrutura tm com os valores de ano, mês e dia
    domingo_tm->tm_year = year - 1900; // O tm_year é contado a partir de 1900
    domingo_tm->tm_mon = month - 1;    // O tm_mon é contado a partir de 0 (janeiro)
    domingo_tm->tm_mday = day;         // O tm_mday já é o valor correto para o dia

    // Inicializa as outras variáveis da estrutura tm com valores padrão
    domingo_tm->tm_hour = 0; // Define a hora como 00:00
    domingo_tm->tm_min = 0;
    domingo_tm->tm_sec = 0;
    domingo_tm->tm_isdst = -1; // O mktime irá determinar automaticamente se é horário de verão

    // Mostra a estrutura tm antes do mktime
    printf("Antes de mktime: tm_year = %d, tm_mon = %d, tm_mday = %d\n",
           domingo_tm->tm_year, domingo_tm->tm_mon, domingo_tm->tm_mday);

    // Calcula a data de domingo (ajuste para o horário local e valida)
    int res = mktime(domingo_tm);
    if (res == -1)
    {
        printf("Erro: falha ao calcular mktime.\n");
        return -1; // Verifica se houve erro no cálculo do mktime
    }

    // Mostra a estrutura tm após o mktime
    printf("Após mktime: tm_year = %d, tm_mon = %d, tm_mday = %d, tm_wday = %d\n",
           domingo_tm->tm_year, domingo_tm->tm_mon, domingo_tm->tm_mday, domingo_tm->tm_wday);

    // Calcula os dias a serem subtraídos para chegar no domingo anterior
    int days_to_sunday = domingo_tm->tm_wday; // tm_wday é o dia da semana (0 = domingo)
    if (days_to_sunday == 0)
    {
        days_to_sunday = 7; // Se for domingo, não subtraímos nenhum dia
    }

    // Mostra a quantidade de dias a ser subtraída
    printf("Número de dias para subtrair: %d\n", days_to_sunday);

    domingo_tm->tm_mday -= days_to_sunday; // Ajusta para o domingo anterior

    // Recalcula a data após o ajuste
    res = mktime(domingo_tm);
    if (res == -1)
    {
        printf("Erro: falha ao recalcular mktime após ajuste.\n");
        return -1; // Verifica se houve erro no recalculo do mktime
    }

    // Mostra a estrutura tm após o cálculo final
    printf("Após ajuste de domingo: tm_year = %d, tm_mon = %d, tm_mday = %d\n",
           domingo_tm->tm_year, domingo_tm->tm_mon, domingo_tm->tm_mday);

    return 0; // Sucesso
}

time_t get_timestamp(const char *data)
{
    struct tm tm_data = {0};
    if (sscanf(data, "%d/%d/%d", &tm_data.tm_year, &tm_data.tm_mon, &tm_data.tm_mday) != 3)
    {
        fprintf(stderr, "Erro: Formato de data inválido.\n");
        return -1;
    }
    tm_data.tm_year -= 1900;
    tm_data.tm_mon -= 1;
    return mktime(&tm_data);
}

// Calcula o domingo da semana de uma data
gchar *calcular_domingo(const gchar *data)
{
    int ano, mes, dia;
    if (sscanf(data, "%d/%d/%d", &ano, &mes, &dia) != 3)
    {
        fprintf(stderr, "Erro ao fazer o parse da data.\n");
        return NULL;
    }

    struct tm tm_data = {0};
    tm_data.tm_year = ano - 1900;
    tm_data.tm_mon = mes - 1;
    tm_data.tm_mday = dia;
    mktime(&tm_data);

    int dias_para_domingo = tm_data.tm_wday;
    tm_data.tm_mday -= dias_para_domingo;
    mktime(&tm_data);

    gchar *domingo = malloc(11 * sizeof(gchar));
    if (domingo == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para a data de domingo.\n");
        return NULL;
    }

    strftime(domingo, 11, "%Y/%m/%d", &tm_data);
    return domingo;
}

// Calcula o sábado seguinte de uma data
gchar *calcular_sabado_seguinte(const gchar *data)
{
    int ano, mes, dia;
    if (sscanf(data, "%d/%d/%d", &ano, &mes, &dia) != 3)
    {
        fprintf(stderr, "Erro ao fazer o parse da data.\n");
        return NULL;
    }

    struct tm tm_data = {0};
    tm_data.tm_year = ano - 1900;
    tm_data.tm_mon = mes - 1;
    tm_data.tm_mday = dia;
    mktime(&tm_data);

    int dias_para_sabado = (6 - tm_data.tm_wday + 7) % 7;
    tm_data.tm_mday += dias_para_sabado;
    mktime(&tm_data);

    gchar *sabado = malloc(11 * sizeof(gchar));
    if (sabado == NULL)
    {
        fprintf(stderr, "Erro ao alocar memória para a data de sábado.\n");
        return NULL;
    }

    strftime(sabado, 11, "%Y/%m/%d", &tm_data);
    return sabado;
}

// Cria uma semana com domingo e sábado
Semana *criar_semana(const char *domingo)
{
    Semana *semana = g_malloc(sizeof(Semana));

    g_strlcpy(semana->domingo, domingo, sizeof(semana->domingo));

    struct tm tm_domingo = {0};
    if (sscanf(domingo, "%d/%d/%d", &tm_domingo.tm_year, &tm_domingo.tm_mon, &tm_domingo.tm_mday) != 3)
    {
        fprintf(stderr, "Erro ao analisar a data.\n");
        g_free(semana);
        return NULL;
    }
    tm_domingo.tm_year -= 1900;
    tm_domingo.tm_mon -= 1;

    tm_domingo.tm_mday += 6;
    mktime(&tm_domingo);

    strftime(semana->sabado, sizeof(semana->sabado), "%Y/%m/%d", &tm_domingo);

    semana->artistas = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, free);
    semana->top_artistas = NULL;

    return semana;
}

GHashTable *get_semanas_artistas(Semana *semana)
{
    if (!semana)
    {
        return NULL;
    }
    return semana->artistas;
}

void destruir_semana(Semana *semana)
{
    if (!semana)
    {
        return;
    }
    if (semana->top_artistas)
    {
        g_list_free(semana->top_artistas);
    }
    if (semana->artistas)
    {
        g_hash_table_destroy(semana->artistas);
    }
    free(semana);
}

gint contar_aparicoes_artista(Semana *semana, int artist_id)
{
    if (!semana || !artist_id)
    {
        fprintf(stderr, "Erro: Semana ou artist_id inválido.\n");
        return 0;
    }

    gint count = 0;
    for (GList *node = semana->top_artistas; node != NULL; node = node->next)
    {
        int artist = *(int *)node->data;
        if (artist && artist == artist_id)
        {
            count++;
        }
    }
    return count;
}

void destruir_resultado_processamento(ResultadoProcessamento *resultado)
{
    if (resultado == NULL)
    {
        return; // Se o ponteiro for NULL, nada a fazer
    }

    // Libera a memória da tabela `top_10_count`
    if (resultado->top_10_count != NULL)
    {
        g_hash_table_destroy(resultado->top_10_count);
    }

    if (resultado->semanas != NULL)
    {
        g_hash_table_destroy(resultado->semanas);
    }

    // Finalmente, liberta o próprio struct
    g_free(resultado);
}

void atualizar_top_artistas_na_semana(Semana *semana)
{
    if (!semana || !semana->artistas)
    {
        return;
    }

    GList *artistas_populares = NULL;
    GHashTableIter iter;
    gpointer key, value;

    // Cria lista de popularidade a partir da hash table
    g_hash_table_iter_init(&iter, semana->artistas);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        int artist_id = GPOINTER_TO_INT(key);
        int duracao_total_segundos = *(int *)value;

        // Cria estrutura de popularidade
        ArtistPopularity *popularity = malloc(sizeof(ArtistPopularity));
        popularity->id = artist_id;
        popularity->type = g_strdup("Desconhecido");
        popularity->total_vezes_no_top = duracao_total_segundos;

        // Adiciona à lista
        artistas_populares = g_list_prepend(artistas_populares, popularity);
    }

    // Ordena lista de popularidade
    artistas_populares = g_list_sort(artistas_populares, (GCompareFunc)comparar_popularidade);

    // Atualiza os top artistas da semana
    GList *top_artistas = NULL;
    int count = 0;
    for (GList *node = artistas_populares; node != NULL && count < 15; node = node->next, count++)
    {
        ArtistPopularity *popularity = (ArtistPopularity *)node->data;
        if (count < 10)
        {
            top_artistas = g_list_append(top_artistas, GINT_TO_POINTER(popularity->id));
        }
    }

    // Substitui a lista de top_artistas na semana
    if (semana->top_artistas)
    {
        g_list_free_full(semana->top_artistas, g_free);
    }
    semana->top_artistas = top_artistas;

    // Limpa lista de popularidade
    liberar_popularidade(artistas_populares);
}

// Compara popularidade dos artistas
gint comparar_popularidade(struct ArtistPopularity *a, struct ArtistPopularity *b)
{
    gint diff = b->total_vezes_no_top - a->total_vezes_no_top;

    // Se as durações forem iguais, compara pelo ID
    if (diff == 0)
    {
        return (a->id - b->id);
    }

    return diff;
}

// Libera a memória da lista de popularidade
void liberar_popularidade(GList *popularity_list)
{
    g_list_free_full(popularity_list, (GDestroyNotify)free_popularity);
}

// Libera memória associada a um objeto de popularidade
void free_popularity(ArtistPopularity *popularity)
{
    if (popularity)
    {
        free(popularity->type);
        free(popularity);
    }
}

// Adiciona um artista na semana, somando a duração
void adicionar_artista_na_semana(Semana *semana, int artist_id, int duracao)
{
    if (!semana || !artist_id)
    {
        fprintf(stderr, "Erro: Semana ou Artist ID é NULL.\n");
        return;
    }

    // Verifica se o artista já existe na hash table
    gpointer valor = g_hash_table_lookup(semana->artistas, GINT_TO_POINTER(artist_id));

    int duracao_existente = valor ? *(int *)valor : 0; // Se existir, obtém a duração; caso contrário, 0

    // Nova duração acumulada
    int nova_duracao = duracao_existente + duracao;

    if (valor)
    {
        // Atualiza o valor existente
        *(int *)valor = nova_duracao;
    }
    else
    {
        // Insere um novo valor
        int *nova_duracao_ptr = malloc(sizeof(int));
        *nova_duracao_ptr = nova_duracao;
        g_hash_table_insert(semana->artistas, GINT_TO_POINTER(artist_id), nova_duracao_ptr);
    }
}

// Retorna os top artistas da semana
GList *get_top_artistas_na_semana(Semana *semana)
{
    if (!semana)
    {
        fprintf(stderr, "Erro: Semana é NULL.\n");
        return NULL;
    }
    return semana->top_artistas;
}

// Retorna a duração de um artista na semana
gint get_duracao_artista_na_semana(Semana *semana, int artist_id)
{
    if (!semana || !semana->artistas || !artist_id)
    {
        return 0;
    }

    gint *duracao = g_hash_table_lookup(semana->artistas, GINT_TO_POINTER(artist_id));
    return duracao ? *duracao : 0;
}
// Função para comparar duas datas no formato "yyyy/mm/dd"
gint comparar_datas(const char *data1, const char *data2)
{
    time_t timestamp1 = get_timestamp(data1);
    time_t timestamp2 = get_timestamp(data2);

    if (timestamp1 == (time_t)-1 || timestamp2 == (time_t)-1)
    {
        return 0; // Retorna 0 em caso de erro na conversão
    }

    if (timestamp1 < timestamp2)
    {
        return -1; // data1 é anterior a data2
    }
    else if (timestamp1 > timestamp2)
    {
        return 1; // data1 é posterior a data2
    }

    return 0; // datas são iguais
}

/*
void processar_historico(char *data_incial, char *data_final, GestorSistema *gestorsis, int line_number, int n)
{

    int size = snprintf(NULL, 0, "resultados/command%d_output.txt", line_number) + 1;
    char *output_file_name = malloc(size);
    snprintf(output_file_name, size, "resultados/command%d_output.txt", line_number);
    RowWriter *writer = initialize_row_writer(output_file_name, 0);

    // Inicializa a tabela de semanas
    GHashTable *semanas = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)destruir_semana);

    // Obtém os históricos
    GestorHistories *gestorhistories = get_gestor_histories(gestorsis);
    GHashTable *historicos = get_hash_histories(gestorhistories);

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, historicos);

    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        History *history = (History *)value;

        char *data = get_history_date(history);

        char *domingo = calcular_domingo(data);
        if (domingo == NULL)
        {
        }
        else
        {

            // Duplica o valor de domingo para uso como chave na tabela hash
            char *domingo_key = g_strdup(domingo);

            // Obtenção ou criação da semana correspondente
            Semana *semana = g_hash_table_lookup(semanas, domingo_key);
            if (!semana)
            {
                semana = criar_semana(domingo);
                g_hash_table_insert(semanas, domingo_key, semana);
            }
            else
            {
                free(domingo_key); // Libera a cópia, pois já existe na tabela hash
            }

            free(domingo); // Libera o valor original de domingo

            // Processamento da música
            int musica_id = get_history_music_id(history);
            GestorMusicas *gestormusicas = get_gestor_musicas(gestorsis);

            Musica *musica = buscar_musicas(gestormusicas, musica_id);
            if (musica)
            {
                gchar **artist_ids = get_music_artist_ids(musica);
                gchar *duration = get_history_duration(history);
                int duracao = duracao_para_segundos(duration);
                free(duration);

                // Adiciona os artistas na semana correspondente
                for (int i = 0; artist_ids[i] != NULL; i++)
                {
                    adicionar_artista_na_semana(semana, artist_ids[i], duracao);
                }
                g_strfreev(artist_ids);
            }
        }
    }

    // Agora, calcula o top 10 de artistas
    GHashTable *top_10_count = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
    GHashTableIter semanas_iter;
    gpointer semana_key, semana_value;
    g_hash_table_iter_init(&semanas_iter, semanas);

    // Processa cada semana e calcula a contagem dos artistas
    while (g_hash_table_iter_next(&semanas_iter, &semana_key, &semana_value))
    {
        Semana *semana = (Semana *)semana_value;

        atualizar_top_artistas_na_semana(semana);

        GList *top_artistas = get_top_artistas_na_semana(semana);
        if (!top_artistas)
        {
            continue;
        }

        // Conta as aparições dos artistas no top 10
        for (GList *node = top_artistas; node != NULL; node = node->next)
        {
            char *artist_id = (char *)node->data;

            int *count = (int *)g_hash_table_lookup(top_10_count, artist_id);
            if (!count)
            {
                count = malloc(sizeof(int));
                *count = 1;
                g_hash_table_insert(top_10_count, g_strdup(artist_id), count);
            }
            else
            {
                (*count)++;
            }
        }
    }
    // Procura o artista com mais aparições
    char *top_artist_id = NULL;
    int max_count = 0;

    GHashTableIter count_iter;
    gpointer count_key, count_value;
    g_hash_table_iter_init(&count_iter, top_10_count);

    while (g_hash_table_iter_next(&count_iter, &count_key, &count_value))
    {
        char *artist_id = (char *)count_key;
        int count = *(int *)count_value;

        if (count > max_count)
        {
            max_count = count;
            if (top_artist_id)
            {
                free(top_artist_id);
            }
            top_artist_id = g_strdup(artist_id);
        }
    }

    // Registra o artista mais aparições
    if (top_artist_id)
    {
        int top_artist_int = atoi(top_artist_id + 1);
        GestorArtistas *gestorartistas = get_gestor_artistas(gestorsis);
        Artista *artista = buscar_artista(gestorartistas, top_artist_int);
        if (artista)
        {
            char *artist_type = get_artist_type(artista);
            char *field_names[] = {"Artist_Id", "Type", "Count"};
            char *formatting[] = {"%s", "%s", "%d"}; // Aqui o %d para o count que é um inteiro
            row_writer_set_field_names(writer, field_names, 3);
            row_writer_set_formatting(writer, formatting);

            // Escreve a linha no arquivo
            write_row(writer, (n == 0) ? ';' : '=', 3, top_artist_id, artist_type, max_count);

            free(artist_type);
        }
        free(top_artist_id);
    }

    // Finaliza o processo
    free_and_finish_writing(writer);
    free(output_file_name);
    g_hash_table_destroy(semanas);
    g_hash_table_destroy(top_10_count);
}

void processar_historico_intervalo_de_datas(char *data_inicial, char *data_final, GestorSistema *gestor_sistema, int line_number, int n)
{

    // Arquivo de saída
    int size = snprintf(NULL, 0, "resultados/command%d_output.txt", line_number) + 1;
    char *output_file_name = malloc(size);
    snprintf(output_file_name, size, "resultados/command%d_output.txt", line_number);
    RowWriter *writer = initialize_row_writer(output_file_name, 0);

    // HashTable para armazenar semanas criadas
    GHashTable *semanas = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)destruir_semana);

    GestorHistories *gestorhistories = get_gestor_histories(gestor_sistema);
    GHashTable *historicos = get_hash_histories(gestorhistories);

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, historicos);

    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        History *history = (History *)value;

        char *data = get_history_date(history);

        // Cálculo dos limites de data inicial e final
        char *domingo_inicial = calcular_domingo(data_inicial);
        char *sabado_final = calcular_sabado_seguinte(data_final);

        if (domingo_inicial == NULL || sabado_final == NULL)
        {
            free(domingo_inicial);
            free(sabado_final);
            return; // Encerra o processamento devido ao erro
        }

        // Comparação com data inicial
        if (data_inicial && comparar_datas(data, domingo_inicial) < 0)
        {
            free(domingo_inicial);
            free(sabado_final);
            continue;
        }

        // Comparação com data final
        if (data_final && comparar_datas(data, sabado_final) > 0)
        {
            free(domingo_inicial);
            free(sabado_final);
            continue;
        }

        // Liberação de memória após uso
        free(domingo_inicial);
        free(sabado_final);

        // Cálculo do domingo correspondente à data
        char *domingo = calcular_domingo(data);
        if (domingo == NULL)
        {
            fprintf(stderr, "Erro ao calcular o domingo correspondente para a data %s.\n", data);
        }
        else
        {

            // Duplica o valor de domingo para uso como chave na tabela hash
            char *domingo_key = g_strdup(domingo);

            // Obtenção ou criação da semana correspondente
            Semana *semana = g_hash_table_lookup(semanas, domingo_key);
            if (!semana)
            {
                semana = criar_semana(domingo);
                g_hash_table_insert(semanas, domingo_key, semana);
            }
            else
            {
                free(domingo_key); // Libera a cópia, pois já existe na tabela hash
            }

            free(domingo); // Libera o valor original de domingo

            // Processamento da música
            int musica_id = get_history_music_id(history);
            GestorMusicas *gestormusicas = get_gestor_musicas(gestor_sistema);

            Musica *musica = buscar_musicas(gestormusicas, musica_id);
            if (musica)
            {
                gchar **artist_ids = get_music_artist_ids(musica);
                gchar *duration = get_history_duration(history);
                int duracao = duracao_para_segundos(duration);
                free(duration);

                // Adiciona os artistas na semana correspondente
                for (int i = 0; artist_ids[i] != NULL; i++)
                {
                    adicionar_artista_na_semana(semana, artist_ids[i], duracao);
                }
                g_strfreev(artist_ids);
            }
        }
    }

    GHashTableIter semanas_iter;
    gpointer semana_key, semana_value;
    g_hash_table_iter_init(&semanas_iter, semanas);

    GHashTable *top_10_count = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
    g_hash_table_iter_init(&semanas_iter, semanas);

    while (g_hash_table_iter_next(&semanas_iter, &semana_key, &semana_value))
    {
        Semana *semana = (Semana *)semana_value;

        atualizar_top_artistas_na_semana(semana);

        GList *top_artistas = get_top_artistas_na_semana(semana);
        if (!top_artistas)
        {
            continue;
        }

        for (GList *node = top_artistas; node != NULL; node = node->next)
        {
            char *artist_id = (char *)node->data;

            int *count = (int *)g_hash_table_lookup(top_10_count, artist_id);
            if (!count)
            {
                count = malloc(sizeof(int));
                *count = 1;
                g_hash_table_insert(top_10_count, g_strdup(artist_id), count);
            }
            else
            {
                (*count)++;
            }
        }
    }

    // Encontrar o artista com mais aparições
    char *top_artist_id = NULL;
    int max_count = 0;

    GHashTableIter count_iter;
    gpointer count_key, count_value;
    g_hash_table_iter_init(&count_iter, top_10_count);

    while (g_hash_table_iter_next(&count_iter, &count_key, &count_value))
    {
        char *artist_id = (char *)count_key;
        int count = *(int *)count_value;

        if (count > max_count)
        {
            max_count = count;
            if (top_artist_id)
            {
                free(top_artist_id);
            }
            top_artist_id = g_strdup(artist_id);
        }
    }

    // Registra o artista mais aparições
    if (top_artist_id)
    {
        int top_artist_int = atoi(top_artist_id + 1);
        GestorArtistas *gestorartistas = get_gestor_artistas(gestor_sistema);
        Artista *artista = buscar_artista(gestorartistas, top_artist_int);
        if (artista)
        {
            char *artist_type = get_artist_type(artista);
            char *field_names[] = {"Artist_Id", "Type", "Count"};
            char *formatting[] = {"%s", "%s", "%d"}; // Aqui o %d para o count que é um inteiro
            row_writer_set_field_names(writer, field_names, 3);
            row_writer_set_formatting(writer, formatting);

            // Escreve a linha no arquivo
            write_row(writer, (n == 0) ? ';' : '=', 3, top_artist_id, artist_type, max_count);

            free(artist_type);
        }
        free(top_artist_id);
    }

    // Finaliza o processo
    free_and_finish_writing(writer);
    free(output_file_name);
    g_hash_table_destroy(semanas);
    g_hash_table_destroy(top_10_count);
}

*/