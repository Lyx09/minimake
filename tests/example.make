SRC=main.c toto.c
OBJ=main.o toto.o
CC=gcc
PROG=toto

all: ${PROG}
	# Compile the final binary

${PROG} : ${OBJ}
	${CC} $^ -o $@

# Pattern rule
%.o: %.c
	@ $(CC) -c $<
