
program: main.c librobot.c
	$(CC) -o $@ main.c

clean:
	@rm program
.PHONY: clean

run: program
	@./program
.PHONY: run