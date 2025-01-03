# Variáveis
CC := gcc
CFLAGS = -Wall -g `pkg-config --cflags glib-2.0` -I$(INC_DIR)
SRC_DIR = src
INC_DIR = include

RECOMENDADOR_OBJ = $(SRC_DIR)/recomendador/recomendador-linux-x86_64.o

# Objetos principais (para o executável principal)
OBJ_MAIN = $(SRC_DIR)/main.o \
           $(SRC_DIR)/entidades/usuario.o \
           $(SRC_DIR)/entidades/musica.o \
           $(SRC_DIR)/entidades/artists.o \
           $(SRC_DIR)/entidades/albuns.o \
           $(SRC_DIR)/entidades/history.o \
           $(SRC_DIR)/gestores/gestor_usuarios.o \
           $(SRC_DIR)/gestores/gestor_musicas.o \
           $(SRC_DIR)/gestores/gestor_artistas.o \
           $(SRC_DIR)/gestores/gestor_albuns.o \
           $(SRC_DIR)/gestores/gestor_histories.o \
           $(SRC_DIR)/gestores/gestor_sistemas.o \
           $(SRC_DIR)/io/io.o \
           $(SRC_DIR)/parsing/parser.o \
           $(SRC_DIR)/utils/string_utils.o \
           $(SRC_DIR)/write/writer.o \
           $(SRC_DIR)/write/file.o \
           $(SRC_DIR)/validacao/valida.o \
           $(SRC_DIR)/queries/queries.o \
           $(SRC_DIR)/queries/queries_aux.o \
           $(SRC_DIR)/queries/query5_aux.o \
           $(RECOMENDADOR_OBJ)

# Objetos de teste (para o executável de testes)
OBJ_TEST = $(SRC_DIR)/testes.o \
           $(SRC_DIR)/entidades/usuario.o \
           $(SRC_DIR)/entidades/musica.o \
           $(SRC_DIR)/entidades/artists.o \
           $(SRC_DIR)/entidades/albuns.o \
           $(SRC_DIR)/entidades/history.o \
           $(SRC_DIR)/gestores/gestor_usuarios.o \
           $(SRC_DIR)/gestores/gestor_musicas.o \
           $(SRC_DIR)/gestores/gestor_artistas.o \
           $(SRC_DIR)/gestores/gestor_sistemas.o \
           $(SRC_DIR)/gestores/gestor_albuns.o \
           $(SRC_DIR)/gestores/gestor_histories.o \
           $(SRC_DIR)/io/io.o \
           $(SRC_DIR)/parsing/parser.o \
           $(SRC_DIR)/utils/string_utils.o \
           $(SRC_DIR)/write/writer.o \
           $(SRC_DIR)/write/file.o \
           $(SRC_DIR)/validacao/valida.o \
           $(SRC_DIR)/queries/queries.o \
           $(SRC_DIR)/queries/queries_aux.o \
           $(SRC_DIR)/queries/query5_aux.o \
           $(RECOMENDADOR_OBJ)

# Alvos (executáveis)
TARGET = programa-principal
TEST_TARGET = programa-testes

# Regra padrão
all: $(TARGET) $(TEST_TARGET)

# Regra para compilar o executável principal
$(TARGET): $(OBJ_MAIN)
	$(CC) -o $(TARGET) $(OBJ_MAIN) `pkg-config --libs glib-2.0` -lm

# Regra para compilar o programa de testes
$(TEST_TARGET): $(OBJ_TEST)
	$(CC) -o $(TEST_TARGET) $(OBJ_TEST) `pkg-config --libs glib-2.0` -lm

# Regras para compilar os arquivos .o
$(SRC_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/entidades/usuario.o: $(SRC_DIR)/entidades/usuario.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/entidades/musica.o: $(SRC_DIR)/entidades/musica.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/entidades/artists.o: $(SRC_DIR)/entidades/artists.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/entidades/albuns.o: $(SRC_DIR)/entidades/albuns.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/entidades/history.o: $(SRC_DIR)/entidades/history.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/gestores/gestor_usuarios.o: $(SRC_DIR)/gestores/gestor_usuarios.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/gestores/gestor_musicas.o: $(SRC_DIR)/gestores/gestor_musicas.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/gestores/gestor_artistas.o: $(SRC_DIR)/gestores/gestor_artistas.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/gestores/gestor_albuns.o: $(SRC_DIR)/gestores/gestor_albuns.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/gestores/gestor_histories.o: $(SRC_DIR)/gestores/gestor_histories.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/gestores/gestor_sistemas.o: $(SRC_DIR)/gestores/gestor_sistemas.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/parsing/parser.o: $(SRC_DIR)/parsing/parser.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/parsing/string_utils.o: $(SRC_DIR)/parsing/string_utils.c   
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/write/writer.o: $(SRC_DIR)/write/writer.c   
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/write/file.o: $(SRC_DIR)/write/file.c   
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/validacao/valida.o: $(SRC_DIR)/validacao/valida.c  
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/queries/queries.o: $(SRC_DIR)/queries/queries.c  
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/queries/queries_aux.o: $(SRC_DIR)/queries/queries_aux.c  
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/testes.o: $(SRC_DIR)/testes.c  
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para executar o programa principal com o Valgrind
valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

# Limpar arquivos gerados
clean:
	rm -f $(filter-out $(RECOMENDADOR_OBJ), $(OBJ_MAIN) $(OBJ_TEST)) $(TARGET) $(TEST_TARGET)
