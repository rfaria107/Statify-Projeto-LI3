#ifndef GESTOR_SISTEMA_H
#define GESTOR_SISTEMA_H

#include <glib.h>

typedef struct {
    GHashTable *usuarios;
    GHashTable *artistas;
    GHashTable *musicas;
} GestorSistema;

// Funções para inicialização e limpeza do sistema
void inicializar_gestor(GestorSistema *gestor);
void liberar_gestor(GestorSistema *gestor);

#endif
