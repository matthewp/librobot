#include <stdio.h>
#include "../robot.c"
#include "../unity/src/unity.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

void test_InvokeSubMachine()
{
  Machine crosswalk = rbt_machine(
    (State[]) {
      rbt_state("walk", (Transition[]) {
        rbt_transition("next", "done")
      }, 1),
      rbt_final("done")
    },
  2);

  Machine stoplight = rbt_machine(
    (State[]) {
      rbt_state("red", (Transition[]) {
        rbt_transition("next", "yellow")
      }, 1),
      rbt_state("yellow", (Transition[]) {
        rbt_transition("next", "green")
      }, 1),
      rbt_invoke("green", &crosswalk, (Transition[]) {
        rbt_transition("done", "red")
      }, 1)
    },
  3);

  printf("Before\n");
  State *state = stoplight.initial->state;
  printf("After\n");

  TEST_ASSERT_EQUAL_STRING_MESSAGE("red", state->name, "Initially red");

  printf("Before send\n");
  Current curr = rbt_send(&stoplight, state, (Event) {.name = "next"}); // yellow
  printf("After send\n");
  printf("Before send\n");
  curr = rbt_send(&stoplight, curr.state, (Event) {.name = "next"}); // green
  printf("After send\n");

  TEST_ASSERT_NOT_NULL_MESSAGE(state, "Child machine exists");

  rbt_machine_cleanup(&stoplight);
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_InvokeSubMachine);
  return UNITY_END();
}