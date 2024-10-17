#include <stdio.h>
#include <glib.h>
#include "../include/gestores/gestor_usuarios.h"
#include "../include/entidades/usuario.h"

// Inicializa o Gestor de Usuarios
void inicializar_gestor_usuarios(GestorUsuarios *gestor) {
    gestor->usuarios = g_hash_table_new(g_str_hash, g_str_equal);
}

// Liberta os recursos do Gestor de Usuarios
void liberar_gestor_usuarios(GestorUsuarios *gestor) {
    g_hash_table_destroy(gestor->usuarios);
}

// Adiciona um usuário ao Gestor de Usuarios se for válido
void adicionar_usuario(GestorUsuarios *gestor, Usuario *usuario) {
    if (validaEmail(usuario) && valida_subscricao(usuario) && validarDataUsuario(usuario)) {
        g_hash_table_insert(gestor->usuarios, g_strdup(usuario->username), usuario);
        printf("Usuário '%s' adicionado com sucesso!\n", usuario->username);
    } else {
        printf("Usuário não adicionado devido a informações inválidas.\n");
    }
}

// Busca um usuário no Gestor de Usuarios
Usuario* buscar_usuario(GestorUsuarios *gestor, const gchar *username) {
    return (Usuario*) g_hash_table_lookup(gestor->usuarios, username);
}

// Imprime informações do usuário, se existir
void imprimir_usuario(GestorUsuarios *gestor, const gchar *username) {
    Usuario* usuario = buscar_usuario(gestor, username);
    int idade = calcularIdade(usuario);

    if (usuario != NULL) {
        printf("Email: %s\n", usuario->email);
        printf("First: %s\n", usuario->first_name);
        printf("Last: %s\n", usuario->last_name);
        printf("Idade: %d\n", idade);
        printf ("País: %s\n", usuario->country);
    } else {
        printf("Usuário '%s' não encontrado.\n", username);
    }
}
