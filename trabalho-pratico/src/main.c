#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/gestores/gestor_sistemas.h"
#include "../include/parsing/parser.h"

int main(int argc, char* argv[]) {
    GestorSistema *gestor = inicializar_gestor_sistema(gestor); // Inicializa o gestor

    FILE *artistas = fopen(strcat(argv[2], "/com_erros/artists.csv"), "r");
    // dar parse aos artistas
    parser_principal(artistas, gestor, 'a');
    fclose(artistas);
    FILE *users = fopen(strcat(argv[2], "/com_erros/users.csv"), "r");
    // dar parse aos users
    parser_principal(users, gestor, 'u');
    fclose(users);
    FILE *musicas = fopen(strcat(argv[2], "/com_erros/musics.csv"), "r");
    // dar parse às musicas
    parser_principal(musicas, gestor, 'm');
    fclose(musicas);

    FILE *input_file = fopen("input_exemplo.txt", "r");
    char command[100]; // Ajuste o tamanho conforme necessário

    while (fgets(command, sizeof(command), input_file)) {
        char *token = strtok(command, " ");
        if (strcmp(token, "1") == 0) {
            char *user_id = strtok(NULL, " ");
            query_user_summary(gestor, user_id);
        } else if (strcmp(token, "2") == 0) {
            int N = atoi(strtok(NULL, " "));
            char *country = strtok(NULL, " ");
            query_top_artists(gestor, N, country);
        } else if (strcmp(token, "3") == 0) {
            int min_age = atoi(strtok(NULL, " "));
            int max_age = atoi(strtok(NULL, " "));
            query_popular_genres(gestor, min_age, max_age);
        }
    }

    fclose(input_file);
    free_gestor_sistemas(gestor); // Libera a memória do gestor
    return 0;
}
