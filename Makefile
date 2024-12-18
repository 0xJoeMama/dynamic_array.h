CC=clang
CFLAGS=-O3 -Wall -Wextra -Werror -pedantic -std=c99
BINS=dynamic_array_tests dynamic_array_tests_new

all: $(BINS)

clean:
	rm -rf $(BINS)
