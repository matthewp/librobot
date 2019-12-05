
program: main.c librobot.c
	$(CC) -o $@ main.c

build/TestBasics: librobot.c test/test_basics.c unity/src/unity.c
	$(CC) -o $@ test/test_basics.c ./unity/src/unity.c

clean:
	@rm program
	@rm build/TestBasics
.PHONY: clean

run: program
	@./program
.PHONY: run

test: build/TestBasics
	@build/TestBasics
.PHONY: test