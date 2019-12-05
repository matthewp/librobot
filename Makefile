
program: main.c librobot.c
	$(CC) -o $@ main.c

build/TestBasics: test/test_basics.c librobot.c unity/src/unity.c
	$(CC) -o $@ test/test_basics.c ./unity/src/unity.c

clean:
	@rm -f program
	@rm -f build/TestBasics
.PHONY: clean

run: program
	@./program
.PHONY: run

test: build/TestBasics
	@build/TestBasics
.PHONY: test