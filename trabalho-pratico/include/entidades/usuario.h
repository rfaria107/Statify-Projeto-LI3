#ifndef USUARIO_H
#define USUARIO_H

#include <glib.h>
#include <ctype.h>  
#include <string.h> 

typedef struct {
    gchar* username;           // Nome de usuário (string dinâmica)
    gchar* email;              // Email (string dinâmica)
    gchar* first_name;         // Primeiro nome (string dinâmica)
    gchar* last_name;          // Sobrenome (string dinâmica)
    gchar* birth_date;         // Data de nascimento (string dinâmica no formato YYYY/MM/DD)
    gchar* country;            // País (string dinâmica)
    gchar* subscription_type;  // Tipo de assinatura (string dinâmica)
    gchar** liked_musics_id;   // Lista de IDs de músicas curtidas (array dinâmico de strings)
    gint liked_musics_count;   // Número de músicas curtidas
} Usuario;

// Funções de manipulação de usuários
void adicionar_usuario(GestorUsuarios *gestor, Usuario *usuario);
Usuario* buscar_usuario(GestorUsuarios *gestor, const gchar *username);
void remover_usuario(GestorUsuarios *gestor, const gchar *username);
void imprimir_usuario(const Usuario *usuario);

// Funções de validação de datas
int validarFormatoData(const gchar *data);
int validarMesEDia(int mes, int dia);
int validarDataFutura(int ano, int mes, int dia);
int validarDataUsuario(const Usuario *usuario);
int calcularIdade(const Usuario *usuario);

// Função de validação 
bool validaEmail(const Usuario *usuario);

// Valida subscrição
bool valida_subscricao(const Usuario *usuario);

#endif
