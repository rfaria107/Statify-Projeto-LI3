#include <glib.h>
#include <stdlib.h>
#include "../include/gestores/gestor_sistemas.h"

struct GestorSistema
{
    GestorUsuarios *gestor_usuarios;   // Gestor de usuários
    GestorArtistas *gestor_artistas;   // Gestor de artistas
    GestorMusicas *gestor_musicas;     // Gestor de músicas
    GestorAlbuns *gestor_albuns;       // Gestor de albuns
    GestorHistories *gestor_histories; // Gestor de albuns
};

GestorSistema *criar_gestor_sistema()
{
    GestorSistema *gestor = malloc(sizeof(GestorSistema));
    if (gestor)
    {
        inicializar_gestor_sistema(gestor);
    }
    return gestor;
}

GestorUsuarios *get_gestor_usuarios(GestorSistema *gestor)
{
    return (gestor->gestor_usuarios);
}

GestorArtistas *get_gestor_artistas(GestorSistema *gestor)
{
    return (gestor->gestor_artistas);
}

GestorMusicas *get_gestor_musicas(GestorSistema *gestor)
{
    return (gestor->gestor_musicas);
}

GestorAlbuns *get_gestor_albuns(GestorSistema *gestor)
{
    return (gestor->gestor_albuns);
}

GestorHistories *get_gestor_histories(GestorSistema *gestor)
{
    return (gestor->gestor_histories);
}

void inicializar_gestor_sistema(GestorSistema *gestor)
{
    gestor->gestor_artistas = criar_gestor_artistas();
    gestor->gestor_musicas = criar_gestor_musicas();
    gestor->gestor_usuarios = criar_gestor_usuarios();
    gestor->gestor_albuns = criar_gestor_albuns();
    gestor->gestor_histories = criar_gestor_histories();
}

void liberar_gestor_sistema(GestorSistema *gestor)
{
    liberar_gestor_artistas(gestor->gestor_artistas);
    liberar_gestor_musicas(gestor->gestor_musicas);
    liberar_gestor_usuarios(gestor->gestor_usuarios);
    liberar_gestor_albuns(gestor->gestor_albuns);
    liberar_gestor_histories(gestor->gestor_histories);
    free(gestor);
}
