#include <stdio.h>
#include <glib.h>
#include "../../include/gestores/gestor_usuarios.h"

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
    if (validaEmail(usuario) && valida_subscricao(usuario) && validarDataUsuario(usuario))
    {
        g_hash_table_insert(gestor->usuarios, g_strdup(usuario->username), usuario);
        printf("Usuário '%s' adicionado com sucesso!\n", usuario->username);
    }
    else
    {
        printf("Usuário não adicionado devido a informações inválidas.\n");
    }
}


// Busca um usuário no Gestor de Usuarios
Usuario* buscar_usuario (GestorUsuarios *gestor, Usuario *usuario) {
    gchar *id_usuario = user_get_id(usuario); // Obtenha o ID através de user_get_id()
    Usuario *resultado = (Usuario*) g_hash_table_lookup(gestor->usuarios, id_usuario);
    g_free(id_usuario); // Libere a cópia do ID gerada por user_get_id()
    return resultado;
}



