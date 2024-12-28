all: run

run: compile
	./run $(ARGS)

compile: GerarCaverna.o MainCaverna.o
	gcc -o run GerarCaverna.o MainCaverna.o

GerarCaverna.o: GerarCaverna.c
	gcc -c GerarCaverna.c -o GerarCaverna.o

MainCaverna.o: MainCaverna.c
	gcc -c MainCaverna.c -o MainCaverna.o

# Limpar arquivos compilados
clean:
	rm -f run GerarCaverna.o MainCaverna.o
