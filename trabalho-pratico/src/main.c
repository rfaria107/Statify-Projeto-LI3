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
#include "../include/parsing/queries.h"


int main(int argc, char *argv[])
{

    GestorSistema *gestor = criar_gestor_sistema(); // Inicializa o gestor

    char *path = g_strdup(argv[1]);

    char *pathartistas = g_strconcat(path, "/com_erros/artists.csv", NULL);

    FILE *fileartistas = fopen(pathartistas, "r");

    // dar parse aos artistas
    if (fileartistas)
    {
        parser_principal(fileartistas, gestor, 'a');
        fclose(fileartistas);
    }
    g_free(pathartistas);

    char *pathmusicas = g_strconcat(path, "/com_erros/musics.csv",NULL);
    FILE *filemusicas = fopen(pathmusicas, "r");
    // dar parse às musicas
    if(filemusicas){
    parser_principal(filemusicas, gestor, 'm');
    fclose(filemusicas);
    }
    g_free(pathmusicas);

    char *pathusers = g_strconcat(path, "/com_erros/users.csv",NULL);

    FILE *fileusers = fopen(pathusers, "r");
    // dar parse aos users
    if(fileusers){
    parser_principal(fileusers, gestor, 'u');
    fclose(fileusers);
    }
    g_free(pathusers);

    g_free(path);
    
    char *pathinputs = g_strdup(argv[2]);
    FILE *fileinputs = fopen(pathinputs, "r");
    interpreter_inputs(fileinputs,gestor);
    fclose(fileinputs);
    
    liberar_gestor_sistema(gestor);

    return 0;
}
