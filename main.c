#include <stdio.h>
#include "librobot.c"

void print_current(State *state)
{
  printf("Current: %s\n", state->name);
}

int main()
{
  State green = rbt_state("green", (Transition[]) {
    rbt_transition("next", "yellow")
  }, 1);

  State yellow = rbt_state("yellow", (Transition[]) {
    rbt_transition("next", "red")
  }, 1);

  State red = rbt_state("red", (Transition[]) {
    rbt_transition("next", "green")
  }, 1);

  State states[] = { green, yellow, red };
  Machine machine = rbt_machine(states, 3);

  State *state = machine.initial;

  print_current(state);

  state = rbt_send(&machine, state, "next");
  print_current(state);

  state = rbt_send(&machine, state, "next");
  print_current(state);

  return 0;
}