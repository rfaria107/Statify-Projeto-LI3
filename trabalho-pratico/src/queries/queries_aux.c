#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <math.h>
#include "../include/queries/queries.h"
#include "../include/entidades/artists.h"
#include "../include/entidades/musica.h"
#include "../include/entidades/usuario.h"
#include "../include/entidades/albuns.h"
#include "../include/entidades/history.h"
#include "../include/gestores/gestor_artistas.h"
#include "../include/gestores/gestor_musicas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/gestores/gestor_histories.h"
#include "../include/gestores/gestor_albuns.h"
#include "../include/write/writer.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/utils/string_utils.h"
#include "../include/queries/queries_aux.h"

struct UserData
{
    GHashTable *hash;
    gboolean reverse;
    gboolean alphabetical;
};

struct GenrePopularity
{
    char *genre;
    int total_likes;
};

 struct ArtistPopularity {
    char *id;
    char *type;
    int total_vezes_no_top;
};


typedef struct Semana {
    char domingo[11];      // Data do domingo no formato YYYY/MM/DD
    char sabado[11];       // Data do sábado no formato YYYY/MM/DD
    GHashTable *artistas;  // Hash table de artista_id -> duração total
    GList *top_artistas;   // Lista dos IDs dos top 10 artistas
} Semana;


gint duracao_para_segundos(const gchar *duracao)
{
    gint horas, minutos, segundos;
    sscanf(duracao, "%d:%d:%d", &horas, &minutos, &segundos);
    return horas * 3600 + minutos * 60 + segundos;
}

// Função para converter segundos em "HH:MM:SS"
gchar *segundos_para_duracao(gint total_segundos)
{
    gint horas = total_segundos / 3600;
    gint minutos = (total_segundos % 3600) / 60;
    gint segundos = total_segundos % 60;

    return g_strdup_printf("%02d:%02d:%02d", horas, minutos, segundos);
}

void calcular_discografia_artistas(GestorSistema *gestorsis)
{
    GestorArtistas *gestorartistas = get_gestor_artistas(gestorsis);
    GestorMusicas *gestormusicas = get_gestor_musicas(gestorsis);

    GHashTable *hashartistas = get_hash_artistas(gestorartistas);
    GHashTable *hashmusicas = get_hash_musicas(gestormusicas);

    if (!hashmusicas || !hashartistas)
    {
        return;
    }

    GHashTableIter iter_musicas;
    gpointer key_musica, value_musica;
    g_hash_table_iter_init(&iter_musicas, hashmusicas);

    while (g_hash_table_iter_next(&iter_musicas, &key_musica, &value_musica))
    {
        Musica *musica = (Musica *)value_musica;
        if (!musica)
            continue;

        gchar *duracao_musica = get_music_duration(musica);

        if (!duracao_musica)
        {
            continue;
        }

        int duracao_musica_seg = duracao_para_segundos(duracao_musica);
        g_free(duracao_musica);

        gchar **artist_ids = get_music_artist_ids(musica);
        if (!artist_ids)
        {
            continue; // Skip if no artist IDs
        }

        for (int i = 0; artist_ids[i] != NULL; i++)
        {
            gchar *artist_id = g_strdup(artist_ids[i]);
            int artist_id_int = atoi(artist_id + 1);
            Artista *artista = buscar_artista(gestorartistas, artist_id_int);
            g_free(artist_id);

            if (artista != NULL)
            {
                int duracao_atual = get_artist_duracao_disco(artista);
                if (duracao_atual < 0)
                {
                    continue;
                }
                int nova_duracao = duracao_atual + duracao_musica_seg;
                set_artista_duracao_disco(artista, nova_duracao);
            }
        }
        g_strfreev(artist_ids);
    }
}

gint compara_duracoes_discografia(gconstpointer a, gconstpointer b)
{
    Artista *artista_a = (Artista *)a;
    Artista *artista_b = (Artista *)b;

    gint duracao_a = get_artist_duracao_disco(artista_a);
    gint duracao_b = get_artist_duracao_disco(artista_b);

    // compara por duracao
    if (duracao_a < duracao_b)
    {
        return 1;
    }
    else if (duracao_a > duracao_b)
    {
        return -1;
    }

    // se a duracao for igual comparar ordem alfabetica do nome
    gchar *nome_a = get_artist_name(artista_a);
    gchar *nome_b = get_artist_name(artista_b);
    int r = g_strcmp0(nome_a, nome_b);
    g_free(nome_a);
    g_free(nome_b);
    return r;
}
// Função para criar uma nova instância de GenrePopularity
GenrePopularity *create_genre_popularity(const char *genre)
{
    GenrePopularity *gp = malloc(sizeof(GenrePopularity));
    gp->genre = g_strdup(genre); // Copia a string para evitar problemas de ponteiros
    gp->total_likes = 0;
    return gp;
}

