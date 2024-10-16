#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../include/entidades/musica.h"
#include "../include/gestores/gestor_musicas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/entidades/usuario.h"

// Função para buscar uma música pelo ID único de música
Musica* buscar_musicas(GestorMusicas *gestor, const gint id) {
    return (Musica*) g_hash_table_lookup(gestor->musicas, GINT_TO_POINTER(id));
}

// Função para determinar o número total de likes de um gênero
int contaLikes(const gchar *genero, const Usuario *usuario) {
    // Implementar lógica para contar likes
}

// Função para validar a duração
bool tudoNum(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

bool validaDuracao(const Musica *musica) {
    const char *duracao = musica->duration;

    if (strlen(duracao) != 8 || duracao[2] != ':' || duracao[5] != ':') 
        return false;

    char horasStr[3] = {duracao[0], duracao[1], '\0'};
    char minutosStr[3] = {duracao[3], duracao[4], '\0'};
    char segundosStr[3] = {duracao[6], duracao[7], '\0'};

    if (!tudoNum(horasStr) || !tudoNum(minutosStr) || !tudoNum(segundosStr)) {
        return false;
    }

    int horas = atoi(horasStr);
    int minutos = atoi(minutosStr);
    int segundos = atoi(segundosStr);

    if (horas < 0 || horas > 99 || minutos < 0 || minutos > 59 || segundos < 0 || segundos > 59) {
        return false;
    }

    return true;
}
