#include <stdio.h>
#include <glib.h>
#include "../include/entidades/usuario.h"
#include "../include/gestores/gestor_usuarios.h"



int main() {
    // Criação do Gestor de Usuários
    GestorUsuarios *gestor = g_new(GestorUsuarios, 1);
    gestor->usuarios = g_hash_table_new(g_str_hash, g_str_equal);

    // Criação de um usuário exemplo
    Usuario *usuario = g_new(Usuario, 1);
    usuario->username = g_strdup("johndoe");
    usuario->email = g_strdup("johndoe@example.com");
    usuario->first_name = g_strdup("John");
    usuario->last_name = g_strdup("Doe");
    usuario->birth_date = g_strdup("2000/01/01");
    usuario->country = g_strdup("USA");
    usuario->subscription_type = g_strdup("premium");
    usuario->liked_musics_count = 0;
    usuario->liked_musics_id = NULL;

    // Adiciona o usuário ao gestor
    adicionar_usuario(gestor, usuario);

    // Imprime as informações do usuário
    imprimir_usuario(gestor, usuario->username);

    // Libera memória
    g_free(usuario->username);
    g_free(usuario->email);
    g_free(usuario->first_name);
    g_free(usuario->last_name);
    g_free(usuario->birth_date);
    g_free(usuario->country);
    g_free(usuario->subscription_type);
    g_free(usuario);
    g_hash_table_destroy(gestor->usuarios);
    g_free(gestor);

    return 0;
}

