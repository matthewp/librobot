#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct Event
{
  void * data;
  char * name;
} Event;

typedef bool (GuardFunction)(void* data, Event ev);
typedef void (MutateFunction)(void* data, Event ev);

typedef struct Guard
{
  GuardFunction *fn;
  struct Guard *next;
} Guard;

typedef struct Mutater
{
  MutateFunction *fn;
  struct Mutater *next;
} Mutater;

typedef struct Transition
{
  char *from;
  char *to;
  struct Transition *next;
  Guard *guard;
  Mutater *mutater;
} Transition;

typedef struct State
{
  char *name;
  Transition *transition;
  struct State *next;
} State;

typedef struct Machine
{
  State *initial;
  void * data;
} Machine;

Transition rbt_transition(char *from, char *to);
Transition * rbt_add_guard(Transition *t, GuardFunction *guard_function);
Transition * rbt_add_mutate(Transition *t, MutateFunction *mutate_function);
Machine * rbt_add_data(Machine *machine, void* data);

State rbt_state(char *name, Transition transitions[], size_t n);
State rbt_final(char *name);

Machine rbt_machine(State states[], size_t n);
void rbt_machine_cleanup(Machine *machine);

State * rbt_send(Machine *machine, State *current, Event ev);