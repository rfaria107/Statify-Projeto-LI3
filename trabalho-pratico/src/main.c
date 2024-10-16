#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "../include/gestores/gestor_usuarios.h"
#include "../include/entidades/usuario.h"
#include "../include/gestores/gestor_sistemas.h"

// Função para adicionar um usuário ao gestor de usuários
void adicionar_usuario(GestorUsuarios *gestor) {
    Usuario *usuario = malloc(sizeof(Usuario)); // Alocando memória para um novo usuário
    if (usuario == NULL) {
        printf("Erro ao alocar memória para o usuário.\n");
        return;
    }

    // Coletando informações do usuário
    printf("Digite o nome de usuário: ");
    scanf("%s", usuario->username);
    
    printf("Digite o email: ");
    scanf("%s", usuario->email);
    
    printf("Digite o primeiro nome: ");
    scanf("%s", usuario->first_name);
    
    printf("Digite o último nome: ");
    scanf("%s", usuario->last_name);
    
    printf("Digite o país: ");
    scanf("%s", usuario->country);
    
    printf("Digite o tipo de subscrição (normal/premium): ");
    scanf("%s", usuario->subscription_type);

    // Adicionando o usuário ao gestor
    if (validaEmail(usuario) && valida_subscricao(usuario) && validarDataUsuario(usuario)) {
        g_hash_table_insert(gestor->usuarios, g_strdup(usuario->username), usuario);
        printf("Usuário '%s' adicionado com sucesso!\n", usuario->username);
    } else {
        printf("Usuário não adicionado devido a informações inválidas.\n");
        free(usuario); // Libera a memória se não for adicionado
    }
}

// Função para verificar se um usuário é válido pelo username
void verificar_usuario_por_id(GestorUsuarios *gestor, const gchar *username) {
    Usuario *usuario_valido = buscar_usuario(gestor, username);
    
    if (usuario_valido != NULL) {
        printf("Usuário '%s' é válido e foi encontrado!\n", usuario_valido->username);
    } else {
        printf("Usuário '%s' não é válido ou não foi encontrado.\n", username);
    }
}

int main() {
    GestorSistema gestor;
    inicializar_gestor_sistema(&gestor); // Inicializando o gestor de sistemas

    // Adicionar usuário
    adicionar_usuario(&gestor.gestor_usuarios); // Passando o gestor de usuários

    // Verificar usuário
    char username[50];
    printf("Digite o nome de usuário para verificar: ");
    scanf("%s", username);
    verificar_usuario_por_id(&gestor.gestor_usuarios, username); // Verifica no gestor de usuários

    // Liberar recursos
    liberar_gestor_sistema(&gestor); // Liberando o gestor de sistemas

    return 0;
}
