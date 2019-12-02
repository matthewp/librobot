
program: main.c librobot.c
	cc -o $@ main.c

clean:
	@rm program
.PHONY: clean

run: program
	@./program
.PHONY: run