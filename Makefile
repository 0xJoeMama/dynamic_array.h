CC=gcc
CFLAGS=-O3 -Wall -Wextra -Werror -pedantic -std=c99
BINS=dynamic_array_tests

all: $(BINS)

clean:
	rm -rf $(BINS) compile_commands.json .cache
