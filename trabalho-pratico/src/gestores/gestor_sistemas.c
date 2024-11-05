#include "../../include/gestores/gestor_sistemas.h"

typedef struct GestorUsuarios
{
    GHashTable *usuarios; // Tabela hash para armazenar usuários
} GestorUsuarios;

typedef struct GestorArtistas
{
    GHashTable *artistas; // Tabela hash de artistas
} GestorArtistas;

typedef struct GestorMusicas
{
    GHashTable *musicas; // Tabela hash de músicas
} GestorMusicas;

typedef struct GestorSistema
{
    GestorUsuarios gestor_usuarios; // Gestor de usuários
    GestorArtistas gestor_artistas; // Gestor de artistas
    GestorMusicas gestor_musicas;   // Gestor de músicas
} GestorSistema;

void inicializar_gestor_sistema(GestorSistema *gestor)
{
    inicializar_gestor_usuarios(&gestor->gestor_usuarios);
    inicializar_gestor_artistas(&gestor->gestor_artistas);
    inicializar_gestor_musicas(&gestor->gestor_musicas);
}

void liberar_gestor_sistema(GestorSistema *gestor)
{
    liberar_gestor_usuarios(&gestor->gestor_usuarios);
    liberar_gestor_artistas(&gestor->gestor_artistas);
    liberar_gestor_musicas(&gestor->gestor_musicas);
}
