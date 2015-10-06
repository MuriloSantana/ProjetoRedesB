enlace : main.o garbler.o enlace.o teste.o  
	gcc main.o garbler.o enlace.o teste.o -o enlace -lcrypt -lpthread

main.o : main.c
	gcc -c main.c -lcrypt -lpthread

garbler.o : garbler.c
	gcc -c garbler.c

enlace.o : enlace.c
	gcc -c enlace.c -lcrypt -lpthread

teste.o : teste.c
	gcc -c teste.c -lcrypt -lpthread

clean :
	rm teste.o enlace.o garbler.o main.o
