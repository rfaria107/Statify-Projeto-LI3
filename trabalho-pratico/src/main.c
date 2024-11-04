#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/gestores/gestor_sistemas.h" // Inclua seu cabeçalho do GestorSistemas


int main() {
    GestorSistemas *gestor = inicializar_gestor_sistemas(); // Inicializa o gestor
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
