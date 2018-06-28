all: main.o nJson.o
	gcc -o json_generator main.o nJson.o

main.o: main.c
	gcc -c main.c -I ./

nJson.o: nJson.c
	gcc -c nJson.c -I ./

clean:
	rm -rf *.o
	rm -rf json_generator

memory-check:
	valgrind --leak-check=yes ./json_generator -c
