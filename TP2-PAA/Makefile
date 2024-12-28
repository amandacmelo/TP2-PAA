# Definir um valor fixo para o arquivo de entrada
#INPUT_FILE = teste.txt
#all: compile

all: run

run: compile
	./run $(ARGS)

compile: Caverna/Caverna.o main.o
	gcc -o run Caverna/Caverna.o main.o

Caverna/Caverna.o: Caverna/Caverna.c
	gcc -c Caverna/Caverna.c -o Caverna/Caverna.o

main.o: main.c
	gcc -c main.c -o main.o

# Limpar arquivos compilados
clean:
	rm -f run Caverna/*.o main.o
