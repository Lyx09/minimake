SRC=src/*.c
OBJ=${SRC:.c=.o}
CC=gcc

all: minimake

minimake: ${SRC}
	gcc -Wall -Werror -Wextra -pedantic -std=c99 $^ -o minimake -I include/ -g

fsanitize: ${SRC}
	gcc -Wall -Werror -Wextra -pedantic -std=c99 $^ -o minimake -I include/ -g -fsanitize=address

check: minimake
	tests/test.py minimake

clean:
	rm -f ${OBJ} minimake

.PHONY: all clean check
