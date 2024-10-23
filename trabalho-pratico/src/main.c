#include <stdio.h>
#include <glib.h>
#include "../include/gestores/gestor_usuarios.h"
#include "../include/entidades/usuario.h"

// Funções fictícias para validar usuário (podes substituir pelas tuas)
gboolean validaEmail(Usuario *usuario)
{
    return g_str_has_suffix(usuario->email, "@example.com"); // Validação simples para teste
}

gboolean valida_subscricao(Usuario *usuario)
{
    return g_strcmp0(usuario->subscription_type, "Premium") == 0; // Exemplo: validar se o tipo de subscrição é 'Premium'
}

gboolean validarDataUsuario(Usuario *usuario)
{
    return g_strcmp0(usuario->birth_date, "2006/01/01") < 0; // Exemplo: a data de nascimento deve ser anterior a 2006
}

int main(int argc, char *argv[])
{
    // Inicializa o Gestor de Usuarios
    GestorUsuarios gestor;
    inicializar_gestor_usuarios(&gestor);

    FILE *artistas = fopen(strcat(argv[2], "/com_erros/artists"), "r");
    // dar parse aos artistas
    parserprincipal(artistas, 'a');
    fclose(artistas);
    FILE *users = fopen(strcat(argv[2], "/com_erros/users"), "r");
    // dar parse aos users
    parserprincipal(users, 'u');
    fclose(users);
    FILE *musicas = fopen(strcat(argv[2], "/com_erros/musics"), "r");
    // dar parse às musicas
    parserprincipal(musicas, 'm');
    fclose(musicas);

    // Cria uma lista de músicas curtidas para testar
    gchar *musicas_alice[] = {"song1", "song2", NULL}; // Lista terminada com NULL
    gchar *musicas_bob[] = {"song3", NULL};

    // Cria alguns usuários para testar
    Usuario usuario1 = {
        .username = "Alice",
        .email = "alice@example.com",
        .first_name = "Alice",
        .last_name = "Smith",
        .birth_date = "1998/03/12",
        .country = "Portugal",
        .subscription_type = "Premium",
        .liked_musics_id = musicas_alice,
        .liked_musics_count = 2};

    Usuario usuario2 = {
        .username = "Bob",
        .email = "bob@example.com",
        .first_name = "Bob",
        .last_name = "Johnson",
        .birth_date = "1990/07/25",
        .country = "Portugal",
        .subscription_type = "Premium",
        .liked_musics_id = musicas_bob,
        .liked_musics_count = 1};

    // Adiciona usuários
    adicionar_usuario(&gestor, &usuario1);
    adicionar_usuario(&gestor, &usuario2);

    // Testa buscar e imprimir usuários
    printf("\nBuscando e imprimindo informações de 'Alice':\n");
    imprimir_usuario(&gestor, "Alice");

    printf("\nBuscando e imprimindo informações de 'Bob':\n");
    imprimir_usuario(&gestor, "Bob");

    // Libera os recursos do Gestor de Usuarios
    liberar_gestor_usuarios(&gestor);

    return 0;
}
