#include "environment.h"

Environment::Environment() {

  for (int i=0;i<N+1;i++) {
    for (int j=0;j<N+1;j++) {
      grid[i][j].free = 1;
      grid[i][j].wall = 0;
      grid[i][j].registry = 0;
      grid[i][j].agent = NULL;
    }
  }
}

Environment::~Environment() {

}

Agent* Environment::get_nearst_agent(pos p, char s) {
  int x = p.x;
  int y = p.y;

  for (int i=x-2;i<x+3;i++) {
    for (int j=y-2;j<y-1;j++) {
      if ( i >= 0 && i <= N && j >= 0 && j <= N) {
        if (grid[i][j].agent != NULL && i != p.x && j != p.y)
          if (grid[i][j].agent->get_id().sex == s)
            // Returns the first agent of a different sex
            return grid[i][j].agent;
      }
    }
  }
  return NULL;
}

pos Environment::get_nearst_registry(pos p) {
  // Uses heuristic to return "the closest" registry
  float shorter_dist = N*N; // Big number
  float temp_dist = 0.0;
  pos   closest_pos;

  for (int i=0; i<REG_TOTAL+1; i++) {
    temp_dist = sqrt( pow( (p.x-registries[i].x),2) + pow( (p.y-registries[i].y),2) );
    if (temp_dist < shorter_dist) {
      shorter_dist = temp_dist;
      closest_pos.x = registries[i].x;
      closest_pos.y = registries[i].y;
    }
  }

  return closest_pos;
}

void Environment::get_path_to_reg(pos start, pos target, vector<pos> &p) {
  // Run A* from start to nearest registry
}

int Environment::free_poisition(pos p) {
  return grid[p.x][p.y].free;
}
