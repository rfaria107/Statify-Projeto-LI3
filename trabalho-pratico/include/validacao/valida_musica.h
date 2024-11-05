#ifndef MUSICA_H
#define MUSICA_H

#include <glib.h>
#include "../../include/gestores/gestor_sistemas.h"

#define MAX_NAME_LENGTH 50
#define MAX_TITLE_LENGTH 100
#define MAX_GENRE_LENGTH 30
#define MAX_LYRICS_LENGTH 1000

typedef struct {
    gint id;                // ID único da música
    gchar* title;           // Título da música (string dinâmica)
    gchar** artist_ids;     // Lista de IDs dos artistas (array dinâmico de strings)
    gchar* duration;        // Duração da música em segundos
    gchar* genre;           // Gênero (string dinâmica)
    gint year;              // Ano de lançamento
    gchar* lyrics;          // Letra da música (string dinâmica)
} Musica;

// Funções de manipulação de músicas
Musica* buscar_musicas(GestorMusicas *gestor, const gint id); // Corrigido para GestorSistema

// Função de validação
gboolean tudoNum(const char *str);
gboolean validaDuracao(const Musica *musica);
gboolean valida_artist_id(const Musica *musica, GHashTable *artistas);

// Funções para calcular a discografia de cada artista
gint duracao_para_segundos(const gchar* duracao);
gchar* segundos_para_duracao(gint total_segundos);
gchar* calcular_discografia(GHashTable *musicas, const Artista *artista);

#endif // MUSICA_H
