    #ifndef ARTISTA_H
    #define ARTISTA_H

    #include <glib.h>

    typedef struct Artista Artista;

    // Função que valida se um artista individual nao tem elementos.  individual/group
    gboolean valida_artista_individual(const Artista *artista);
    void free_artista(Artista *artista);
    Artista *inicializar_artista();
    #endif // ARTISTA_H
