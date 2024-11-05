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

// Função para criar músicas

Musica* create_musica(int id, char* title,char** artist_ids,
                      char* duration, char* genre,int year,
                      char* lyrics){
    Musica* musica= inicializar_musica();

    // Define os atributos da musica;
    musica->id = g_strdup(id);
    musica->title = g_strdup(title);
    musica->artist_ids = g_strdup(artist_ids);
    musica->duration = g_strdup(duration);
    musica->genre = g_strdup(genre);
    musica->year = g_strdup(year);
    musica->lyrics = g_strdup(lyrics);
    
    return musica;
}

Musica* inicializar_musica() {
    Musica* musica = (Musica*)malloc(sizeof(Musica));
    if (!musica) {
        return NULL;
    }

    musica->id = NULL;
    musica->title = NULL;
    musica->artist_ids = NULL;;
    musica->duration = NULL;
    musica->genre = NULL;
    musica->year = NULL;
    musica->lyrics = NULL;

    return musica; 
}

void free_musica (Musica* musica) {
    if (musica) {
        g_free(musica->id);
        g_free(musica->title);
        g_free(musica->artist_ids);
        g_free(musica->duration);
        g_free(musica->genre);
        g_free(musica->year);
        g_free(musica->lyrics);

        g_free(musica);
    }
}

int parse_musica_and_add_him(RowReader* reader, GestorMusicas* gestorMusic) {

    int id = reader_next_cell(reader);
    if (is_empty_value(id)) return 1;

    char* title = reader_next_cell(reader);
    if (invalid_email(title)) return 1;

    char* artist_ids = reader_next_cell(reader);
    if (is_empty_value(artist_ids)) return 1;

    char* duration = reader_next_cell(reader);
    if (is_empty_value(duration)) return 1;

    char* genre = reader_next_cell(reader);
    if (invalid_date(genre)) return 1;

    char* year = reader_next_cell(reader);
    if (is_empty_value(year)) return 1;

    char* lyrics = reader_next_cell(reader);
    if (is_empty_value(lyrics)) return 1;

    Musica* musica = create_musica(id, title, artist_ids, duration, genre, year,lyrics);

    if (!musica) return 1;

    adicionar_musica(gestorMusic, musica);

    free_musica(musica);

    return 0;
}


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
