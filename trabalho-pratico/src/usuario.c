#include <stdio.h>
#include <stdbool.h>
#include "../include/usuario.h"
#include "../include/gestor_sistemas.h"

// Função para adicionar um usuário ao "gestor_sistemas"
void adicionar_usuario(GestorSistema *gestor, Usuario *usuario) {
    if (validaEmail(usuario)) {  // Valida o email antes de adicionar o usuário
        g_hash_table_insert(gestor->usuarios, g_strdup(usuario->username), usuario);
    } else {
        printf("Email inválido. Usuário não adicionado.\n");
    }
}
// Função para buscar um usuário pelo id de usuário
Usuario* buscar_usuario(GestorSistema *gestor, const char *username) {
    if (validaEmail){ 
    return (Usuario*) g_hash_table_lookup(gestor->usuarios, username);}
}

// Função para remover um usuário
void remover_usuario(GestorSistema *gestor, const char *username) {
    g_hash_table_remove(gestor->usuarios, username);
}

// Valida se a data está no formato correto "YYYY/MM/DD"
int validarFormatoData(const char *data) {
    if (strlen(data) != 10 || data[4] != '/' || data[7] != '/') {
        return 0;  // Formato inválido
    }

    // Verifica se todos os outros caracteres são dígitos (numeros inteiros)
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!isdigit(data[i])) return 0;
    }

    return 1;
}

// Verifica se o mês e o dia são válidos
int validarMesEDia(int mes, int dia) {
    return (mes >= 1 && mes <= 12 && dia >= 1 && dia <= 31);
}

// Verifica se a data fornecida está no futuro em relação à data atual 
int validarDataFutura(int ano, int mes, int dia) {
    const int anoAtual = 2024, mesAtual = 9, diaAtual = 9; // data atual

    if (ano > anoAtual || (ano == anoAtual && (mes > mesAtual || (mes == mesAtual && dia > diaAtual)))) {
        return 0;  // Data no futuro
    }

    return 1;
}

// Valida se a data de nascimento do usuario é válida
int validarDataUsuario(const Usuario *usuario) {
    const char *birth_date = usuario->birth_date;

    if (!validarFormatoData(birth_date)) {
        printf("Formato inválido para a data de nascimento!\n");
        return 0;
    }

    // Extrai ano, mês e dia da data de nascimento do usuario
    int ano, mes, dia;
    sscanf(birth_date, "%4d/%2d/%2d", &ano, &mes, &dia);

    if (!validarMesEDia(mes, dia) || !validarDataFutura(ano, mes, dia)) {
        printf("Data de nascimento inválida!\n");
        return 0;
    }

    return 1;  
}

// Calcula a idade do usuário com base na data de nascimento
int calcularIdade(const Usuario *usuario) {
    if (!validarDataUsuario(usuario)) {
        return -1;  // Data Inválida
    }

    int ano, mes, dia;
    sscanf(usuario->birth_date, "%4d/%2d/%2d", &ano, &mes, &dia);

    const int anoAtual = 2024, mesAtual = 9, diaAtual = 9;
    int idade = anoAtual - ano;

    // Ajusta a idade se o aniversário ainda não foi completado (no ano atual)
    if (mes > mesAtual || (mes == mesAtual && dia > diaAtual)) {
        idade--;
    }

    return idade;
}
// Função para imprimir as informações de um usuário
void imprimir_usuario(Usuario *usuario) {
    int idade = calcularIdade (usuario);
    printf("Username: %s\nEmail: %s\nNome: %s %s\nIdade: %d\nPaís: %s\nSubscrição: %s\n", 
        usuario->username, usuario->email, usuario->first_name, usuario->last_name, 
        idade, usuario->country, usuario->subscription_type);
}

// Função que valida o email 

bool validaEmail(const Usuario *usuario) {
    const char *email = usuario->email;

    int arroba_count = 0;
    int ponto_count = 0;
    int a = -1;
    int p = -1;
    bool antes_arroba = true;

    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') {
            arroba_count++;
            antes_arroba = false;  // Muda para após o '@'
            a = i;  
        } else if (email[i] == '.') {
            ponto_count++;
            p = i; 
        } else if (antes_arroba) {

            if (!islower(email[i]) && !isdigit(email[i])) {
                return false;
            }
        } else {

            if (!islower(email[i])) {
                return false;
            }
        }
    }

    // Verificações finais:
    // 1. O '@' deve existir e não pode ser o primeiro ou último caractere.
    // 2. O '.' deve existir após o '@' e não pode ser imediatamente após '@'.
    // 3. O comprimento entre o último '.' e o final da string deve ser 2 ou 3.
    if (arroba_count != 1 || ponto_count != 1) return false;
    if (a == 0 || p == a + 1 || p == -1 || (p - a <= 1)) return false;
    int tamanho_apos_ponto = 0;
    for (int i = p + 1; email[i] != '\0'; i++) {
        tamanho_apos_ponto++;
    }
    if (tamanho_apos_ponto < 2 || tamanho_apos_ponto > 3) return false;

    return true;
}
