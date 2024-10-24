#ifndef ARTISTA_H
#define ARTISTA_H

#define MAX_NAME_LENGTH 50
#define MAX_DESCRIPTION_LENGTH 200

#include <glib.h>

typedef struct artista{
    gchar* id;                    // Identificador único do artista (inteiro)
    gchar* name;                // Nome do artista (string dinâmica)
    gchar* description;         // Descrição do artista (string dinâmica)
    gdouble recipe_per_stream;  // Receita por stream (valor em double)
    gchar** id_constituent;     // Lista de IDs de membros (para grupos musicais), array dinâmico de strings
    gchar* country;             // País de origem (string dinâmica)
    gchar* type;                // Tipo de artista: "individual" ou "grupo musical" (string dinâmica)
} Artista;

// Função que valida se um artista individual nao tem elementos.  individual/group
gboolean valida_artista_individual(const Artista *artista);

#endif // ARTISTA_H
