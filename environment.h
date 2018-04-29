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
#include <bits/stdc++.h>

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
    float f;
    float g;
    float h;
  Node(){}
  Node(int x, int y, float f, float g, float h) : x(x), y(y), f(f), g(g), h(h) {}
};

class Environment
{
public:
  Environment();
  ~Environment();

  Agent* get_nearst_agent(pos p, char s);                          // Locate nearst agent of sex s
  pos get_nearest_registry(pos p);                                 // Return position of the closest registry
  void get_path_to_reg(pos start, pos target);                     // Return (?) path to position p
  int free_position(int x, int y);                                 // Return wether the position is free of obstacles
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
  stack<pos> path;

private:
  cell grid[N][N];              // Main matrix
  vector<pos> registries;
  vector<pos> walls;
  vector<Agent*> active_ags;     // Vector of active agents

  bool isWithinGrid(int cur_x, int cur_y);
  bool isUnBlocked(cell grid[N][N], int cur_x, int cur_y);
  bool isDestination(int cur_x, int cur_y, int dest_x, int dest_y);
  float h(int x1, int y1, int x2, int y2);
  void tracePath(Node costs[N][N], pos goal);
  void astar(cell grid[N][N], pos start, pos goal);
};

#endif // ENVIRONMENT_H
