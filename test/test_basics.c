#include <stdio.h>
#include "../robot.c"
#include "../unity/src/unity.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_Basics()
{
  Machine machine = rbt_machine(
    (State[]) {
      rbt_state("on", (Transition[]) {
        rbt_transition("toggle", "off")
      }, 1),
      rbt_state("off", (Transition[]) {
        rbt_transition("toggle", "on")
      }, 1)
    },
  2);

  State *state = machine.initial;

  TEST_ASSERT_EQUAL_STRING_MESSAGE("on", state->name, "Initially on");

  state = rbt_send(&machine, state, (Event) { .name = "toggle" });
  TEST_ASSERT_EQUAL_STRING_MESSAGE("off", state->name, "Toggles to off");

  state = rbt_send(&machine, state, (Event) { .name = "toggle" });
  TEST_ASSERT_EQUAL_STRING_MESSAGE("on", state->name, "Toggles back to on");

  rbt_machine_cleanup(&machine);
}

void test_Immediate()
{
  Machine machine = rbt_machine(
    (State[]) {
      rbt_state("initial", (Transition[]) {
        rbt_transition("next", "second")
      }, 1),
      rbt_state("second", (Transition[]) {
        rbt_immediate("last")
      }, 1),
      rbt_final("last")
    },
  3);

  State *state = rbt_send(&machine, machine.initial, (Event) { .name = "next" });

  TEST_ASSERT_EQUAL_STRING_MESSAGE("last", state->name, "Moved to last");
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_Basics);
  RUN_TEST(test_Immediate);
  return UNITY_END();
}