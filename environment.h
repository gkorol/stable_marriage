#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "defines.h"

using namespace std;

class Environment
{
public:
  Environment(int n);
  ~Environment();

  id get_nearst_agent(pos p, char s);            // Locate nearst agent of sex s
  pos get_nearst_registry(pos p);     // Return position of the closest registry
  void get_path_to_reg(pos p);                  // Return (?) path to position p
  int free_poisition(pos p);  // Return wether the position is free of obstacles

private:

  int n;
  cell[n][n] grid;
};

#endif // ENVIRONMENT_H
