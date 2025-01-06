#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include "../include/entidades/musica.h"
#include "../include/entidades/history.h"
#include "../include/gestores/gestor_musicas.h"
#include "../include/gestores/gestor_histories.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/queries/query5_aux.h"
#include "../include/gestores/gestor_sistemas.h"

char **preprocessIdsUtilizadores(GestorSistema *gestorsis, int *numUtilizadores)
{
    GestorUsuarios *gestorusers = get_gestor_usuarios(gestorsis);
    GHashTable *hashusers = get_hash_usuarios(gestorusers);
    // encontrar o numero de utilizadores
    *numUtilizadores = g_hash_table_size(hashusers);
    // alocar e inicializar memoria com 0 g_malloc0
    char **idsUtilizadores = g_malloc(*numUtilizadores * sizeof(char *));

    GHashTableIter iter;
    gpointer key, value;
    int index = 0;

    // iterar sobre a hashtable dos users e guardar os ids na lista de strings
    g_hash_table_iter_init(&iter, hashusers);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        gint user_id = GPOINTER_TO_INT(key);
        idsUtilizadores[index] = g_strdup_printf("U%07d", user_id); // Converter para string (o tipo do id é int)
        index++;
    }

    return idsUtilizadores;
}

char **preprocessNomesGeneros(GestorSistema *gestorsis, int *numGeneros)
{
    GestorMusicas *gestormusicas = get_gestor_musicas(gestorsis);
    GHashTable *hashmusicas = get_hash_musicas(gestormusicas);
    // usar uma hashtable para não copiar generos ja existentes
    GHashTable *uniqueGenres = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

    GHashTableIter iter;
    gpointer key, value;

    // iterar na hash das musicas e guardar os generos se nao estiverem contidos na hashtable dos generos
    g_hash_table_iter_init(&iter, hashmusicas);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        Musica *musica = (Musica *)value;
        char *genre = get_music_genre(musica);
        if (genre && !g_hash_table_contains(uniqueGenres, genre))
        {
            g_hash_table_add(uniqueGenres, g_strdup(genre));
            free(genre);
        }
        else
            free(genre);
    }

    // calcular numero de generos únicos
    *numGeneros = g_hash_table_size(uniqueGenres);

    // alocar e inicializar o array dos nomes dos generos
    char **nomesGeneros = g_malloc(*numGeneros * sizeof(char *));
    int index = 0;

    GHashTableIter genreIter;
    gpointer genreKey;

    // transferir os generos da hashtable dos generos unicos para o array
    g_hash_table_iter_init(&genreIter, uniqueGenres);
    while (g_hash_table_iter_next(&genreIter, &genreKey, NULL))
    {
        nomesGeneros[index] = g_strdup((char *)genreKey);
        index++;
    }

    // destruir a hashtable temporaria dos generos
    g_hash_table_destroy(uniqueGenres);

    return nomesGeneros;
}

void freeMatrizClassificacaoMusicas(int **matriz, int numUtilizadores)
{
    for (int i = 0; i < numUtilizadores; i++)
    {
        free(matriz[i]);
    }
    free(matriz);
}

int **createMatrizClassificacaoMusicas(int numUtilizadores, int numGeneros)
{
    // alocar memoria para as linhas
    int **matriz = (int **)g_malloc0(numUtilizadores * sizeof(int *));
    if (!matriz)
    {
        return NULL;
    }

    // alocar memoria para as colunas
    for (int i = 0; i < numUtilizadores; i++)
    {
        matriz[i] = (int *)g_malloc0(numGeneros * sizeof(int));
        if (!matriz[i])
        {
            // Free em caso de erro
            for (int j = 0; j < i; j++)
            {
                free(matriz[j]);
            }
            free(matriz);
            return NULL;
        }
    }

    return matriz;
}

void calculaMatrizClassificacaoMusicas(int **matriz, char **ids_utilizadores, char **nomes_generos,
                                       int num_utilizadores, int num_generos, GestorSistema *gestorsis)
{
    if (num_utilizadores <= 0 || num_generos <= 0)
    {
        return;
    }
    GestorHistories *gestorhistories = get_gestor_histories(gestorsis);
    GestorMusicas *gestormusicas = get_gestor_musicas(gestorsis);
    GHashTable *hashhistory = get_hash_histories(gestorhistories);

    // UPDATE colocar os users numa hashtable,deste modo apenas preciso de fazer lookup (O(1)) em vez de iterar sobre todos os users para cada entrada no historico
    GHashTable *userIndexMap = g_hash_table_new(g_direct_hash, g_direct_equal);
    for (int i = 0; i < num_utilizadores; i++)
    {
        int id_utilizador_int = atoi(ids_utilizadores[i] + 1); // Convert to int
        g_hash_table_insert(userIndexMap, GINT_TO_POINTER(id_utilizador_int), GINT_TO_POINTER(i));
    }

    // iterar sobre o historico
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, hashhistory);
    while (g_hash_table_iter_next(&iter, &key, &value))
    {
        History *history = (History *)value;
        int history_user_id = get_history_user_id(history);

        // encontrar a posição do user que ouviu a musica na hashtable para saber a linha onde incrementar o valor
        int userIndex = GPOINTER_TO_INT(g_hash_table_lookup(userIndexMap, GINT_TO_POINTER(history_user_id)));
        if (userIndex == -1)
        {
            continue; // User not found
        }

        int history_music_id = get_history_music_id(history);
        Musica *musica = buscar_musicas(gestormusicas, history_music_id);
        if (!musica)
        {
            continue; // skip se nao encontrar musica ou genero
        }

        char *genre = get_music_genre(musica);

        // encontrar a coluna onde incrementar (indice do genero no array nomesgeneros)
        int genreIndex = -1;
        for (int j = 0; j < num_generos; j++)
        {
            if (g_strcmp0(nomes_generos[j], genre) == 0)
            {
                genreIndex = j;
                free(genre);
                break;
            }
        }

        // se o genero não estiver na lista skipar(improvável acontecer mas..)
        if (genreIndex == -1)
        {
            free(genre);
            continue;
        }

        // incrementar a celula correta na matriz
        matriz[userIndex][genreIndex]++;
    }
    g_hash_table_destroy(userIndexMap);
}