#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "../../include/entidades/usuario.h"
#include "../../include/entidades/musica.h"
#include "../../include/parsing/rowreader.h"
#include "../../include/gestores/gestor_usuarios.h"

struct Usuario
{
    gchar *username;
    gchar *email;
    gchar *first_name;
    gchar *last_name;
    gchar *birth_date;
    gchar *country;
    gchar *subscription_type;
    gchar **liked_musics_id; // Array de IDs de músicas curtidas
};

// Função para criar um usuário
Usuario* create_usuario(char* username, char* email, char* first_name, char* last_name, char* birth_date, 
                        char* country, char* subscription_type, char** liked_musics) {
    Usuario* usuario = (Usuario*)malloc(sizeof(Usuario));
    if (!usuario) {
        fprintf(stderr, "Erro ao alocar memória para Usuario.\n");
        return NULL;
    }

    usuario->username = strdup(username);
    usuario->email = strdup(email);
    usuario->first_name = strdup(first_name);
    usuario->last_name = strdup(last_name);
    usuario->birth_date = strdup(birth_date);
    usuario->country = strdup(country);
    usuario->subscription_type = strdup(subscription_type);

    // Alocar memória para liked_musics
    int num_musicas = 0;
    while (liked_musics[num_musicas] != NULL) {
        num_musicas++;
    }
    
    usuario->liked_musics_id = (char**)malloc((num_musicas + 1) * sizeof(char*));
    if (!usuario->liked_musics_id) {
        fprintf(stderr, "Erro ao alocar memória para liked_musics.\n");
        free(usuario); // Liberar memória do usuário caso falhe
        return NULL;
    }

    for (int i = 0; i < num_musicas; i++) {
        usuario->liked_musics_id[i] = strdup(liked_musics[i]);
    }
    usuario->liked_musics_id[num_musicas] = NULL; // Terminar com NULL

    return usuario;
}
Usuario *inicializar_usuario()
{
    Usuario *usuario = malloc(sizeof(Usuario)); // Aloca memória para o usuário
    if (!usuario)
    {
        return NULL; // Retorna NULL se a alocação falhar
    }

    // Inicializa os campos do usuário
    usuario->username = NULL;
    usuario->email = NULL;
    usuario->first_name = NULL;
    usuario->last_name = NULL;
    usuario->birth_date = NULL;
    usuario->country = NULL;
    usuario->subscription_type = NULL;
    usuario->liked_musics_id = NULL;

    return usuario; // Retorna o ponteiro para o usuário inicializado
}

void free_usuario(Usuario *usuario)
{
    if (usuario)
    {
        g_free(usuario->username);
        g_free(usuario->email);
        g_free(usuario->first_name);
        g_free(usuario->last_name);
        g_free(usuario->birth_date);
        g_free(usuario->country);
        g_free(usuario->subscription_type);

        // Libera os IDs das músicas curtidas, se existirem
        if (usuario->liked_musics_id)
        {
            for (int i = 0; usuario->liked_musics_id[i] != NULL; i++)
            {
                g_free(usuario->liked_musics_id[i]); // Libera cada ID
            }
            g_free(usuario->liked_musics_id); // Libera o array de IDs
        }

        g_free(usuario); // Libera a estrutura do usuário em si
    }
}
/*
int parse_usuario_and_add_him(RowReader *reader, GestorUsuarios *gestorUser)
{

    char *username = reader_next_cell(reader);


    char *email = reader_next_cell(reader);

    char *first_name = reader_next_cell(reader);

    char *last_name = reader_next_cell(reader);


    char *birth_date_str = reader_next_cell(reader);

    char *country = reader_next_cell(reader);


    char *subscription_type = reader_next_cell(reader);


    // Parsing liked music IDs
    char **liked_musics_id = parse_liked_musics(reader);

    // Cria o usuário com os dados parseados
    Usuario *usuario = create_usuario(username, email, first_name, last_name,
                                      birth_date_str, country, subscription_type,
                                      liked_musics_id);

    if (!usuario)
        return 1;

    // Insere o usuário no catálogo
    adicionar_usuario(gestorUser, usuario);

    free_usuario(usuario);

    return 0;
}
*/

gchar *user_get_id(Usuario *user) { return g_strdup(user->username); }

gchar *user_get_email(Usuario *user) { return g_strdup(user->email); }

gchar *user_get_first_name(Usuario *user) { return g_strdup(user->first_name); }

gchar *user_get_last_name(Usuario *user) { return g_strdup(user->last_name); }

gchar *user_get_birth_date(Usuario *user) { return g_strdup(user->birth_date); }

gchar *user_get_country(Usuario *user) { return g_strdup(user->country); }

gchar *user_get_subscription_type(Usuario *user) { return g_strdup(user->subscription_type); }

gchar **user_get_liked_musics_id(Usuario *user)
{
    if (!user->liked_musics_id)
        return NULL;

    // Contar o número de IDs de músicas curtidas
    int count = 0;
    while (user->liked_musics_id[count] != NULL)
    {
        count++;
    }

    gchar **liked_musics_copy = malloc((count + 1) * sizeof(gchar *));

    // Copiar cada ID de música
    for (int i = 0; i < count; i++)
    {
        liked_musics_copy[i] = g_strdup(user->liked_musics_id[i]);
    }
    liked_musics_copy[count] = NULL;

    return liked_musics_copy;
}

// Calcula a idade do usuário com base na data de nascimento
int calcularIdade(Usuario *usuario)
{
    int ano, mes, dia;
    gchar *birth_date = user_get_birth_date(usuario);
    sscanf(birth_date, "%4d/%2d/%2d", &ano, &mes, &dia);
    const int anoAtual = 2024, mesAtual = 9, diaAtual = 9;
    int idade = anoAtual - ano;
    if (mes > mesAtual || (mes == mesAtual && dia > diaAtual))
        idade--;
    return idade;
}