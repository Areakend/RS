# Dans le bash, au choix:
# make all : nettoie le dossier puis compile
# make project : compile le projet
# make mrpropre : supprime les *.o


CC = gcc
CFLAGS = -Wall -Wextra
EXEC = project
LDFLAGS =


all : clean $(EXEC)
	./$(EXEC)

$(EXEC) : tesh.o cut_buffer.o
	$(CC) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(CC) -o $@ -c $^ $(CFLAGS)

clean:
	rm -rf project *.o
	echo Clean done

mrpropre:
	rm -rf *.o
