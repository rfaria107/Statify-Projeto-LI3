#include <stdio.h>
#include <glib.h>
#include "../../include/gestores/gestor_usuarios.h"
#include "../../include/entidades/usuario.h"

struct GestorUsuarios
{
    GHashTable *usuarios; // Tabela hash para armazenar usuários
};

void free_usuario_value(gpointer value)
{
    free_usuario((Usuario *)value);
}

// Função para obter a hash table de usuários
GHashTable *get_hash_usuarios(GestorUsuarios *gestor)
{
    return gestor->usuarios;
}

// Função para criar o gestor de usuários
GestorUsuarios *criar_gestor_usuarios()
{
    GestorUsuarios *gestor = malloc(sizeof(GestorUsuarios));
    if (gestor)
    {
        inicializar_gestor_usuarios(gestor); // Inicializa a hash table
    }
    return gestor;
}

// Inicializa o Gestor de Usuarios
void inicializar_gestor_usuarios(GestorUsuarios *gestor)
{
    gestor->usuarios = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_usuario_value);
}

// Libera os recursos do Gestor de Usuarios
void liberar_gestor_usuarios(GestorUsuarios *gestor)
{
    g_hash_table_destroy(gestor->usuarios); // Libera a hash table
    free(gestor);                           // Libera o próprio gestor
}

// Busca um usuário no Gestor de Usuarios
Usuario *buscar_usuario(GestorUsuarios *gestor, Usuario *usuario)
{
    if (gestor && gestor->usuarios && usuario)
    {
        gchar *id_usuario = user_get_id(usuario); // Obtenha o ID através de user_get_id()
        if (id_usuario)
        {
            Usuario *resultado = (Usuario *)g_hash_table_lookup(gestor->usuarios, id_usuario);
            g_free(id_usuario); // Libere a cópia do ID gerada por user_get_id()
            return resultado;
        }
        else
        {
            fprintf(stderr, "Erro ao obter ID do usuário.\n");
        }
    }
    return NULL;
}

Usuario *buscar_usuario_id(GestorUsuarios *gestor, gchar *id_usuario)
{
    if (gestor)
    {
        Usuario *usuario = (Usuario *)g_hash_table_lookup(gestor->usuarios, id_usuario);
        return usuario;
    }
    return NULL;
}

// Insere um usuário na hash table do gestor
void inserir_usuario(GestorUsuarios *gestor, Usuario *user)
{
    if (gestor && gestor->usuarios && user)
    {
        gchar *id = user_get_id(user);
        if (id)
        {
            g_hash_table_insert(gestor->usuarios, g_strdup(id), user);
            g_free(id); // Libere o ID temporário após duplicação
        }
    }
}