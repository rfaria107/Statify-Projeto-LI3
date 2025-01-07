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
            if (t == 0)
            { // Escreve a linha no arquivo
                write_row(writer, (n == 0) ? ';' : '=', 3, top_artist_id, artist_type, max_count);
            }
            else if (t == 1)
            {
                print_row(writer, (n == 0) ? ';' : '=', 3, top_artist_id, artist_type, max_count);
            }

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
            if (t == 0)
            {
                write_row(writer, (n == 0) ? ';' : '=', 3, top_artist_id, artist_type, max_count);
            }
            if (t == 1)
            {
                print_row(writer, (n == 0) ? ';' : '=', 3, top_artist_id, artist_type, max_count);
            }

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