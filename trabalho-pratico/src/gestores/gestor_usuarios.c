#include <stdio.h>
#include <glib.h>
#include "../../include/gestores/gestor_usuarios.h"
#include "../../include/entidades/usuario.h"
#include "../../include/validacao/valida_user.h"

struct Usuario {
    char* username;
    char* email;
    char* first_name;
    char* last_name;
    char* birth_date;
    char* country;
    char* subscription_type;
    char** liked_musics_id; // Array de IDs de músicas curtidas
};

struct GestorUsuarios {
    GHashTable *usuarios; // Tabela hash para armazenar usuários
};

GHashTable *get_hash_usuarios(GestorUsuarios *gestor){
    return (gestor->usuarios);
}

GestorUsuarios* criar_gestor_usuarios() {
    GestorUsuarios *gestor = malloc(sizeof(GestorUsuarios));
    if (gestor) {
        inicializar_gestor_usuarios(gestor);
    }
    return gestor;
}

// Inicializa o Gestor de Usuarios
void inicializar_gestor_usuarios(GestorUsuarios *gestor)
{
    gestor->usuarios = g_hash_table_new(g_str_hash, g_str_equal);
}

// Liberta os recursos do Gestor de Usuarios
void liberar_gestor_usuarios(GestorUsuarios *gestor)
{
    g_hash_table_destroy(gestor->usuarios);
}

// Adiciona um usuário ao Gestor de Usuarios se for válido
void adicionar_usuario(GestorUsuarios *gestor, Usuario *usuario)
{
        g_hash_table_insert(gestor->usuarios, g_strdup_inline  (usuario->username), usuario);
}

// Busca um usuário no Gestor de Usuarios
Usuario* buscar_usuario (GestorUsuarios *gestor, Usuario *usuario) {
    gchar *id_usuario = user_get_id(usuario); // Obtenha o ID através de user_get_id()
    Usuario *resultado = (Usuario*) g_hash_table_lookup(gestor->usuarios, id_usuario);
    g_free(id_usuario); // Libere a cópia do ID gerada por user_get_id()
    return resultado;
}

void inserir_usuario(GestorUsuarios *gestor, Usuario *user)
{
    g_hash_table_insert(gestor->usuarios, g_strdup(get_username(user)), user);
}



