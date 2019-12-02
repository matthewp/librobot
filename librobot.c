#include <string.h>
#include <stdbool.h>

typedef void *Data;

typedef struct Event
{
  void * data;
  char * name;
} Event;

typedef bool (*GuardFunction)(Data *data, Event ev);

typedef struct Guard
{
  GuardFunction fn;
  struct Guard *next;
} Guard;

typedef struct Transition
{
  char *from;
  char *to;
  struct Transition *next;
  Guard guard;
} Transition;

typedef struct State
{
  char *name;
  Transition *transition;
  struct State *next;
} State;

typedef struct Machine
{
  State *current;
  State *initial;
} Machine;

static bool default_guard(Data *data, Event ev)
{
  return true;
}

Transition rbt_transition(char *from, char *to)
{
  Transition t = {
    .from = from,
    .to = to,
    .guard = (Guard) {
      .fn = &default_guard,
      .next = NULL
    }
  };

  return t;
}

Transition * rbt_add_guard(Transition *t)
{
  // TODO not implemented
  return NULL;
}

State rbt_state(char *name, Transition transitions[], size_t n)
{
  State s = {.name = name};

  int i = 0;
  Transition *last = NULL;

  while(i < n) {
    Transition t = transitions[i];
    t.next = last;

    i++;
  }

  s.transition = &transitions[0];

  return s;
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
  m.current = &states[0];

  return m;
}

State* rbt_send(Machine *machine, State *current, char *name)
{
  Transition *t = current->transition;

  while(t != NULL)
  {
    if(strcmp(t->from, name) == 0)
    {
      char *new_state_name = t->to;
      State *new_state = (*machine).initial;

      //printf("New state name %s\n", new_state_name);

      while(new_state != NULL)
      {
        if(strcmp(new_state->name, new_state_name) == 0)
        {
          return new_state;
        }
        new_state = new_state->next;
      }
    }

    t = NULL;
  }

  return current;
}