// Função para liberar a memória de GenrePopularity
void free_genre_popularity(GenrePopularity *gp)
{
    g_free(gp->genre);
    free(gp);
}

// Função de comparação para ordenar primeiro pela popularidade e depois alfabeticamente (em caso de empate)
int compare_genre_popularity(const void *a, const void *b)
{
    // Converter
    GenrePopularity *gp_a = (GenrePopularity *)a;
    GenrePopularity *gp_b = (GenrePopularity *)b;

    if (gp_b->total_likes != gp_a->total_likes)
    {
        return gp_b->total_likes - gp_a->total_likes; // Ordena por likes decrescentemente
    }
    return g_strcmp0(gp_a->genre, gp_b->genre); // Ordena alfabeticamente em caso de empate (usando uma funcao pre-definida do Glib)
}

// Funcao para calcular o num_albums_individual (Possivel erro se tiverem 2 como grupo)

int get_artist_num_albuns_individual(Artista *artista, GestorAlbuns *gestor_albuns)
{
    int artist_id = get_artist_id(artista);
    GHashTable *hash_albuns = get_hash_albuns(gestor_albuns);

    int count = 0;
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, hash_albuns);

    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        Album *album = (Album *)value;

        // Itera pelos IDs dos artistas associados ao álbum
        gchar **artist_ids = get_album_artist_ids(album);
        if (artist_ids)
        {
            for (int i = 0; artist_ids[i] != NULL; i++)
            {
                int artist_ids_int = atoi(artist_ids[i] + 1);
                if (artist_ids_int == artist_id)
                {
                    count++;
                    break; // Já encontramos uma correspondência, podemos sair do loop
                }
            }
            g_strfreev(artist_ids);
        }
    }
    // free(artist_id);
    return count;
}

void calcula_streams(GestorSistema *gestorsis)
{ // atualiza o campo streams das músicas encontradas no histórico
    GestorHistories *gestorhistory = get_gestor_histories(gestorsis);
    GestorMusicas *gestormusicas = get_gestor_musicas(gestorsis);
    GHashTable *hash_history = get_hash_histories(gestorhistory);

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, hash_history);

    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        History *history = (History *)value;
        int music_id = get_history_music_id(history);
        Musica *musica = buscar_musicas(gestormusicas, music_id);
        if (musica)
        {
            int stream_count = get_music_streams(musica);
            stream_count++;
            set_music_streams(musica, stream_count);
        }
        // free(music_id);
    }
}

double calcular_receita_total_artista(Artista *artista, GestorArtistas *gestorartistas, GestorMusicas *gestormusicas)
{
    GHashTable *hash_musicas = get_hash_musicas(gestormusicas);
    int artist_id = get_artist_id(artista);

    double receita_artista = 0.0;

    // Iterar pelas músicas para calcular receita direta
    GHashTableIter music_iter;
    gpointer music_key, music_value;
    g_hash_table_iter_init(&music_iter, hash_musicas);
    while (g_hash_table_iter_next(&music_iter, &music_key, &music_value))
    {
        Musica *musica = (Musica *)music_value;

        // Verifica se a música pertence ao artista
        gchar **artist_ids = get_music_artist_ids(musica); // Obtém os IDs de artistas associados à música
        for (int i = 0; artist_ids && artist_ids[i] != NULL; i++)
        {
            int artist_id_int = atoi(artist_ids[i] + 1);
            if (artist_id_int == artist_id)
            {
                int stream_count = get_music_streams(musica);
                receita_artista += stream_count * get_artist_recipe_per_stream(artista);
                break;
            }
        }
        g_strfreev(artist_ids);
    }
    gchar *artist_type = get_artist_type(artista);

    // Verifica se o artista é individual; caso contrário, retorna apenas receita direta
    if (g_strcmp0(artist_type, "individual") != 0)
    {
        free(artist_type);
        return receita_artista;
    }

    // Calcular receita por participação em grupos
    double receita_participacao = 0.0;
    g_hash_table_iter_init(&music_iter, hash_musicas);
    while (g_hash_table_iter_next(&music_iter, &music_key, &music_value))
    {
        Musica *musica = (Musica *)music_value;
        gchar **artist_ids = get_music_artist_ids(musica);

        for (int i = 0; artist_ids && artist_ids[i]; i++)
        {
            int artist_id_int = atoi(artist_ids[i] + 1);
            Artista *grupo = buscar_artista(gestorartistas, artist_id_int);
            if (grupo && grupo != artista)
            {
                gchar **constituents = get_artist_id_constituent(grupo);
                int is_member = 0;
                for (int j = 0; constituents && constituents[j] != NULL; j++)
                {
                    int constituent_int = atoi(constituents[j] + 1);
                    if (constituent_int == artist_id)
                    {
                        is_member = 1;
                        break;
                    }
                }

                if (is_member)
                {
                    int stream_count = get_music_streams(musica);
                    receita_participacao += (stream_count * get_artist_recipe_per_stream(grupo)) / get_num_constituents(grupo);
                }
                g_strfreev(constituents);
            }
        }
        g_strfreev(artist_ids);
    }
    free(artist_type);
    //  Soma as receitas diretas e de participação
    return (receita_artista + receita_participacao);
}

