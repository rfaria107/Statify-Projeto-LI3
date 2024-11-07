#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "../include/entidades/artists.h"
#include "../include/entidades/musica.h"
#include "../include/entidades/usuario.h"
#include "../include/gestores/gestor_artistas.h"
#include "../include/gestores/gestor_musicas.h"
#include "../include/gestores/gestor_sistemas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/parsing/parser.h"
#include "../include/parsing/rowreader.h"
#include "../include/parsing/string_utils.h"
#include "../include/validacao/valida.h"

int main(int argc, char *argv[])
{

    GestorSistema *gestor = criar_gestor_sistema(); // Inicializa o gestor
    
    char *pathartistas = calloc(200, sizeof(char));
    snprintf(pathartistas, sizeof(pathartistas), "%s/com_erros/artists.csv", argv[2]);
    FILE *fileartistas = fopen(pathartistas, "r");
    // dar parse aos artistas
    parser_principal(fileartistas, gestor, 'a');
    fclose(fileartistas);
    free(pathartistas);

    char *pathusers = calloc(200, sizeof(char));
    snprintf(pathusers, sizeof(pathusers), "%s/com_erros/artists.csv", argv[2]);
    FILE *fileusers = fopen(pathusers, "r");
    // dar parse aos users
    parser_principal(fileusers, gestor, 'u');
    fclose(users);
    free(pathusers);

    char *pathmusicas = calloc(200, sizeof(char));
    snprintf(pathmusicas, sizeof(pathmusicas), "%s/com_erros/artists.csv", argv[2]);
    FILE *filemusicas = fopen(pathmusicas, "r");
    // dar parse às musicas
    parser_principal(filemusicas, gestor, 'm');
    fclose(musicas);
    free(pathmusicas);

    if(g_strcomp(argv[1],"1")==0){

    }
    return 0;
}
