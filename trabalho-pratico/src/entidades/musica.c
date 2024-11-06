#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../../include/entidades/musica.h"
#include "../../include/gestores/gestor_musicas.h"
#include "../../include/gestores/gestor_usuarios.h"
#include "../../include/gestores/gestor_artistas.h"
#include "../../include/entidades/usuario.h"
#include "../../include/entidades/artists.h"
#include "../../include/parsing/rowreader.h"

struct Musica
{
    gint id;            // ID único da música
    gchar *title;       // Título da música (string dinâmica)
    gchar **artist_ids; // Lista de IDs dos artistas (array dinâmico de strings)
    gchar *duration;    // Duração da música em segundos
    gchar *genre;       // Gênero (string dinâmica)
    gint year;          // Ano de lançamento
    gchar *lyrics;      // Letra da música (string dinâmica)
};

// Função para criar músicas

gint *get_music_id(Musica *musica){
    return ((gint *) (musica->id));
}

Musica* create_musica(int id, char *title, char **artist_ids,
                      char *duration, char *genre, int year,
                      char *lyrics)
{
    Musica* musica = inicializar_musica();

    // Define os atributos da musica;
    musica->id = id;
    musica->year = year;
    musica->title = g_strdup(title);
    musica->duration = g_strdup(duration);
    musica->genre = g_strdup(genre);
    musica->lyrics = g_strdup(lyrics);

    if (artist_ids)
    {
        int count = 0;
        while (artist_ids[count] != NULL)
        {
            count++; // Conta quantos IDs de artista existem
        }

        musica->artist_ids = g_new(gchar *, count + 1); // Aloca espaço para o array
        for (int i = 0; i < count; i++)
        {
            musica->artist_ids[i] = g_strdup(artist_ids[i]); // Duplica cada string
        }
        musica->artist_ids[count] = NULL; // Termina o array com NULL
    }
    else
    {
        musica->artist_ids = NULL; // Caso artist_ids seja NULL
    }

    return musica;
}

Musica* inicializar_musica()
{
    Musica *musica = malloc(sizeof(Musica));
    if (!musica)
    {
        return NULL;
    }

    musica->id = 0;
    musica->title = NULL;
    musica->artist_ids = NULL;
    ;
    musica->duration = NULL;
    musica->genre = NULL;
    musica->year = 0;
    musica->lyrics = NULL;

    return musica;
}

void free_musica(Musica* musica)
{
    if (musica)
    {
        // Libera os campos dinâmicos da estrutura
        g_free(musica->title);

        // Libera cada string no array artist_ids
        if (musica->artist_ids)
        {
            for (int i = 0; musica->artist_ids[i] != NULL; i++)
            {
                g_free(musica->artist_ids[i]); // Libera cada ID do artista
            }
            g_free(musica->artist_ids); // Libera o array de IDs
        }

        g_free(musica->duration);
        g_free(musica->genre);
        g_free(musica->lyrics);

        // Finalmente, libera a estrutura Musica
        g_free(musica);
    }
}

int parse_musica_and_add_him(RowReader* reader, GestorMusicas* gestorMusic)
{

    gchar *id_str = reader_next_cell(reader);
    //if (is_empty_value(id_str))
    //    return 1;
    int id = atoi(id_str);

    gchar *title = reader_next_cell(reader);
    //if (invalid_email(title))
    //    return 1;

    gchar *artist_ids_str = reader_next_cell(reader);
    //if (is_empty_value(artist_ids_str))
    //    return 1;
    gchar **artist_ids = g_strsplit(artist_ids_str, ",", -1);

    gchar *duration = reader_next_cell(reader);
    //if (is_empty_value(duration))
    //    return 1;

    gchar *genre = reader_next_cell(reader);
    //if (invalid_date(genre))
    //    return 1;

    char *year_str = reader_next_cell(reader);
    //if (is_empty_value(year_str))
    //    return 1;
    int year = atoi(year_str);

    gchar *lyrics = reader_next_cell(reader);
    //if (is_empty_value(lyrics))
    //    return 1;

    Musica *musica = create_musica(id, title, artist_ids, duration, genre, year, lyrics);

    if (!musica)
        return 1;

    inicializar_musica(gestorMusic, musica);

    free_musica(musica);

    return 0;
}

char **parse_liked_musics(RowReader* reader)
{
    char *liked_musics_id = reader_next_cell(reader);

    if (liked_musics_id == NULL || strlen(liked_musics_id) <= 2)
        return NULL;

    // Remove os parênteses
    char *liked_musics_copy = g_strdup(liked_musics_id + 1); // Ignora o primeiro caractere
    liked_musics_copy[strlen(liked_musics_copy) - 1] = '\0'; // Remove o último caractere

    // Conta quantos IDs existem
    int count = 0;
    char *temp = liked_musics_copy;
    while (strchr(temp, ','))
    {
        count++;
        temp = strchr(temp, ',') + 1; // Move para o próximo caractere após a vírgula
    }
    count++; // Conta também o último ID

    // Aloca memória para o array de strings
    char **liked_musics = malloc((count + 1) * sizeof(char *));
    if (!liked_musics)
    {
        g_free(liked_musics_copy);
        return NULL;
    }

    // Tokeniza a string e armazena os IDs no array
    int i = 0;
    char *token = strtok(liked_musics_copy, ", ");
    while (token != NULL)
    {
        liked_musics[i++] = g_strdup(token); // Duplica o token para o array
        token = strtok(NULL, ", ");
    }
    liked_musics[i] = NULL;

    // Libera a memória
    g_free(liked_musics_copy);
    return liked_musics;
}

