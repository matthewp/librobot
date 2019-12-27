#include "robot.h"

static bool default_guard(void* data, Event ev)
{
  return true;
}

Transition rbt_transition(char *from, char *to)
{
  Guard *guard = malloc(sizeof *guard);
  guard->fn = &default_guard;
  guard->next = NULL;

  Transition t = {
    .from = from,
    .to = to,
    .guard = guard,
    .mutater = NULL
  };

  return t;
}

Transition * rbt_add_guard(Transition *t, GuardFunction *guard_function)
{
  Guard *guard = malloc(sizeof *guard);
  guard->fn = guard_function;
  guard->next = t->guard;

  t->guard = guard;

  return t;
}

Transition * rbt_add_mutate(Transition *t, MutateFunction *mutate_function)
{
  Mutater *mutater = malloc(sizeof *mutater);
  mutater->fn = mutate_function;
  mutater->next = t->mutater;

  t->mutater = mutater;

  return t;
}

Machine * rbt_add_data(Machine *machine, void* data)
{
  machine->data = data;
  return machine;
}

State rbt_state(char *name, Transition transitions[], size_t n)
{
  State s = {.name = name};

  int i = 0;
  Transition *last = NULL;

  while(i < n) {
    Transition t = transitions[i];
    t.next = last;
    last = &t;

    i++;
  }

  if(n > 0) {
    s.transition = &transitions[0];
  } else {
    s.transition = NULL;
  }

  return s;
}

State rbt_final(char *name)
{
  return rbt_state(name, (Transition[]) {}, 0);
}

Machine rbt_machine(State states[], size_t n)
{
  Machine m = {};

  int i = n - 1;
  State *last = NULL;

  while(i >= 0)
  {
    State *s = &states[i];
    s->next = last;

    last = s;
    i--;
  }

  m.initial = &states[0];

  return m;
}

void rbt_machine_cleanup(Machine *machine)
{
  State *state = machine->initial;

  while(state != NULL) {
    Transition *transition = state->transition;

    while(transition != NULL) {
      Guard *guard = transition->guard;

      while(guard != NULL) {
        Guard *next_guard = guard->next;
        free(guard);
        guard = next_guard;
      }

      Mutater *mutater = transition->mutater;

      while(mutater != NULL) {
        Mutater *next_mutater = mutater->next;
        free(mutater);
        mutater = next_mutater;
      }

      transition = transition->next;
    }

    state = state->next;
  }
}

bool run_guards(Guard *g, Event ev, void* d)
{
  bool passes = true;
  while(g != NULL) {
    passes = g->fn(d, ev);

    if(!passes) {
      break;
    }

    g = g->next;
  }

  return passes;
}

void run_mutators(Mutater *m, Event ev, void* d)
{
  bool passes = true;
  while(m != NULL) {
    m->fn(d, ev);
    m = m->next;
  }
}

State * rbt_send(Machine *machine, State *current, Event ev)
{
  Transition *t = current->transition;

  while(t != NULL) {
    if(strcmp(t->from, ev.name) == 0) {
      if(!run_guards(t->guard, ev, machine->data)) {
        break;
      }

      run_mutators(t->mutater, ev, machine->data);

      char *new_state_name = t->to;
      State *new_state = machine->initial;

      while(new_state != NULL) {
        if(strcmp(new_state->name, new_state_name) == 0) {
          return new_state;
        }
        new_state = new_state->next;
      }
    }

    t = t->next;
  }

  return current;
}
