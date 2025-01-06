#ifndef QUERY3_AUX_H
#define QUERY3_AUX_H

typedef struct GenrePopularity GenrePopularity;

GenrePopularity *create_genre_popularity(const char *genre);
void free_genre_popularity(GenrePopularity *gp);
int compare_genre_popularity(const void *a, const void *b);
char *get_gp_genre(GenrePopularity *gp);
void set_gp_genre(GenrePopularity *gp, char *genre);
int get_gp_total_likes(GenrePopularity *gp);
void set_gp_total_likes(GenrePopularity *gp, int total_likes);

#endif