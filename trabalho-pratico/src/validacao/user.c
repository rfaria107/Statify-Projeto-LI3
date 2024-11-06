#include <stdio.h>
#include <string.h>
#include <glib.h>
#include "../../src/entidades/usuario.c"
#include "../../include/entidades/musica.h"


// Valida se a data está no formato correto "YYYY/MM/DD"
gboolean validarFormatoData(const gchar *data) {
    if (strlen(data) != 10 || data[4] != '/' || data[7] != '/') {
        return FALSE;  // Formato inválido
    }

    // Verifica se todos os outros caracteres são dígitos
    for (int i = 0; i < 10; i++) {
        if (i == 4 || i == 7) continue;
        if (!g_ascii_isdigit(data[i])) return FALSE;
    }

    return TRUE;
}

// Verifica se o mês e o dia são válidos
gboolean validarMesEDia(int mes, int dia) {
    return (mes >= 1 && mes <= 12 && dia >= 1 && dia <= 31);
}

// Verifica se a data fornecida está no futuro em relação à data atual
gboolean validarDataFutura(int ano, int mes, int dia) {
    const int anoAtual = 2024, mesAtual = 9, diaAtual = 9; // data atual

    if (ano > anoAtual || (ano == anoAtual && (mes > mesAtual || (mes == mesAtual && dia > diaAtual)))) {
        return FALSE;  // Data no futuro
    }

    return TRUE;
}

// Valida se a data de nascimento do usuario é válida
gboolean validarDataUsuario(const Usuario *usuario) {
    const gchar *birth_date = usuario->birth_date;

    if (!validarFormatoData(birth_date)) {
        printf("Formato inválido para a data de nascimento!\n");
        return FALSE;
    }

    // Extrai ano, mês e dia da data de nascimento do usuario
    int ano, mes, dia;
    sscanf(birth_date, "%4d/%2d/%2d", &ano, &mes, &dia);

    if (!validarMesEDia(mes, dia) || !validarDataFutura(ano, mes, dia)) {
        printf("Data de nascimento inválida!\n");
        return FALSE;
    }

    return TRUE;  
}

// Calcula a idade do usuário com base na data de nascimento
gint calcularIdade(const Usuario *usuario) {
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



// Função que valida o email
gboolean validaEmail(const Usuario *usuario) {
    const gchar *email = usuario->email;

    int arroba_count = 0;
    int ponto_count = 0;
    int a = -1;
    int p = -1;
    gboolean antes_arroba = TRUE;

    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') {
            arroba_count++;
            antes_arroba = FALSE;  // Muda para após o '@'
            a = i;  
        } else if (email[i] == '.') {
            ponto_count++;
            p = i; 
        } else if (antes_arroba) {
            if (!g_ascii_islower(email[i]) && !g_ascii_isdigit(email[i])) {
                return FALSE;
            }
        } else {
            if (!g_ascii_islower(email[i])) {
                return FALSE;
            }
        }
    }

    // Verificações finais:
    if (arroba_count != 1 || ponto_count != 1) return FALSE;
    if (a == 0 || p == a + 1 || p == -1 || (p - a <= 1)) return FALSE;

    int tamanho_apos_ponto = strlen(email) - p - 1;
    if (tamanho_apos_ponto < 2 || tamanho_apos_ponto > 3) return FALSE;

    return TRUE;
}

// Função que valida a subscrição do usuário
gboolean valida_subscricao(const Usuario *usuario) {
    // Se a subscrição não for normal e não for premium, é inválida.
    if (g_strcmp0(usuario->subscription_type, "normal") != 0 && g_strcmp0(usuario->subscription_type, "premium") != 0) {
        return FALSE;
    }
    return TRUE;
}

// Função que valida se as músicas que têm gosto existem
gboolean valida_liked_musics_id(const Usuario *usuario, GestorMusicas *musicas) {
    
    for (int i = 0; usuario->liked_musics_id[i] != NULL; i++) {
        Musica *musica_encontrada = (Musica *)g_hash_table_lookup(musicas, usuario->liked_musics_id[i]);

        if (musica_encontrada == NULL) {
            return FALSE; 
        }
    }
    return TRUE;
}


gboolean valida_user (Usuario* user, GestorMusicas* musics) {
    if (user == NULL) return;

    if (validarDataUsuario (user) && valida_subscricao (user)&& valida_liked_musics_id(user, musics)&& validaEmail (user)) 
    return TRUE;
    else return FALSE;

}