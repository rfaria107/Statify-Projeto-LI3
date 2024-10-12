#include <stdio.h>
#include <stdbool.h>
#include "../include/gestor_sistemas.h"
#include "../include/usuario.h"

int main() {
    GestorSistema gestor;
    inicializar_gestor(&gestor);

// Exemplo
    Usuario u1 = {"john_doe", "john@example.com", "John", "Doe", "1990/01/01", "USA", "premium"};
    adicionar_usuario(&gestor, &u1);

    // Busca o usuário
    Usuario *usuarioBuscado = buscar_usuario(&gestor, "john_doe");
    if (usuarioBuscado) {
        imprimir_usuario(usuarioBuscado);
    } else {
        printf("Usuário não encontrado!\n");
    }

    // liberta a memória
    liberar_gestor(&gestor);

    return 0;
}
