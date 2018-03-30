#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <cstddef>
#include <math.h>
#include "defines.h"
#include "agent.h"

// Grid indexes:
// (x,y)
//
// 0,0  1,0  2,0 ...
// 0,1  1,1  2,1 ...
// 0,2  1,2  2,2 ...
// ...  ...  ...

using namespace std;

class Environment
{
public:
  Environment();
  ~Environment();

  Agent* get_nearst_agent(pos p, char s);            // Locate nearst agent of sex s
  pos get_nearst_registry(pos p);                    // Return position of the closest registry
  void get_path_to_reg(pos start, pos target, vector<pos> &p);   // Return (?) path to position p
  int free_poisition(pos p);                         // Return wether the position is free of obstacles

private:
  cell grid[N][N];              // Main matrix
  pos  registries[REG_TOTAL];   // Array of registries. Positions here must be
                                // the same in the matrix
};

#endif // ENVIRONMENT_H
