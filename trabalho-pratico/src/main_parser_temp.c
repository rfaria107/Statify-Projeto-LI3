#include <stdio.h>
#include <glib.h>
#include "../include/entidades/usuario.h"
#include "../include/gestores/gestor_usuarios.h"


//apenas para não perder a minha main no merge -Rui
int main(int argc, char *argv[])
{
    // Inicializa o Gestor de Usuarios
    GestorUsuarios gestor;
    inicializar_gestor_usuarios(&gestor);

    FILE *artistas = fopen(strcat(argv[2], "/com_erros/artists"), "r");
    // dar parse aos artistas
    parser_principal(artistas, 'a');
    fclose(artistas);
    FILE *users = fopen(strcat(argv[2], "/com_erros/users"), "r");
    // dar parse aos users
    parser_principal(users, 'u');
    fclose(users);
    FILE *musicas = fopen(strcat(argv[2], "/com_erros/musics"), "r");
    // dar parse às musicas
    parser_principal(musicas, 'm');
    fclose(musicas);

    // Cria uma lista de músicas curtidas para testar
    gchar *musicas_alice[] = {"song1", "song2", NULL}; // Lista terminada com NULL
    gchar *musicas_bob[] = {"song3", NULL};

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
