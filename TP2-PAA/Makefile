#all: run

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
