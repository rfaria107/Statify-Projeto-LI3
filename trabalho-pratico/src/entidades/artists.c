#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../../include/entidades/artists.h"
#include "../include/gestores/gestor_artistas.h"
#include "../../include/gestores/gestor_muscias.h"
#include "../../include/entidades/musica.h"

// Função que valida se um artista individual nao tem elementos.  individual/group
gboolean valida_artista_individual(const Artista *artista) {

    if (g_strcmp0(artista->type, "individual") == 0) {

        if (artista->id_constituent != NULL) {
            return FALSE;
        }
    }
    return TRUE;
}

void free_artista(Artista artista)
{
    if (!artista)
        return;

    g_free(artista->id);
    g_free(artista->name);
    g_free(artista->description);
    // recipe per stream é um double
    if (artista->id_constituent)
    {
        for (gint i = 0; artista->id_constituent[i] != NULL; i++)
        {
            g_free(artista->id_constituent[i]);
        }
        g_free(artista->id_constituent); // Free array
    }
    g_free(artista->country);
    g_free(artista->type);
    g_free(artista);
}