#ifndef USUARIO_H
#define USUARIO_H

#include <glib.h>
#include <ctype.h>  // Para usar isdigit
#include <string.h> // Para usar strlen
#include "../include/gestor_sistemas.h"

typedef struct {
    char username[50];
    char email[100];
    char first_name[50];
    char last_name[50];
    char birth_date[11]; // Formato: YYYY/MM/DD
    char country[50];
    char subscription_type[10];
    char liked_musics_id [100];
} Usuario;

// Funções de manipulação de usuários
void adicionar_usuario(GestorSistema *gestor, Usuario *usuario);
Usuario* buscar_usuario(GestorSistema *gestor, const char *username);
void remover_usuario(GestorSistema *gestor, const char *username);
void imprimir_usuario(Usuario *usuario);

// Funções de validação de datas
int validarFormatoData(const char *data);
int validarMesEDia(int mes, int dia);
int validarDataFutura(int ano, int mes, int dia);
int validarDataUsuario(const Usuario *usuario);
int calcularIdade(const Usuario *usuario);

// Função de validação 
bool validaEmail(const Usuario *usuario);

// Valida email
bool valida_subscricao (Usuario *usuario);

#endif
