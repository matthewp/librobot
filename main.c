#include <stdio.h>
#include "librobot.c"

typedef struct FormData {
  char *login;
  char *password;
} FormData;

void print_current(State *state)
{
  printf("Current: %s\n", state->name);
}

bool canTransition(FormData *data, Event event)
{
  return true;
}

void setLogin(FormData *data, Event event)
{
  printf("Running the mutater\n");
  data->login = "something else";
}

int main()
{
  Transition nextToYellow = rbt_transition("next", "yellow");
  rbt_add_guard(&nextToYellow, (void*)&canTransition);

  rbt_add_mutate(&nextToYellow, (void*)&setLogin);

  State green = rbt_state("green", (Transition[]) {
    nextToYellow
  }, 1);

  State yellow = rbt_state("yellow", (Transition[]) {
    rbt_transition("next", "red")
  }, 1);

  State red = rbt_state("red", (Transition[]) {
    rbt_transition("next", "green")
  }, 1);

  State states[] = { green, yellow, red };
  Machine machine = rbt_machine(states, 3);

  FormData data = {};
  rbt_add_data(&machine, &data);

  State *state = machine.initial;

  print_current(state);

  state = rbt_send(&machine, state, (Event) { .name = "next" });
  print_current(state);

  state = rbt_send(&machine, state, (Event) { .name = "next" });
  print_current(state);

  printf("Login is %s\n", data.login);

  rbt_machine_cleanup(&machine);

  return 0;
}