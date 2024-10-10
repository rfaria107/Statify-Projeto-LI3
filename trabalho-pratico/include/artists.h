#ifndef ARTISTA_H
#define ARTISTA_H

#define MAX_NAME_LENGTH 50
#define MAX_DESCRIPTION_LENGTH 200

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    char description[MAX_DESCRIPTION_LENGTH];
    double recipe_per_stream;
    char id_constituent[MAX_NAME_LENGTH]; // 
    char country[MAX_NAME_LENGTH];
    char type[MAX_NAME_LENGTH]; // individual ou grupo musical
} Artista;

#endif // ARTISTA_H
