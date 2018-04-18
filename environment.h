#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <cstddef>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <queue>
#include <limits>
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

class Node {
  public:
    int x;
    int y;
    float cost;
    Node(int x, int y, float c) : x(x),y(y),cost(c) {}
};

class Environment
{
public:
  Environment();
  ~Environment();

  Agent* get_nearst_agent(pos p, char s);            // Locate nearst agent of sex s
  pos get_nearest_registry(pos p);                    // Return position of the closest registry
  void get_path_to_reg(pos start, pos target, vector<pos> &p);   // Return (?) path to position p
  int free_position(int x, int y);                  // Return wether the position is free of obstacles
  void update_position(Agent* a, int new_x, int new_y);
  void update_position_partner(Agent* a, int new_x, int new_y);
  void set_couple(int x, int y);
  void clean_position(int x, int y);
  void clean_position_partner(int x, int y);
  cell (*get_grid(void))[N][N];
  void print_cell(int x, int y);
  void print_agents();
  int  who_is_happy();
  int  finished();
  void add_walls();
  void add_registries(int number);
  void add_agent(Agent* a);
  int is_agent_here(Agent* a, int x, int y);
  Agent* get_agent(int i);

private:
  cell grid[N][N];              // Main matrix
  // pos  registries[REG_TOTAL];   // Array of registries. Positions here must be
                                // the same in the matrix
  vector<pos> registries;
  vector<pos> walls;
  vector<Agent*> active_ags;     // Vector of active agents
  void astar(cell matrix[][N], pos start, pos goal, vector<pos> &path);
  float h(int x1, int y1, int x2, int y2);
};

#endif // ENVIRONMENT_H
