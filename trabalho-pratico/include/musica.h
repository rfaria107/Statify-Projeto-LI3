#ifndef MUSICA_H
#define MUSICA_H
#define MAX_NAME_LENGTH 50

#define MAX_TITLE_LENGTH 100
#define MAX_GENRE_LENGTH 30
#define MAX_LYRICS_LENGTH 1000

typedef struct {
    int id;
    char title[MAX_TITLE_LENGTH];
    char artist_id[MAX_NAME_LENGTH]; // Lista de IDs dos artistas
    char duration; // em segundos
    char genre[MAX_GENRE_LENGTH];
    int year;
    char lyrics[MAX_LYRICS_LENGTH];
} Musica;

// Funções de manipulação de músicas
Musica* buscar_musicas(GestorSistema *gestor, const char *musicas)

// Função valida
bool validaDuracao (const Musica *musica);

#endif // MUSICA_H
