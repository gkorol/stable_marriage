#ifndef AGENT_H
#define AGENT_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "defines.h"

using namespace std;

class Agent
{
public:
	Agent(char sex, int name);
	~Agent();
  void run();   // Run the FSM
  id get_id();
  void init(int x, int y, environment* env);
  int marry_me(id proposer);  // Gets asked by other agents

private:
  int propose(id proposed);   // Asks another agent
  char get_opposed_sex();

  ag_states ps;
  id my_id;
  id partner;               // id of partner if married
  char status;              // SINGLE OR MARRIED
  vector<int> preferences;  // Vector of preferences
  environment* env;         // Pointer to environment object
  char engaged;             // Signal for next cycle that proposal was accepted
};

#endif
