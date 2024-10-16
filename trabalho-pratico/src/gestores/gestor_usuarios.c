#include <stdio.h>
#include <glib.h>
#include "../include/gestores/gestor_usuarios.h"
#include "../include/entidades/usuario.h"

void inicializar_gestor_usuarios(GestorUsuarios *gestor) {
    gestor->usuarios = g_hash_table_new(g_str_hash, g_str_equal);
}

void liberar_gestor_usuarios(GestorUsuarios *gestor) {
    g_hash_table_destroy(gestor->usuarios);
}

void adicionar_usuario(GestorUsuarios *gestor, Usuario *usuario) {
    if (validaEmail(usuario) && valida_subscricao(usuario) && validarDataUsuario(usuario)) {
        g_hash_table_insert(gestor->usuarios, g_strdup(usuario->username), usuario);
        printf("Usuário '%s' adicionado com sucesso!\n", usuario->username);
    } else {
        printf("Usuário não adicionado devido a informações inválidas.\n");
    }
}

Usuario* buscar_usuario(GestorUsuarios *gestor, const gchar *username) {
    return (Usuario*) g_hash_table_lookup(gestor->usuarios, username);
}
