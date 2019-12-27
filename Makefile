
build/StopLight: examples/stoplight.c robot.c robot.h
	$(CC) -o $@ examples/stoplight.c

build/TestBasics: test/test_basics.c robot.c robot.h unity/src/unity.c
	$(CC) -o $@ test/test_basics.c ./unity/src/unity.c

build/TestGuard: test/test_guard.c robot.c robot.h unity/src/unity.c
	$(CC) -o $@ test/test_guard.c ./unity/src/unity.c

build/TestMutate: test/test_mutate.c robot.c robot.h unity/src/unity.c
	$(CC) -o $@ test/test_mutate.c ./unity/src/unity.c

clean:
	@rm -f build/StopLight
	@rm -f build/TestBasics
	@rm -f build/TestGuard
.PHONY: clean

run: build/StopLight
	@./build/StopLight
.PHONY: run

test: build/TestBasics build/TestGuard build/TestMutate
	@build/TestBasics
	@build/TestGuard
	@build/TestMutate
.PHONY: test