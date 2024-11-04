#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../../include/entidades/musica.h"
#include "../../include/gestores/gestor_musicas.h"
#include "../../include/gestores/gestor_usuarios.h"
#include "../../include/entidades/usuario.h"
#include "../../include/entidades/artists.h"
#include "../../include/parsing/rowreader.h"

char **parse_liked_musics (RowReader* reader) {
   char** liked_musics_id = reader_next_cell(reader);
   if ( liked_musics_id == NULL || strlen(liked_musics_id <=2)) return 0;

// Remove os parentesis
    char* liked_musics_copy = g_strdup(liked_musics_id + 1);
    liked_musics_copy[strlen(liked_musics_copy) - 1] = '\0';

    int conta =0;
    char * temp = liked_musics_copy;


    while (strrchr (temp, ", ")){
        conta++;
        temp++;
    }
    conta++; // conta tambem o ultimo id

 // Aloca memória para o array de strings
    char** liked_musics = malloc((conta + 1) * sizeof(char*));
    if (!liked_musics) {
        g_free(liked_musics_copy);
        return NULL;
    }

   int i=0;
   char* token = strtok (liked_musics_copy, ", ");
   while ((token) != NULL) {
    liked_musics [i++] = g_strdup (token);
    token = strtok(NULL, ", '");
   }
   liked_musics [i] = NULL; // Fechamos o array

g_free (liked_musics_copy);
return liked_musics;

}