//funções de validação

gboolean tudoNum(const char *str)
{
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit(str[i]))
        {
            return FALSE;
        }
    }
    return TRUE;
}

gboolean validaDuracao(const Musica *musica)
{
    const gchar *duracao = musica->duration;

    if (strlen(duracao) != 8 || duracao[2] != ':' || duracao[5] != ':')
        return FALSE;

    char horasStr[3] = {duracao[0], duracao[1], '\0'};
    char minutosStr[3] = {duracao[3], duracao[4], '\0'};
    char segundosStr[3] = {duracao[6], duracao[7], '\0'};

    if (!tudoNum(horasStr) || !tudoNum(minutosStr) || !tudoNum(segundosStr))
    {
        return FALSE;
    }

    int horas = atoi(horasStr);
    int minutos = atoi(minutosStr);
    int segundos = atoi(segundosStr);

    if (horas < 0 || horas > 99 || minutos < 0 || minutos > 59 || segundos < 0 || segundos > 59)
    {
        return FALSE;
    }

    return TRUE;
}

gboolean valida_artist_id(const Musica *musica, GestorArtistas *gestoratistas)
{
    GHashTable *hash_artistas = get_hash_artistas(gestoratistas);
    for (int i = 0; musica->artist_ids[i] != NULL; i++)
    {

        Artista *artista_encontrado = (Artista *)g_hash_table_lookup(hash_artistas, musica->artist_ids[i]);

        if (artista_encontrado == NULL)
        {
            return FALSE;
        }
    }
    return TRUE;
}

// Função para converter "HH:MM:SS" para segundos
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

/*
gchar *calcular_discografia(GestorArtistas *gestorartistas, const Artista *artista)
{
    GHashTable *hash_musicas = get_hash_table(gestorartistas);
    gint duracao_total_segundos = 0;

    for (int i = 0; artista->id_constituent != NULL && artista->id_constituent[i] != NULL; i++)
    {
        gchar *id_musica = artista->id_constituent[i];
        Musica *musica = (Musica *)g_hash_table_lookup(hash_musicas, id_musica);

        if (musica != NULL)
        {
            duracao_total_segundos += duracao_para_segundos(musica->duration);
        }
    }

    return segundos_para_duracao(duracao_total_segundos);
}

void genero_mais_curtido(int idade_min, int idade_max, GestorUsuarios *gestor_usuarios, GestorMusicas *gestor_musicas)
{
    GHashTable *hash_usuarios = get_hash_table_usuarios(gestor_usuarios);
    GHashTableIter iter;
    gpointer key, value;
    GHashTable *genero_contagem = g_hash_table_new(g_str_hash, g_str_equal);

    // Itera pelos usuários
    g_hash_table_iter_init(&iter, hash_usuarios);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        Usuario *usuario = (Usuario *)value;
        int idade = calcularIdade(usuario);

        // Verifica se o usuário está dentro da faixa etária
        if (idade >= idade_min && idade <= idade_max)
        {
            // Itera pelas músicas curtidas pelo usuário
            for (int i = 0; usuario->liked_musics_id[i] != NULL; i++)
            {
                // Converte o ID da música de string para inteiro, se necessário
                int music_id = atoi(usuario->liked_musics_id[i]);

                // Busca a música pelo ID
                Musica *musica = buscar_musicas(gestor_musicas, music_id);

                if (musica != NULL)
                {
                    const char *genero = musica->genre; // Obtém o gênero da música

                    // Atualiza a contagem de cada gênero
                    gpointer count_ptr = g_hash_table_lookup(genero_contagem, genero);
                    int count = (count_ptr != NULL) ? GPOINTER_TO_INT(count_ptr) : 0;
                    g_hash_table_insert(genero_contagem, g_strdup(genero), GINT_TO_POINTER(count + 1));
                }
            }
        }
    }

    // Determina o gênero mais popular
    GHashTableIter contagem_iter;
    gpointer genero, contagem;
    const char *genero_mais_popular = NULL;
    int maior_contagem = 0;

    g_hash_table_iter_init(&contagem_iter, genero_contagem);
    while (g_hash_table_iter_next(&contagem_iter, &genero, &contagem))
    {
        int count = GPOINTER_TO_INT(contagem);
        if (count > maior_contagem)
        {
            maior_contagem = count;
            genero_mais_popular = (const char *)genero;
        }
    }

    // Imprime o resultado
    if (genero_mais_popular != NULL)
    {
        printf("O gênero mais popular é: %s\n", genero_mais_popular);
    }
    else
    {
        printf("Nenhum gênero foi encontrado.\n");
    }

    // Liberta a memória da hashtable temporaria criada para recolher os gêneros
    g_hash_table_destroy(genero_contagem);
}
*/