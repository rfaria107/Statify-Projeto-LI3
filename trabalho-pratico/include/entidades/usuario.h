#ifndef USUARIO_H
#define USUARIO_H

#include <glib.h>
#include "../include/parsing/rowreader.h"

typedef struct {
    char* username;
    char* email;
    char* first_name;
    char* last_name;
    char* birth_date;
    char* country;
    char* subscription_type;
    char** liked_musics_id; // Array de IDs de músicas curtidas
} Usuario;

// Funções para manipulação de usuários
Usuario* inicializar_usuario();
Usuario* create_usuario(char* username, char* email, char* first_name, 
                        char* last_name, char* birth_date, 
                        char* country, char* subscription_type, 
                        char** liked_musics_id);
void free_usuario(Usuario* usuario);
int parse_usuario_and_add_him(RowReader* reader, GestorUsuarios* gestorUser);

// Funções de acesso aos atributos do usuário
char* user_get_id(Usuario* user);
char* user_get_email(Usuario* user);
char* user_get_first_name(Usuario* user);
char* user_get_last_name(Usuario* user);
char* user_get_country(Usuario* user);
char* user_get_subscription_type(Usuario* user);
char** user_get_liked_musics_id(Usuario* user);

#endif // USUARIO_H
