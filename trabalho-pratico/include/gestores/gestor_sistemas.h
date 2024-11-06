#ifndef GESTOR_SISTEMA_H
#define GESTOR_SISTEMA_H

#include <glib.h>

typedef struct GestorSistema GestorSistema;

// Funções para inicialização e limpeza do sistema
GestorSistema* criar_gestor_sistema();
void inicializar_gestor_sistema(GestorSistema *gestor);
void liberar_gestor_sistema(GestorSistema *gestor);

#endif