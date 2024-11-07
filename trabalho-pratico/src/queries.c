#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>

#include "../include/entidades/artists.h"
#include "../include/entidades/musica.h"
#include "../include/entidades/usuario.h"
#include "../include/gestores/gestor_artistas.h"
#include "../include/gestores/gestor_musicas.h"
#include "../include/gestores/gestor_usuarios.h"
#include "../include/parsing/writer.h"

gint calcularIdade(const Usuario *usuario) {
    int ano, mes, dia;
    int birth_date = user_get_birth_date (usuario);
    sscanf(birth_date, "%4d/%2d/%2d", &ano, &mes, &dia);
    const int anoAtual = 2024, mesAtual = 9, diaAtual = 9;
    int idade = anoAtual - ano;
    if (mes > mesAtual || (mes == mesAtual && dia > diaAtual))
        idade--;
    return idade;
}

void querie_1(GestorUsuarios* gestor, const char* username) {
   
   GHashTable *usuarios = get_hash_usuarios (gestor);
   Usuario *usuario = (Usuario *) g_hash_table_lookup(usuarios, username);

    if (usuario) {
        // Cria um nome de arquivo único baseado no username
        char output_file_name[100];
        snprintf(output_file_name, sizeof(output_file_name), "%s_output.csv", username);

        RowWriter* writer = initialize_row_writer(output_file_name, WRITE_MODE_CSV);

        // Define nomes e formatos dos campos para o RowWriter
        char* field_names[] = {"Email", "First Name", "Last Name", "Age", "Country"};
        char* formatting[] = {"%s","%s", "%s", "%d", "%s"};
        row_writer_set_field_names(writer, field_names, 5);
        row_writer_set_formatting(writer, formatting);

        gint idade = calcularIdade(usuario);
     
    char *mail = user_get_email (usuario);
    char *first_name = user_get_first_name(usuario);
    char *last_name = user_get_last_name (usuario);
    char *country= user_get_country (usuario);

        // Escreve a linha com os dados do usuário no arquivo de saída
        write_row(writer, 5, mail, first_name, last_name, idade, country);

        free_and_finish_writing(writer);
    } else {
        printf("Usuário %s não encontrado.\n", username);
    }
}
 

/*

// Função para converter "HH:MM:SS" para segundos
gint duracao_para_segundos(const gchar *duracao)
{
    gint horas, minutos, segundos;
    sscanf(duracao, "%d:%d:%d", &horas, &minutos, &segundos);
    return horas * 3600 + minutos * 60 + segundos;
}

// Função para converter segundos em "HH:MM:SS"
gchar *segundos_para_duracao(gint total_segundos)
{
    gint horas = total_segundos / 3600;
    gint minutos = (total_segundos % 3600) / 60;
    gint segundos = total_segundos % 60;

    return g_strdup_printf("%02d:%02d:%02d", horas, minutos, segundos);
}


gchar *calcular_discografia(GestorArtistas *gestorartistas, const Artista *artista)
{
    GHashTable *hash_musicas = get_hash_table(gestorartistas);
    gint duracao_total_segundos = 0;
    
    for (int i = 0; get_ar != NULL && artista->id_constituent[i] != NULL; i++)
    {
        gchar *id_musica = artista->id_constituent[i];
        Musica *musica = (Musica *)g_hash_table_lookup(hash_musicas, id_musica);

        if (musica != NULL)
        {
            duracao_total_segundos += duracao_para_segundos(musica ->duration);
        }
    }

    return segundos_para_duracao(duracao_total_segundos);
}


Querie 3 

void querie_3(int idade_min, int idade_max, GHashTable *gestor_usuarios, GHashTable *gestor_musicas) {
    GHashTableIter iter;
    gpointer key, value;
    GHashTable *genero_contagem = g_hash_table_new(g_str_hash, g_str_equal);

    // Itera pelos usuários
    g_hash_table_iter_init(&iter, gestor_usuarios);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Usuario *usuario = (Usuario *)value;
        if (usuario == NULL) continue; // Verifica se o usuário é NULL
        int idade = calcularIdade(usuario);

        // Verifica se o usuário está dentro da faixa etária
        if (idade >= idade_min && idade <= idade_max) {

            // Itera pelas músicas curtidas pelo usuário
            for (int i = 0; usuario->liked_musics_id && usuario->liked_musics_id[i] != NULL; i++) {
                int music_id = atoi(usuario->liked_musics_id[i]);

                Musica *musica = g_hash_table_lookup(gestor_musicas, GINT_TO_POINTER(music_id));
                if (musica != NULL && musica->genre != NULL) {
                    const char *genero = musica->genre;

                    gpointer count_ptr = g_hash_table_lookup(genero_contagem, genero);
                    int count = (count_ptr != NULL) ? GPOINTER_TO_INT(count_ptr) : 0;
                    g_hash_table_insert(genero_contagem, g_strdup(genero), GINT_TO_POINTER(count + 1));
                }
            }
        }
    }

    // Ordenação e exibição
    GList *genero_lista = g_hash_table_get_keys(genero_contagem);
    genero_lista = g_list_sort(genero_lista, (GCompareFunc)g_ascii_strcasecmp);

    for (GList *iter = genero_lista; iter != NULL; iter = iter->next) {
        const char *genero = iter->data;
        int count = GPOINTER_TO_INT(g_hash_table_lookup(genero_contagem, genero));
        printf("%s: % curtidas\n", genero, count);
    }

    g_list_free_full(genero_lista, g_free);
    g_hash_table_destroy(genero_contagem);
}
*/
