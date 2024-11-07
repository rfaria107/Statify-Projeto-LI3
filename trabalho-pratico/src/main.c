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

    char *path = g_strdup(argv[2]);    
    char *pathartistas = strcat(path,"/com_erros/artists.csv");

    FILE *fileartistas = fopen(pathartistas, "r");
    // dar parse aos artistas
    parser_principal(fileartistas, gestor, 'a');
    fclose(fileartistas);
    free(pathartistas);

    char *pathusers = strcat(path,"/com_erros/users.csv");

    FILE *fileusers = fopen(pathusers, "r");
    // dar parse aos users
    parser_principal(fileusers, gestor, 'u');
    fclose(fileusers);
    free(pathusers);

    char *pathmusicas = strcat(path,"/com_erros/musics.csv");
    FILE *filemusicas = fopen(pathmusicas, "r");
    // dar parse às musicas
    parser_principal(filemusicas, gestor, 'm');
    fclose(filemusicas);
    free(pathmusicas);
    
    free(path);
    //if(g_strcomp(argv[1],"1")==0){
    //
    //}
    return 0;
}
