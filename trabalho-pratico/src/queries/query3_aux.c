#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include "../include/queries/query3_aux.h"

struct GenrePopularity
{
    char *genre;
    int total_likes;
};

// Função para criar uma nova instância de GenrePopularity
GenrePopularity *create_genre_popularity(const char *genre)
{
    GenrePopularity *gp = malloc(sizeof(GenrePopularity));
    gp->genre = g_strdup(genre); // Copia a string para evitar problemas de ponteiros
    gp->total_likes = 0;
    return gp;
}

char *get_gp_genre(GenrePopularity *gp)
{
    return g_strdup(gp->genre);
}

void set_gp_genre(GenrePopularity *gp, char *genre)
{
    if (gp->genre)
    {
        g_free(gp->genre);
    }
    gp->genre = g_strdup(genre);
}

int get_gp_total_likes(GenrePopularity *gp)
{
    return gp->total_likes;
}

void set_gp_total_likes(GenrePopularity *gp, int total_likes)
{
    gp->total_likes = total_likes;
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


// Função para liberar a memória de GenrePopularity
void free_genre_popularity(GenrePopularity *gp)
{
    g_free(gp->genre);
    free(gp);
}
