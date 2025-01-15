CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lws2_32
BIN_DIR = bin

# Création du répertoire bin s'il n'existe pas
$(shell mkdir -p $(BIN_DIR))

# Cibles pour chatServer
chatServer: chatServer.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/chatServer chatServer.o $(LDFLAGS)

# Cibles pour chatClient
chatClient: chatClient.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/chatClient chatClient.o $(LDFLAGS)

# Cibles pour tictactoe
tictactoe: tictactoe.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/tictactoe tictactoe.o $(LDFLAGS)

# Cibles pour tictactoeClient
tictactoeClient: tictactoeClient.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/tictactoeClient tictactoeClient.o $(LDFLAGS)

# Règles de compilation des fichiers .o
chatServer.o: chatServer.c
	$(CC) $(CFLAGS) -c chatServer.c

chatClient.o: chatClient.c
	$(CC) $(CFLAGS) -c chatClient.c

tictactoe.o: tictactoe.c
	$(CC) $(CFLAGS) -c tictactoe.c

tictactoeClient.o: tictactoeClient.c
	$(CC) $(CFLAGS) -c tictactoeClient.c

# Nettoyage des fichiers objets et exécutables
clean:
	rm -f *.o $(BIN_DIR)/chatServer $(BIN_DIR)/chatClient $(BIN_DIR)/tictactoe $(BIN_DIR)/tictactoeClient
