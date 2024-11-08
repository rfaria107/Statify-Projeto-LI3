# Variáveis
CC := gcc
CFLAGS = -Wall -g `pkg-config --cflags glib-2.0` -I$(INC_DIR)
SRC_DIR = src
INC_DIR = include

# Adiciona todos os arquivos objeto
OBJ = $(SRC_DIR)/main.o \
      $(SRC_DIR)/entidades/usuario.o \
      $(SRC_DIR)/entidades/musica.o \
      $(SRC_DIR)/entidades/artists.o \
      $(SRC_DIR)/gestores/gestor_usuarios.o \
      $(SRC_DIR)/gestores/gestor_musicas.o \
      $(SRC_DIR)/gestores/gestor_artistas.o \
      $(SRC_DIR)/gestores/gestor_sistemas.o \
      $(SRC_DIR)/parsing/parser.o \
      $(SRC_DIR)/parsing/rowreader.o \
      $(SRC_DIR)/parsing/string_utils.o \
      $(SRC_DIR)/parsing/writer.o \
      $(SRC_DIR)/parsing/file.o \
      $(SRC_DIR)/validacao/valida.o \
      $(SRC_DIR)/queries.o

TARGET = programa-principal

# Regra padrão
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) `pkg-config --libs glib-2.0`

# Regras para compilar os arquivos .o
$(SRC_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/entidades/usuario.o: $(SRC_DIR)/entidades/usuario.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/entidades/musica.o: $(SRC_DIR)/entidades/musica.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/entidades/artists.o: $(SRC_DIR)/entidades/artists.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/gestores/gestor_usuarios.o: $(SRC_DIR)/gestores/gestor_usuarios.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/gestores/gestor_musicas.o: $(SRC_DIR)/gestores/gestor_musicas.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/gestores/gestor_artistas.o: $(SRC_DIR)/gestores/gestor_artistas.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/gestores/gestor_sistemas.o: $(SRC_DIR)/gestores/gestor_sistemas.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/parsing/parser.o: $(SRC_DIR)/parsing/parser.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/parsing/rowreader.o: $(SRC_DIR)/parsing/rowreader.c
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/parsing/string_utils.o: $(SRC_DIR)/parsing/string_utils.c   
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/parsing/writer.o: $(SRC_DIR)/parsing/writer.c   
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/parsing/file.o: $(SRC_DIR)/parsing/file.c   
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/validacao/valida.o: $(SRC_DIR)/validacao/valida.c  
	$(CC) $(CFLAGS) -c $< -o $@

$(SRC_DIR)/queries.o: $(SRC_DIR)/queries.c  
	$(CC) $(CFLAGS) -c $< -o $@

# Limpar arquivos gerados
clean:
	rm -f $(OBJ) $(TARGET)