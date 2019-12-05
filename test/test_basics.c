#include <stdio.h>
#include "../librobot.c"
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

}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_Basics);
  return UNITY_END();
}