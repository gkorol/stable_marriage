#ifndef AGENT_H
#define AGENT_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstddef>
#include "defines.h"
#include "environment.h"

using namespace std;

class Environment;

class Agent
{
public:
	Agent(char sex, int name, int x, int y, Environment* e);
	~Agent();
  void run();   // Run the FSM
  id get_id();
  int init();
  int marry_me(id proposer);  // Gets asked by other agents

private:
  int propose(Agent* proposed);   // Asks another agent
  char get_opposed_sex();

  ag_states ps;							// Present state
  id my_id;
	pos my_position;
  id partner;               // id of partner if married
  char status;              // SINGLE OR MARRIED
  vector<int> preferences;  // Vector of preferences
  Environment* env;         // Pointer to environment object
  char engaged;             // Signal for next cycle that proposal was accepted
	vector<pos> path;					// Vector of positions that lead to the registry
};

#endif
