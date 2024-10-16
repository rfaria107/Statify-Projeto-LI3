#include "../include/gestores/gestor_sistemas.h"

void inicializar_gestor_sistema(GestorSistema *gestor) {
    inicializar_gestor_usuarios(&gestor->gestor_usuarios);
    inicializar_gestor_artistas(&gestor->gestor_artistas);
    inicializar_gestor_musicas(&gestor->gestor_musicas);
}

void liberar_gestor_sistema(GestorSistema *gestor) {
    liberar_gestor_usuarios(&gestor->gestor_usuarios);
    liberar_gestor_artistas(&gestor->gestor_artistas);
    liberar_gestor_musicas(&gestor->gestor_musicas);
}
