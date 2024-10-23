#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "../include/entidades/musica.h"
#include "../include/gestores/gestor_musicas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/entidades/usuario.h"
#include "../include/entidades/artists.h"

// Função para buscar uma música pelo ID único de música
Musica* buscar_musicas(GestorMusicas *gestor, const gint id) {
    return (Musica*) g_hash_table_lookup(gestor->musicas, GINT_TO_POINTER(id));
}

// Função para validar a duração
gboolean tudoNum(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isdigit(str[i])) {
            return FALSE;
        }
    }
    return TRUE;
}

gboolean validaDuracao(const Musica *musica) {
    const gchar *duracao = musica->duration;

    if (strlen(duracao) != 8 || duracao[2] != ':' || duracao[5] != ':') 
        return FALSE;

    char horasStr[3] = {duracao[0], duracao[1], '\0'};
    char minutosStr[3] = {duracao[3], duracao[4], '\0'};
    char segundosStr[3] = {duracao[6], duracao[7], '\0'};

    if (!tudoNum(horasStr) || !tudoNum(minutosStr) || !tudoNum(segundosStr)) {
        return FALSE;
    }

    int horas = atoi(horasStr);
    int minutos = atoi(minutosStr);
    int segundos = atoi(segundosStr);

    if (horas < 0 || horas > 99 || minutos < 0 || minutos > 59 || segundos < 0 || segundos > 59) {
        return FALSE;
    }

    return TRUE;
}

// Função que valida se o artista existe e é válido
gboolean valida_artist_id(const Musica *musica, GHashTable *artistas) {
    for (int i = 0; musica->artist_ids[i] != NULL; i++) {

        Artista *artista_encontrado = (Artista *)g_hash_table_lookup(artistas, musica ->artist_ids[i]);

        if (artista_encontrado == NULL) {
            return FALSE; 
        }
    }
    return TRUE;
}

// Função para converter "HH:MM:SS" para segundos
gint duracao_para_segundos(const gchar* duracao) {
    gint horas, minutos, segundos;
    sscanf(duracao, "%d:%d:%d", &horas, &minutos, &segundos);
    return horas * 3600 + minutos * 60 + segundos;
}

// Função para converter segundos em "HH:MM:SS"
gchar* segundos_para_duracao(gint total_segundos) {
    gint horas = total_segundos / 3600;
    gint minutos = (total_segundos % 3600) / 60;
    gint segundos = total_segundos % 60;
    
    return g_strdup_printf("%02d:%02d:%02d", horas, minutos, segundos);
}

// Função para calcular a discografia de cada artista
gchar* calcular_discografia(GHashTable *musicas, const Artista *artista) {
    gint duracao_total_segundos = 0;

    for (int i = 0; artista->id_constituent != NULL && artista->id_constituent[i] != NULL; i++) {
        gchar *id_musica = artista->id_constituent[i];
        Musica *musica = (Musica*) g_hash_table_lookup(musicas, id_musica);

        if (musica != NULL) {
            duracao_total_segundos += duracao_para_segundos(musica->duration);
        }
    }

    return segundos_para_duracao(duracao_total_segundos);
}

int num_musicas_curtidas (Usuario *usuario) {

}
void genero_mais_curtido(int idade_min, int idade_max, GHashTable *gestor_usuarios, GestorMusicas *gestor_musicas) {
    GHashTableIter iter;
    gpointer key, value;
    GHashTable *genero_contagem = g_hash_table_new(g_str_hash, g_str_equal);
    
    // Itera pelos usuários
    g_hash_table_iter_init(&iter, gestor_usuarios);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Usuario *usuario = (Usuario *)value;
        int idade = calcular_idade(usuario->birth_date); 
        
        // Verifica se o usuário está dentro da faixa etária
        if (idade >= idade_min && idade <= idade_max) {
            for (int i = 0; i < usuario->liked_musics_id; i++) {
                int music_id = usuario->liked_musics_id[i];
                Musica *musica = buscar_musicas(gestor_musicas, music_id); // Obter música pelo ID
                
                if (musica != NULL) {
                    const char *genero = musica->genre; // Obtém o género da música
                    
                    // Atualiza a contagem de cada género
                    gpointer count_ptr = g_hash_table_lookup(genero_contagem, genero);
                    int count = (count_ptr != NULL) ? GPOINTER_TO_INT(count_ptr) : 0;
                    g_hash_table_insert(genero_contagem, g_strdup(genero), GINT_TO_POINTER(count + 1));
                }
            }
        }
    }
    
    // Determina o género mais popular
    GHashTableIter contagem_iter;
    gpointer genero, contagem;
    const char *genero_mais_popular = NULL;
    int maior_contagem = 0;
    
    g_hash_table_iter_init(&contagem_iter, genero_contagem);
    while (g_hash_table_iter_next(&contagem_iter, &genero, &contagem)) {
        int count = GPOINTER_TO_INT(contagem);
        if (count > maior_contagem) {
            maior_contagem = count;
            genero_mais_popular = (const char *)genero;
        }
    }
    
    // Imprime o resultado
    if (genero_mais_popular != NULL) {
        printf("O género mais popular é: %s\n", genero_mais_popular);
    } else {
        printf("Nenhum género foi encontrado.\n");
    }
    
    // Liberta a memória da hashtable temporaria criada para recolher os generos
    g_hash_table_destroy(genero_contagem);
}

    