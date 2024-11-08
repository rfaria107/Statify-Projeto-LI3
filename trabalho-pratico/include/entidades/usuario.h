#ifndef USUARIO_H
#define USUARIO_H

#include <glib.h>

typedef struct Usuario Usuario;

// Funções para manipulação de usuários
Usuario* inicializar_usuario();
Usuario* create_usuario(char* username, char* email, char* first_name, 
                        char* last_name, char* birth_date, 
                        char* country, char* subscription_type, 
                        char** liked_musics_id);
void free_usuario(Usuario* usuario);
//int parse_usuario_and_add_him(RowReader* reader, GestorUsuarios* gestorUser);
int calcularIdade(Usuario *usuario);

// Funções de acesso aos atributos do usuário
gchar* user_get_id(Usuario* user);
gchar* user_get_email(Usuario* user);
gchar* user_get_first_name(Usuario* user);
gchar* user_get_last_name(Usuario* user);
gchar *user_get_birth_date(Usuario *user);
gchar* user_get_country(Usuario* user);
gchar* user_get_subscription_type(Usuario* user);
gchar** user_get_liked_musics_id(Usuario* user);

#endif // USUARIO_H
