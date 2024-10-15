#include "../include/gestor_sistemas.h"

// Definição dos gestores individuais
typedef struct {
    GHashTable *usuarios;
} GestorUsuarios;

typedef struct {
    GHashTable *artistas;
} GestorArtistas;

typedef struct {
    GHashTable *musicas;
} GestorMusicas;

// Estrutura principal que gerencia todos os gestores
typedef struct {
    GestorUsuarios *gestor_usuarios;
    GestorArtistas *gestor_artistas;
    GestorMusicas *gestor_musicas;
} GestorSistemas;
