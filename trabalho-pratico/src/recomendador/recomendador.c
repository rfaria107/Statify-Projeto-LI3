#include "../include/recomendador/recomendador.h"
#include <stdlib.h>

char **recomendaUtilizadores(char *idUtilizadorAlvo,
                             int **matrizClassificacaoMusicas,
                             char **idsUtilizadores,
                             char **nomesGeneros,
                             int numUtilizadores,
                             int numGeneros,
                             int numRecomendacoes) {
    // Exemplo simples de implementação.
    // Substitua isso pela lógica real.
    char **recomendados = malloc(numRecomendacoes * sizeof(char *));
    for (int i = 0; i < numRecomendacoes && i < numUtilizadores; i++) {
        recomendados[i] = idsUtilizadores[i];
    }
    return recomendados;
}
