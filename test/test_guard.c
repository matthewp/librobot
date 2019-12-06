#include <stdio.h>
#include "../robot.c"
#include "../unity/src/unity.h"

void setUp(void) {
    // set stuff up here
}

void tearDown(void) {
    // clean stuff up here
}

static bool guard_fail(void* data, Event ev)
{
  return false;
}

static bool guard_pass(void* data, Event ev)
{
  return true;
}

void test_CanPreventProceeding()
{
  Transition t = rbt_transition("next", "last");
  rbt_add_guard(&t, &guard_fail);

  Machine machine = rbt_machine(
    (State[]) {
      rbt_state("idle", (Transition[]) { t }, 1),
      rbt_final("last")
    },
  2);

  State *state = machine.initial;

  TEST_ASSERT_EQUAL_STRING_MESSAGE("idle", state->name, "Initially in the idle state");

  state = rbt_send(&machine, state, (Event) { .name = "next" });
  TEST_ASSERT_EQUAL_STRING_MESSAGE("idle", state->name, "Still in the idle state");
}

void test_ReturnTrueMovesToNextState()
{
  Transition t = rbt_transition("next", "last");
  rbt_add_guard(&t, &guard_pass);

  Machine machine = rbt_machine(
    (State[]) {
      rbt_state("idle", (Transition[]) { t }, 1),
      rbt_final("last")
    },
  2);

  State *state = machine.initial;

  TEST_ASSERT_EQUAL_STRING_MESSAGE("idle", state->name, "Initially in the idle state");

  state = rbt_send(&machine, state, (Event) { .name = "next" });
  TEST_ASSERT_EQUAL_STRING_MESSAGE("last", state->name, "Now in the last state");
}

void test_MultipleGuardsChecksIfAnyFail()
{
  Transition t = rbt_transition("next", "last");
  rbt_add_guard(&t, &guard_fail);
  rbt_add_guard(&t, &guard_pass);

  Machine machine = rbt_machine(
    (State[]) {
      rbt_state("idle", (Transition[]) { t }, 1),
      rbt_final("last")
    },
  2);

  State *state = machine.initial;

  TEST_ASSERT_EQUAL_STRING_MESSAGE("idle", state->name, "Initially in the idle state");

  state = rbt_send(&machine, state, (Event) { .name = "next" });
  TEST_ASSERT_EQUAL_STRING_MESSAGE("idle", state->name, "Stays in the idle state");
}

int main(void)
{
  UNITY_BEGIN();
  RUN_TEST(test_CanPreventProceeding);
  RUN_TEST(test_ReturnTrueMovesToNextState);
  RUN_TEST(test_MultipleGuardsChecksIfAnyFail);
  return UNITY_END();
}