#ifndef AGENT_H
#define AGENT_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstddef>
#include <time.h>
#include "defines.h"
#include "environment.h"

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
	void print_prefs();
  int init_prefs(vector<int> p);
	void init_position(int x, int y);
	void step();
	pos get_position();
  int marry_me(Agent* proposer);  // Gets asked by other agents

private:
  int propose(Agent* proposed);   // Asks another agent
  char get_opposed_sex();

  ag_states ps;							// Present state
  id my_id;
	pos my_position;
  Agent* partner;           // id of partner if married
	pos reg;									// Registry
  char status;              // SINGLE OR MARRIED
  vector<int> preferences;  // Vector of preferences
  Environment* env;         // Pointer to environment object
	vector<pos> path;					// Vector of positions that lead to the registry
	int walking_pattern;			// Keeps track of the walking pattern through cycles
};

#endif
