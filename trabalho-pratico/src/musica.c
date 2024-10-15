#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../include/musica.h"
#include "../include/gestor_sistemas.h"

// Função para buscar uma musica pelo id unico de musica

Musica* buscar_musicas(GestorSistema *gestor, const char *musicas) {
    if (validaDuracao){ 
    return (Musica*) g_hash_table_lookup(gestor->musicas, id);}
}

// Função para determinar o numero de likes total de um género

int contaLikes (genero[], const Usuario *usuario) {
    
}

// Funçao para validar Duração

bool tudoNum(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

bool validaDuracao (const Musica *musica) {
    const char *duracao = musica->duration;

    if (strlen (duracao) != 8 || duracao[2] != ':' || duracao[2] != ':') return false;

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