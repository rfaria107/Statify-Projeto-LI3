#include <stdio.h>
#include "../Include/usuario.h"
#include "../Include/gestor_sistemas.h"

// Função para adicionar um usuário ao "gestor_sistemas"
void adicionar_usuario(GestorSistema *gestor, Usuario *usuario) {
    g_hash_table_insert(gestor->usuarios, g_strdup(usuario->username), usuario);
}

// Função para buscar um usuário pelo id de usuário
Usuario* buscar_usuario(GestorSistema *gestor, const char *username) {
    return (Usuario*) g_hash_table_lookup(gestor->usuarios, username);
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
    int idade= calcularIdade (usuario);
    printf("Username: %s\nEmail: %s\nNome: %s %s\nIdade: %d\nPaís: %s\nSubscrição: %s\n", 
        usuario->username, usuario->email, usuario->first_name, usuario->last_name, 
        idade, usuario->country, usuario->subscription_type);
}