// Função para contar o número de constituintes (membros) de um artista/grupo
int get_num_constituents(Artista *artista)
{
    gchar **constituents = get_artist_id_constituent(artista);
    int num_constituents = 0;

    // Conta o número de membros
    if (constituents)
    {
        while (constituents[num_constituents] != NULL)
        {
            num_constituents++;
        }
        g_strfreev(constituents); // Libera memória associada aos IDs
    }

    return num_constituents;
}
/*
char **lista_user_ids(GestorUsuarios *gestorusers)
{
    GHashTable *hashusers = get_hash_usuarios(gestorusers);
    if (!hashusers)
        return NULL;

    GList *user_ids_glist = g_hash_table_get_keys(hashusers); // os ids dos utilizadores são utilizados como keys na hashtable, basta extrair as keys com a funcção da glib e converter a GList em char**
    int num_users = g_list_length(user_ids_glist);

    char **user_ids = malloc((num_users + 1) * sizeof(char *));

    if (!user_ids)
    {
        g_list_free(user_ids_glist);
        return NULL;
    }

    int i = 0;
    for (GList *node = user_ids_glist; node != NULL; node = node->next, i++)
    {
        user_ids[i] = g_strdup((char *)node->data);
    }
    user_ids[i] = NULL;

    g_list_free(user_ids_glist);

    return user_ids;
}
*/
gint find_top_entry_with_tiebreaker(GHashTable *table, gboolean alphabetical)
{
    GHashTableIter iter;
    gpointer key, value;
    gint top_key = -1;
    gint64 top_value = -1;

    g_hash_table_iter_init(&iter, table);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {

        gint current_key = GPOINTER_TO_INT(key);
        gint64 current_value = GPOINTER_TO_INT(value);

        if (current_value > top_value ||
            (current_value == top_value && alphabetical && current_key < top_key))
        {
            top_key = current_key;
            top_value = current_value;
        }
    }
    return top_key;
}

gchar *find_top_entry_with_tiebreaker_str(GHashTable *table, gboolean alphabetical)
{
    GHashTableIter iter;
    gpointer key, value;
    gchar *top_key = NULL;
    gint64 top_value = -1;

    g_hash_table_iter_init(&iter, table);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        gchar *current_key = (gchar *)key;
        gint64 current_value = GPOINTER_TO_INT(value);

        if (current_value > top_value ||
            (current_value == top_value && alphabetical && g_strcmp0(current_key, top_key) < 0))
        {
            if (top_key)
            {
                g_free(top_key); // Free the previously allocated top_key
            }
            top_key = g_strdup(current_key); // Duplicate the new top_key
            top_value = current_value;
        }
    }
    return top_key;
}

