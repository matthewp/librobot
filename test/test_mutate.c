#include <stdio.h>
#include "../robot.c"
#include "../unity/src/unity.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

typedef struct Counter {
  int count;
} Counter;

void add_one(Counter *c, Event ev)
{
  c->count = c->count + 1;
}

void test_MutatorMutatesData()
{
  Transition t = rbt_transition("inc", "idle");
  rbt_add_mutate(&t, (void*)&add_one);

  Machine machine = rbt_machine(
    (State[]) {
      rbt_state("idle", (Transition[]) { t }, 1),
      rbt_final("last")
    },
  2);

  Counter data = { .count = 0 };
  rbt_add_data(&machine, &data);

  State *state = machine.initial;

  TEST_ASSERT_EQUAL_STRING_MESSAGE("idle", state->name, "Initially in the idle state");

  rbt_send(&machine, state, (Event) { .name = "inc" });
  rbt_machine_cleanup(&machine);

  TEST_ASSERT_EQUAL_INT_MESSAGE(1, data.count, "Incremented the count");
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_MutatorMutatesData);
  return UNITY_END();
}