SRC = coordcube.c cubiecube.c facecube.c search.c
INCLUDE = include
CFLAGS = -std=c99 -O3
BINDIR = bin
BIN = cube_solver

solve: solve.c $(SRC)
	mkdir -p $(BINDIR)
	gcc $(CFLAGS) $(SRC) -I$(INCLUDE) solve.c -o $(BINDIR)/$(BIN)