gint compare_by_value_with_tiebreaker(gconstpointer a, gconstpointer b, gpointer user_data)
{
    UserData *data = (UserData *)user_data;
    GHashTable *table = data->hash;
    gboolean reverse = data->reverse;
    gboolean alphabetical = data->alphabetical;

    gint key_a = GPOINTER_TO_INT(a);
    gint key_b = GPOINTER_TO_INT(b);

    // Verificando se as chaves existem na tabela
    gpointer value_a_ptr = g_hash_table_lookup(table, GINT_TO_POINTER(key_a));
    gpointer value_b_ptr = g_hash_table_lookup(table, GINT_TO_POINTER(key_b));

    // Se algum valor for NULL, retornamos que a comparação não é válida
    if (value_a_ptr == NULL || value_b_ptr == NULL) {
        return 0;  // Ignoramos chaves ausentes
    }

    gint value_a = GPOINTER_TO_INT(value_a_ptr);
    gint value_b = GPOINTER_TO_INT(value_b_ptr);

    // Comparação dos valores
    if (value_a != value_b)
    {
        return reverse ? value_b - value_a : value_a - value_b;
    }

    // Se os valores forem iguais, realiza-se a ordenação alfabética
    if (alphabetical)
    {
        return key_a - key_b;
    }

    return 0; // Caso contrário, valores e chaves são iguais
}


// Função para ordenar a GHashTable por valor com desempate
GList *sort_hash_table_by_value_with_tiebreaker(GHashTable *table, gboolean reverse, gboolean alphabetical)
{
    // Obtém as chaves da tabela de hash
    GList *keys = g_hash_table_get_keys(table);

    // Verifica se a lista de chaves não está vazia
    if (keys == NULL) {
        return NULL; // Se a lista de chaves for NULL, não há nada a ordenar
    }

    // Cria o usuário de dados com a tabela e os parâmetros de ordenação
    UserData *user_data = create_user_data(table, reverse, alphabetical);

    // Ordena as chaves pela função de comparação
    keys = g_list_sort_with_data(keys, compare_by_value_with_tiebreaker, user_data);

    // Libera os dados do usuário após a ordenação
    free_user_data(user_data);
    return keys;
}

UserData *create_user_data(GHashTable *table, gboolean reverse, gboolean alphabetical)
{
    UserData *user_data = malloc(sizeof(UserData));
    if (user_data == NULL) {
        return NULL;  // Se a alocação falhar, retornamos NULL
    }
    user_data->hash = table;
    user_data->reverse = reverse;
    user_data->alphabetical = alphabetical;
    return user_data;
}

void free_user_data(UserData *user_data)
{
    if (user_data != NULL) {
        free(user_data);
    }
}

// Função para converter string de data para timestamp
time_t get_timestamp(const char *data) {
    struct tm tm_data = {0};
    if (sscanf(data, "%d/%d/%d", &tm_data.tm_year, &tm_data.tm_mon, &tm_data.tm_mday) != 3) {
        fprintf(stderr, "Erro: Formato de data inválido.\n");
        return -1;
    }
    tm_data.tm_year -= 1900;
    tm_data.tm_mon -= 1;
    return mktime(&tm_data);
}

