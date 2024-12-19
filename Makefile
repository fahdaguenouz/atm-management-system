objects = src/main.o src/system.o src/auth.o
CC = gcc
CFLAGS = -Wall -g
LIBS = -lsqlite3

atm : $(objects)
	$(CC) -o atm $(objects) $(LIBS)

main.o : src/header.h
kbd.o : src/header.h
command.o : src/header.h
display.o : src/header.h
insert.o : src/header.h
search.o : src/header.h
files.o : src/header.h
utils.o : src/header.h

clean :
	rm -f $(objects) atm
