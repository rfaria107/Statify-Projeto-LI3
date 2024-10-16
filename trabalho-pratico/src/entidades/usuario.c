#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <glib.h>
#include "../include/entidades/usuario.h"
#include "../include/gestores/gestor_usuarios.h"

// Função para adicionar um usuário ao "gestor_usuarios"
void adicionar_usuario(GestorUsuarios *gestor, Usuario *usuario) {
    if (validaEmail(usuario) && valida_subscricao(usuario) && validarDataUsuario(usuario)) {  // Valida o email e a data antes de adicionar o usuário
        g_hash_table_insert(gestor->usuarios, g_strdup(usuario->username), usuario);
        printf("Usuário adicionado: %s\n", usuario->username);
    } else {
        printf("Usuário não adicionado devido a informações inválidas.\n");
    }
}

// Função para buscar um usuário pelo nome de usuário
Usuario* buscar_usuario(GestorUsuarios *gestor, const gchar *username) {
    return (Usuario*) g_hash_table_lookup(gestor->usuarios, username);
}

// Função para remover um usuário
void remover_usuario(GestorUsuarios *gestor, const gchar *username) {
    if (g_hash_table_remove(gestor->usuarios, username)) {
        printf("Usuário removido: %s\n", username);
    } else {
        printf("Usuário não encontrado: %s\n", username);
    }
}

// Função para imprimir as informações de um usuário
void imprimir_usuario(const Usuario *usuario) {
    int idade = calcularIdade(usuario);
    printf("Username: %s\nEmail: %s\nNome: %s %s\nIdade: %d\nPaís: %s\nSubscrição: %s\n", 
        usuario->username, usuario->email, usuario->first_name, usuario->last_name, 
        idade, usuario->country, usuario->subscription_type);
}

// Função que valida a subscrição do usuário
bool valida_subscricao(const Usuario *usuario) {
    // Se a subscrição não for normal e não for premium, é inválida.
    if (strcmp(usuario->subscription_type, "normal") != 0 && strcmp(usuario->subscription_type, "premium") != 0) {
        return false;
    }
    return true;
}
