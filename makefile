FLAGS = -Wall -lm

lab1: lab1.c padre.o hijo.o
	gcc $(FLAGS) -o lab1 lab1.c padre.o hijo.o

padre.o: padre.c
	gcc -c $(FLAGS) padre.c

hijo.o: hijo.c
	gcc -c $(FLAGS) hijo.c

clear:
	rm -f *.o lab1
