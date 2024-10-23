# Variáveis
CC := gcc

CFLAGS = -Wall -g `pkg-config --cflags glib-2.0`
SRC_DIR = src
INC_DIR = include
OBJ = $(SRC_DIR)/main.o $(SRC_DIR)/entidades/usuario.o $(SRC_DIR)/gestores/gestor_usuarios.o
TARGET = main

# Regra padrão
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) `pkg-config --libs glib-2.0`

# Regras para compilar os arquivos .o
$(SRC_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c -o $(SRC_DIR)/main.o

$(SRC_DIR)/entidades/usuario.o: $(SRC_DIR)/entidades/usuario.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/entidades/usuario.c -o $(SRC_DIR)/entidades/usuario.o

$(SRC_DIR)/gestores/gestor_usuarios.o: $(SRC_DIR)/gestores/gestor_usuarios.c
	$(CC) $(CFLAGS) -c $(SRC_DIR)/gestores/gestor_usuarios.c -o $(SRC_DIR)/gestores/gestor_usuarios.o

# Limpar arquivos gerados
clean:
	rm -f $(OBJ) $(TARGET)
