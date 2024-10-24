#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../../include/entidades/artists.h"
#include "../../include/gestores/gestor_artistas.h"
#include "../../include/gestores/gestor_musicas.h"
#include "../../include/entidades/musica.h"

gboolean valida_artista_individual(const Artista *artista) {
    // Verifica se o artista é nulo
    if (artista == NULL) {
        return FALSE;
    } 

    // Verifica se o tipo do artista é "individual"
    if (g_strcmp0(artista->type, "individual") == 0) {
        // Verifica se o id_constituent é nulo, já que um artista individual não deve ter constituintes
        if (artista->id_constituent != NULL) {
            return FALSE;  // Artista individual não pode ter constituinte
        }
    }

    // Valida se campos obrigatórios estão preenchidos
    if (artista->id == NULL || artista->name == NULL || 
        artista->recipe_per_stream <= 0 || artista->country == NULL || 
        artista->type == NULL) {
        return FALSE;  // Algum campo obrigatório está faltando
    }

    return TRUE;  // Artista individual válido
}