// Calcula o domingo da semana de uma data
gchar* calcular_domingo(const gchar *data) {
    int ano, mes, dia;
    if (sscanf(data, "%d/%d/%d", &ano, &mes, &dia) != 3) {
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
    if (domingo == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a data de domingo.\n");
        return NULL;
    }

    strftime(domingo, 11, "%Y/%m/%d", &tm_data);
    return domingo;
}

// Calcula o sábado seguinte de uma data
gchar* calcular_sabado_seguinte(const gchar *data) {
    int ano, mes, dia;
    if (sscanf(data, "%d/%d/%d", &ano, &mes, &dia) != 3) {
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
    if (sabado == NULL) {
        fprintf(stderr, "Erro ao alocar memória para a data de sábado.\n");
        return NULL;
    }

    strftime(sabado, 11, "%Y/%m/%d", &tm_data);
    return sabado;
}


// Cria uma semana com domingo e sábado
Semana* criar_semana(const char *domingo) {
    Semana *semana = g_malloc(sizeof(Semana));

    g_strlcpy(semana->domingo, domingo, sizeof(semana->domingo));

    struct tm tm_domingo = {0};
    if (sscanf(domingo, "%d/%d/%d", &tm_domingo.tm_year, &tm_domingo.tm_mon, &tm_domingo.tm_mday) != 3) {
        fprintf(stderr, "Erro ao analisar a data.\n");
        g_free(semana);
        return NULL;
    }
    tm_domingo.tm_year -= 1900;
    tm_domingo.tm_mon -= 1;
    
    tm_domingo.tm_mday += 6;
    mktime(&tm_domingo);
    
    strftime(semana->sabado, sizeof(semana->sabado), "%Y/%m/%d", &tm_domingo);
    
    semana->artistas = g_hash_table_new(g_str_hash, g_str_equal);
    semana->top_artistas = NULL;

    return semana;
}


GHashTable *get_semanas_artistas(Semana *semana) {
    if (!semana) {
        return NULL;
    }
    return semana->artistas;
}


void destruir_semana(Semana *semana) {
    if (semana) {
        free(semana);
        semana = NULL;  // Evita um double free
    }
}



gint contar_aparicoes_artista(Semana *semana, const gchar *artist_id) {
    if (!semana || !artist_id) {
        fprintf(stderr, "Erro: Semana ou artist_id inválido.\n");
        return 0;
    }

    gint count = 0;
    for (GList *node = semana->top_artistas; node != NULL; node = node->next) {
        gchar *artist = (gchar *)node->data;
        if (artist && g_strcmp0(artist, artist_id) == 0) {
            count++;
        }
    }
    return count;
}


// Função para comparar duas datas no formato "yyyy/mm/dd"
gint comparar_datas(const char *data1, const char *data2) {
    time_t timestamp1 = get_timestamp(data1);
    time_t timestamp2 = get_timestamp(data2);

    if (timestamp1 == (time_t)-1 || timestamp2 == (time_t)-1) {
        return 0;  // Retorna 0 em caso de erro na conversão
    }

    if (timestamp1 < timestamp2) {
        return -1;  // data1 é anterior a data2
    } else if (timestamp1 > timestamp2) {
        return 1;   // data1 é posterior a data2
    }

    return 0;  // datas são iguais
}


// Atualiza os top artistas na semana
void atualizar_top_artistas_na_semana(Semana *semana) {
    if (!semana || !semana->artistas) {
        return;
    }

    GList *artistas_populares = NULL;
    GHashTableIter iter;
    gpointer key, value;

    // Cria lista de popularidade a partir da hash table
    g_hash_table_iter_init(&iter, semana->artistas);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        const char *artist_id = (const char *)key;
        int duracao_total_segundos = *(int *)value;

        // Cria estrutura de popularidade
        ArtistPopularity *popularity = malloc(sizeof(ArtistPopularity));
        popularity->id = g_strdup(artist_id);
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
    for (GList *node = artistas_populares; node != NULL && count < 15; node = node->next, count++) {
        ArtistPopularity *popularity = (ArtistPopularity *)node->data;
        if (count < 10) {
            top_artistas = g_list_append(top_artistas, g_strdup(popularity->id));
        }
    }

    // Substitui a lista de top_artistas na semana
    if (semana->top_artistas) {
        g_list_free_full(semana->top_artistas, g_free);
    }
    semana->top_artistas = top_artistas;

    // Limpa lista de popularidade
    liberar_popularidade(artistas_populares);
}

// Compara popularidade dos artistas
gint comparar_popularidade(struct ArtistPopularity *a, struct ArtistPopularity *b) {
    gint diff = b->total_vezes_no_top - a->total_vezes_no_top;

    // Se as durações forem iguais, compara pelo ID
    if (diff == 0) {
        return g_strcmp0(a->id, b->id);
    }

    return diff;
}


// Libera a memória da lista de popularidade
void liberar_popularidade(GList *popularity_list) {
    g_list_free_full(popularity_list, (GDestroyNotify)free_popularity);
}

// Libera memória associada a um objeto de popularidade
void free_popularity(ArtistPopularity *popularity) {
    if (popularity) {
        free(popularity->id);
        free(popularity->type);
        free(popularity);
    }
}

// Adiciona um artista na semana, somando a duração
void adicionar_artista_na_semana(Semana *semana, const char *artist_id, int duracao) {
    if (!semana || !artist_id) {
        fprintf(stderr, "Erro: Semana ou Artist ID é NULL.\n");
        return;
    }

    // Verifica se o artista já existe na hash table
    gpointer valor = g_hash_table_lookup(semana->artistas, artist_id);

    int duracao_existente = valor ? *(int *)valor : 0; // Se existir, obtém a duração; caso contrário, 0

    // Nova duração acumulada
    int nova_duracao = duracao_existente + duracao;

    if (valor) {
        // Atualiza o valor existente
        *(int *)valor = nova_duracao;
    } else {
        // Insere um novo valor
        int *nova_duracao_ptr = malloc(sizeof(int));
        *nova_duracao_ptr = nova_duracao;
        g_hash_table_insert(semana->artistas, g_strdup(artist_id), nova_duracao_ptr);
    }
}

// Retorna os top artistas da semana
GList* get_top_artistas_na_semana(Semana *semana) {
    if (!semana) {
        fprintf(stderr, "Erro: Semana é NULL.\n");
        return NULL;
    }
    return semana->top_artistas;
}

// Retorna a duração de um artista na semana
gint get_duracao_artista_na_semana(Semana *semana, const char *artist_id) {
    if (!semana || !semana->artistas || !artist_id) {
        return 0;
    }

    gint *duracao = g_hash_table_lookup(semana->artistas, artist_id);
    return duracao ? *duracao : 0;
}

void processar_historico (char* data_incial, char* data_final, GestorSistema *gestorsis, int line_number, int n) {

    int size = snprintf(NULL, 0, "resultados/command%d_output.txt", line_number) + 1;
    char *output_file_name = malloc(size);
    snprintf(output_file_name, size, "resultados/command%d_output.txt", line_number);
    RowWriter *writer = initialize_row_writer(output_file_name, WRITE_MODE_CSV);

    // Inicializa a tabela de semanas
    GHashTable *semanas = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)destruir_semana);

    // Obtém os históricos
    GestorHistories *gestorhistories = get_gestor_histories(gestorsis);
    GHashTable *historicos = get_hash_histories(gestorhistories);

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, historicos);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        History *history = (History *)value;

        char *data = get_history_date(history);

        char *domingo = calcular_domingo(data);
        if (domingo == NULL) {
        } else {

            // Duplica o valor de domingo para uso como chave na tabela hash
            char *domingo_key = g_strdup(domingo);

            // Obtenção ou criação da semana correspondente
            Semana *semana = g_hash_table_lookup(semanas, domingo_key);
            if (!semana) {
                semana = criar_semana(domingo);
                g_hash_table_insert(semanas, domingo_key, semana);
            } else {
                free(domingo_key);  // Libera a cópia, pois já existe na tabela hash
            }

            free(domingo);  // Libera o valor original de domingo

            // Processamento da música
            int musica_id = get_history_music_id(history);
            GestorMusicas *gestormusicas = get_gestor_musicas(gestorsis);

            Musica *musica = buscar_musicas(gestormusicas, musica_id);
            if (musica) {
                gchar **artist_ids = get_music_artist_ids(musica);
                gchar *duration = get_history_duration(history);
                int duracao = duracao_para_segundos(duration);
                free(duration);

                // Adiciona os artistas na semana correspondente
                for (int i = 0; artist_ids[i] != NULL; i++) {
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
    while (g_hash_table_iter_next(&semanas_iter, &semana_key, &semana_value)) {
        Semana *semana = (Semana *)semana_value;

        atualizar_top_artistas_na_semana(semana);

        GList *top_artistas = get_top_artistas_na_semana(semana);
        if (!top_artistas) {
            continue;
        }

        // Conta as aparições dos artistas no top 10
        for (GList *node = top_artistas; node != NULL; node = node->next) {
            char *artist_id = (char *)node->data;

            int *count = (int *)g_hash_table_lookup(top_10_count, artist_id);
            if (!count) {
                count = malloc(sizeof(int));
                *count = 1;
                g_hash_table_insert(top_10_count, g_strdup(artist_id), count);
            } else {
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

    while (g_hash_table_iter_next(&count_iter, &count_key, &count_value)) {
        char *artist_id = (char *)count_key;
        int count = *(int *)count_value;

        if (count > max_count) {
            max_count = count;
            if (top_artist_id) {
                free(top_artist_id);
            }
            top_artist_id = g_strdup(artist_id);
        }
    }

    // Registra o artista mais aparições
    if (top_artist_id) {
        int top_artist_int = atoi(top_artist_id+1);
        GestorArtistas *gestorartistas = get_gestor_artistas(gestorsis);
        Artista *artista = buscar_artista(gestorartistas,top_artist_int);
        if (artista) {
            char *artist_type = get_artist_type(artista);
            char *field_names[] = {"Artist_Id", "Type", "Count"};
            char *formatting[] = {"%s", "%s", "%d"};  // Aqui o %d para o count que é um inteiro
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

void processar_historico_intervalo_de_datas (char* data_inicial, char* data_final, GestorSistema *gestor_sistema, int line_number, int n) {

    // Arquivo de saída
    int size = snprintf(NULL, 0, "resultados/command%d_output.txt", line_number) + 1;
    char *output_file_name = malloc(size);
    snprintf(output_file_name, size, "resultados/command%d_output.txt", line_number);
    RowWriter *writer = initialize_row_writer(output_file_name, WRITE_MODE_CSV);


    // HashTable para armazenar semanas criadas
    GHashTable *semanas = g_hash_table_new_full(g_str_hash, g_str_equal, free, (GDestroyNotify)destruir_semana);

    GestorHistories *gestorhistories = get_gestor_histories(gestor_sistema);
    GHashTable *historicos = get_hash_histories(gestorhistories);

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, historicos);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        History *history = (History *)value;

        char *data = get_history_date(history);

        // Cálculo dos limites de data inicial e final
        char *domingo_inicial = calcular_domingo(data_inicial);
        char *sabado_final = calcular_sabado_seguinte(data_final);

        if (domingo_inicial == NULL || sabado_final == NULL) {
            free(domingo_inicial);
            free(sabado_final);
            return;  // Encerra o processamento devido ao erro
        }

        // Comparação com data inicial
        if (data_inicial && comparar_datas(data, domingo_inicial) < 0) {
            free(domingo_inicial);
            free(sabado_final);
            continue;
        }

        // Comparação com data final
        if (data_final && comparar_datas(data, sabado_final) > 0) {
            free(domingo_inicial);
            free(sabado_final);
            continue;
        }

        // Liberação de memória após uso
        free(domingo_inicial);
        free(sabado_final);

        // Cálculo do domingo correspondente à data
        char *domingo = calcular_domingo(data);
        if (domingo == NULL) {
            fprintf(stderr, "Erro ao calcular o domingo correspondente para a data %s.\n", data);
        } else {

            // Duplica o valor de domingo para uso como chave na tabela hash
            char *domingo_key = g_strdup(domingo);

            // Obtenção ou criação da semana correspondente
            Semana *semana = g_hash_table_lookup(semanas, domingo_key);
            if (!semana) {
                semana = criar_semana(domingo);
                g_hash_table_insert(semanas, domingo_key, semana);
            } else {
                free(domingo_key);  // Libera a cópia, pois já existe na tabela hash
            }

            free(domingo);  // Libera o valor original de domingo

            // Processamento da música
            int musica_id = get_history_music_id(history);
            GestorMusicas *gestormusicas = get_gestor_musicas(gestor_sistema);

            Musica *musica = buscar_musicas(gestormusicas,musica_id);
            if (musica) {
                gchar **artist_ids = get_music_artist_ids(musica);
                gchar *duration = get_history_duration(history);
                int duracao = duracao_para_segundos(duration);
                free(duration);

                // Adiciona os artistas na semana correspondente
                for (int i = 0; artist_ids[i] != NULL; i++) {
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

    while (g_hash_table_iter_next(&semanas_iter, &semana_key, &semana_value)) {
        Semana *semana = (Semana *)semana_value;

        atualizar_top_artistas_na_semana(semana);

        GList *top_artistas = get_top_artistas_na_semana(semana);
        if (!top_artistas) {
            continue;
        }

        for (GList *node = top_artistas; node != NULL; node = node->next) {
            char *artist_id = (char *)node->data;

            int *count = (int *)g_hash_table_lookup(top_10_count, artist_id);
            if (!count) {
                count = malloc(sizeof(int));
                *count = 1;
                g_hash_table_insert(top_10_count, g_strdup(artist_id), count);
            } else {
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

    while (g_hash_table_iter_next(&count_iter, &count_key, &count_value)) {
        char *artist_id = (char *)count_key;
        int count = *(int *)count_value;

        if (count > max_count) {
            max_count = count;
            if (top_artist_id) {
                free(top_artist_id);
            }
            top_artist_id = g_strdup(artist_id);
        }
    }

    // Registra o artista mais aparições
    if (top_artist_id) {
        int top_artist_int = atoi(top_artist_id+1);
        GestorArtistas *gestorartistas = get_gestor_artistas(gestor_sistema);
        Artista *artista = buscar_artista(gestorartistas,top_artist_int);
        if (artista) {
            char *artist_type = get_artist_type(artista);
            char *field_names[] = {"Artist_Id", "Type", "Count"};
            char *formatting[] = {"%s", "%s", "%d"};  // Aqui o %d para o count que é um inteiro
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
