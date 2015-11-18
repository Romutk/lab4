	hello : hello.o

	hello.o : hello.c
	gcc -c hello.c



	clean :

	rm hello.o
