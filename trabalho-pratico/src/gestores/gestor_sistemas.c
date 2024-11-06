#include <glib.h>
#include <stdlib.h>
#include "../include/gestores/gestor_sistemas.h"
#include "../include/gestores/gestor_musicas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/gestores/gestor_artistas.h"

struct GestorSistema
{
    GestorUsuarios *gestor_usuarios; // Gestor de usuários
    GestorArtistas *gestor_artistas; // Gestor de artistas
    GestorMusicas *gestor_musicas;   // Gestor de músicas
};

GestorSistema* criar_gestor_sistema() {
    GestorSistema *gestor = malloc(sizeof(GestorSistema));
    if (gestor) {
        inicializar_gestor_sistema(gestor);
    }
    return gestor;
}

void inicializar_gestor_sistema(GestorSistema *gestor)
{
    inicializar_gestor_usuarios(gestor->gestor_usuarios);
    inicializar_gestor_artistas(gestor->gestor_artistas);
    inicializar_gestor_musicas(gestor->gestor_musicas);
}

void liberar_gestor_sistema(GestorSistema *gestor)
{
    liberar_gestor_usuarios(gestor->gestor_usuarios);
    liberar_gestor_artistas(gestor->gestor_artistas);
    liberar_gestor_musicas(gestor->gestor_musicas);
    free(gestor);
}
