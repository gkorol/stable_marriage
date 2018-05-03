#ifndef AGENT_H
#define AGENT_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <time.h>
#include "defines.h"
#include "environment.h"
#include <bits/stdc++.h>

using namespace std;

class Environment;

class Agent
{
public:
	Agent(char sex, int name, Environment* e);
	~Agent();
  void run();   // Run the FSM
  id get_id();
	char get_status();
	Agent* get_partner();
	void print_prefs();
	int get_pref_at(int i);
  int init_prefs(vector<int> p);
	void init_position(int x, int y);
	void step();
	pos get_position();
  int marry_me(Agent* proposer);  // Gets asked by other agents
	void divorce_me();
	int get_state();
	void set_register(Agent* partner, pos reg);
	pos get_registry();

private:
  char get_opposed_sex();
	int greater_pref(Agent* a);

  ag_states ps;							// Present state
  id my_id;
	pos my_position;
  Agent* partner;           // id of partner if married
	int new_partner;					// Need to get divorce
	int asked_divorce;				// partner asked divorce
	pos reg;									// Registry
  char status;              // SINGLE, TAKEN or MARRIED
  vector<int> preferences;  // Vector of preferences
  Environment* env;         // Pointer to environment object
	stack<pos> PrivatePath;		// Vector of positions that lead to the registry
	int walking_pattern;			// Keeps track of the walking pattern through cycles
};

#endif
