#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "../../include/entidades/usuario.h"
#include "../../include/entidades/musica.h"
#include "../include/parsing/rowreader.h"


Usuario* create_usuario(char* username, char* email, char* first_name, 
                        char* last_name, char* birth_date, 
                        char* country, char* subscription_type, 
                        char** liked_musics_id) {
    Usuario* usuario = inicializar_usuario (); // Aloca memória para o usuário

    // Define os atributos do usuário
    usuario->username = g_strdup(username);
    usuario->email = g_strdup(email);
    usuario->first_name = g_strdup(first_name);
    usuario->last_name = g_strdup(last_name);
    usuario->birth_date = g_strdup(birth_date);
    usuario->country = g_strdup(country);
    usuario->subscription_type = g_strdup(subscription_type);

    // Inicializa a lista de músicas curtidas
    if (liked_musics_id) {
        int count = 0;
        while (liked_musics_id[count] != NULL) {
            count++;
        }

        usuario->liked_musics_id = g_malloc((count + 1) * sizeof(char*));
        for (int i = 0; i < count; i++) {
            usuario->liked_musics_id[i] = g_strdup(liked_musics_id[i]);
        }
        usuario->liked_musics_id[count] = NULL; // Termina a lista
    } else {
        usuario->liked_musics_id = NULL; // Se não houver músicas curtidas
    }

    return usuario; // Retorna o usuário criado
}


Usuario* inicializar_usuario() {
    Usuario* usuario = (Usuario*)malloc(sizeof(Usuario)); // Aloca memória para o usuário
    if (!usuario) {
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
void free_usuario(Usuario* usuario) {
    if (usuario) {
        g_free(usuario->username);
        g_free(usuario->email);
        g_free(usuario->first_name);
        g_free(usuario->last_name);
        g_free(usuario->birth_date);
        g_free(usuario->country);
        g_free(usuario->subscription_type);

        // Libera os IDs das músicas curtidas, se existirem
        if (usuario->liked_musics_id) {
            for (int i = 0; usuario->liked_musics_id[i] != NULL; i++) {
                g_free(usuario->liked_musics_id[i]); // Libera cada ID
            }
            g_free(usuario->liked_musics_id); // Libera o array de IDs
        }

        g_free(usuario); // Libera a estrutura do usuário em si
    }
}


int parse_usuario_and_add_him(RowReader* reader, GestorUsuarios* gestorUser) {

    char* username = reader_next_cell(reader);
    if (is_empty_value(username)) return 1;

    char* email = reader_next_cell(reader);
    if (invalid_email(email)) return 1;

    char* first_name = reader_next_cell(reader);
    if (is_empty_value(first_name)) return 1;

    char* last_name = reader_next_cell(reader);
    if (is_empty_value(last_name)) return 1;

    char* birth_date_str = reader_next_cell(reader);
    if (invalid_date(birth_date_str)) return 1;

    char* country = reader_next_cell(reader);
    if (is_empty_value(country)) return 1;

    char* subscription_type = reader_next_cell(reader);
    if (is_empty_value(subscription_type)) return 1;

    // Parsing liked music IDs
    char** liked_musics_id = parse_liked_musics(reader);

    // Cria o usuário com os dados parseados
    Usuario* usuario = create_usuario(username, email, first_name, last_name,
                                      birth_date_str, country, subscription_type,
                                      liked_musics_id);

    if (!usuario) return 1;

    // Insere o usuário no catálogo
    adicionar_usuario(gestorUser, usuario);

    free_usuario(usuario);

    return 0;
}

char* user_get_id(Usuario* user) { return g_strdup(user->username); }

char* user_get_email (Usuario *user) {return g_strdup (user->email);}

char* user_get_first_name( Usuario* user) {return g_strdup (user->first_name);}

char* user_get_last_name (Usuario *user) {return g_strdup (user->last_name);}

char* user_get_country (Usuario* user) {return g_strdup (user->country);}

char* user_get_subscription_type (Usuario* user) {return g_strdup (user->subscription_type);}

char** user_get_liked_musics_id(Usuario* user) {
    if (!user->liked_musics_id) return NULL;

    // Contar o número de IDs de músicas curtidas
    int count = 0;
    while (user->liked_musics_id[count] != NULL) {
        count++;
    }

    char** liked_musics_copy = malloc((count + 1) * sizeof(char*));
    
    // Copiar cada ID de música
    for (int i = 0; i < count; i++) {
        liked_musics_copy[i] = g_strdup(user->liked_musics_id[i]);
    }
    liked_musics_copy[count] = NULL;  

    return liked_musics_copy;
}
