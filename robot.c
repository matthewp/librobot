#include "robot.h"

static State * transition_to(Machine *machine, State *current, Transition *t, Event ev);

static bool default_guard(void* data, Event ev)
{
  return true;
}

static bool is_immediate_transition(Transition *t)
{
  return t->from == NULL;
}

static State * enter_state(Machine *machine, State *state, Event ev)
{
  // A noop at the moment.
  return state;
}

static State * enter_immediate(Machine *machine, State *state, Event ev)
{
  Transition *t = state->transition;

  while(t != NULL) {
    if(is_immediate_transition(t)) {
      return transition_to(machine, state, t, ev);
    }

    t = t->next;
  }

  return state;
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

Transition rbt_immediate(char *to)
{
  return rbt_transition(NULL, to);
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

  int i = n - 1;
  bool isImmediate = false;
  Transition *last = NULL;

  while(i >= 0) {
    Transition t = transitions[i];
    Transition *lt = malloc(sizeof *lt);
    lt->from = t.from;
    lt->to = t.to;
    lt->guard = t.guard;
    lt->mutater = t.mutater;
    lt->next = last;

    if(is_immediate_transition(&t)) {
      isImmediate = true;
    }

    last = lt;
    i--;
  }

  if(n > 0) {
    s.transition = last;
  } else {
    s.transition = NULL;
  }

  if(isImmediate) {
    s.enter = &enter_immediate;
  } else {
    s.enter = &enter_state;
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

      Transition *next_transition = transition->next;
      free(transition);
      transition = next_transition;
    }

    state = state->next;
  }
}

static bool run_guards(Guard *g, Event ev, void* d)
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

static void run_mutators(Mutater *m, Event ev, void* d)
{
  bool passes = true;
  while(m != NULL) {
    m->fn(d, ev);
    m = m->next;
  }
}

static State * transition_to(Machine *machine, State *current, Transition *t, Event ev)
{
  if(!run_guards(t->guard, ev, machine->data)) {
    return current;
  }

  run_mutators(t->mutater, ev, machine->data);

  char *new_state_name = t->to;
  State *new_state = machine->initial;

  int i = 0;
  while(new_state != NULL) {
    i++;
    if(strcmp(new_state->name, new_state_name) == 0) {
      return new_state->enter(machine, new_state, ev);
    }
    new_state = new_state->next;
  }

  return current;
}

State * rbt_send(Machine *machine, State *current, Event ev)
{
  Transition *t = current->transition;

  while(t != NULL) {
    if(strcmp(t->from, ev.name) == 0) {
      return transition_to(machine, current, t, ev);
    }

    t = t->next;
  }

  return current;
}
