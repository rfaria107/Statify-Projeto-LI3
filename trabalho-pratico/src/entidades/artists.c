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
