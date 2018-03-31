#include "environment.h"

// the top of the priority queue is the greatest element by default,
// but we want the smallest, so flip the sign
bool operator<(const Node &n1, const Node &n2) {
  return n1.cost > n2.cost;
}
bool operator==(const Node &n1, const Node &n2) {
  return (n1.x == n2.x && n1.y == n2.y);
}

Environment::Environment() {
  static bool seeded = false;

  if(!seeded) {
      srand(time(NULL));
      seeded = true;
  }

  for (int i=0;i<N;i++) {
    for (int j=0;j<N;j++) {
      grid[i][j].free = 1;
      grid[i][j].wall = 0;
      grid[i][j].registry = 0;
      grid[i][j].agent = NULL;
    }
  }
}

Environment::~Environment() {

}

cell (*Environment::get_grid(void))[N][N] {
  return &grid;
}

void Environment::print_cell(int x, int y) {
  if(grid[x][y].free == 1){
    printf(" _ ");
  } else if (grid[x][y].registry > 0) {
    printf("\e[34m C \e[0m");
  } else if (grid[x][y].wall == 1) {
    printf("\e[91m # \e[0m");
  } else if (grid[x][y].agent != NULL) {
    printf("\e[92m %c \e[0m", grid[x][y].agent->get_id().sex);
  }
}

void Environment::add_walls() {
  const float INF = std::numeric_limits<float>::infinity();
  // ~ N/5 Ex; 20x20 -> 4 walls
  // Walls from N/5 to N/2 cells
  int number = ceil(N/5);

  for( int i=1; i<=number; i++) {
    int size = rand() % (int)ceil(N/2) + (int)ceil(N/5);
    int x = rand() % (N-1); // any x from 0 to N-1
    int y = rand() % (N-size); // any y from 0 to N-size
    for( int j=0; j<size; j++){
      grid[x][y+j].free = INF;
      grid[x][y+j].wall = 1;
    }
  }
}

void Environment::add_registries(int number) {
  const float INF = std::numeric_limits<float>::infinity();

  for( int i=0; i<number; i++) {
    for(;;){
      int x = rand() % (N-1);
      int y = rand() % (N-1);
      if (grid[x][y].free == 1) {
        grid[x][y].free = INF;
        grid[x][y].registry = i+1;
        break;
      }
    }
  }
}

void Environment::add_agent(Agent* a) {
  const float INF = std::numeric_limits<float>::infinity();

  for(;;){
    int x = rand() % (N-1);
    int y = rand() % (N-1);
    if (grid[x][y].free == 1) {
      a->init_position(x,y);
      grid[x][y].free = INF;
      grid[x][y].agent = a;
      active_ags.push_back(a);
      break;
    }
  }
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

int Environment::free_poisition(pos p) {
  return grid[p.x][p.y].free;
}

void Environment::get_path_to_reg(pos start, pos target, vector<pos> &p) {
  // Run A* from start to nearest registry
  astar(grid, start, target, p);
}

float Environment::h(int x1, int y1, int x2, int y2) {
  return sqrt( pow( (x1-x2),2) + pow( (y1-y2),2) );
}

void Environment::astar(cell matrix[][N], pos start, pos goal, vector<pos> &path) {

  const float INF = std::numeric_limits<float>::infinity();

  Node start_node(start.x, start.y, 0.);
  Node goal_node(goal.x, goal.y, 0.);

  float** costs = new float*[N];
  for(int i = 0; i < N; ++i) {
      costs[i] = new float[N];
  }
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      costs[i][j] = INF;

  costs[start.x][start.y] = 0.0;

  std::priority_queue<Node> nodes_to_visit;
  nodes_to_visit.push(start_node);

  int* nbrs = new int[8]; // neighbor cells
  int* nbrs_x = new int[8];
  int* nbrs_y = new int[8];

  bool solution_found = false;
  while (!nodes_to_visit.empty()) {
    Node cur = nodes_to_visit.top();

    if (cur == goal_node) {
      solution_found = true;
      break;
    }

    nodes_to_visit.pop();

    // check for free cells, bounds and walls arouond
    if (cur.x-1 >= 0 && cur.x-1 < N && cur.y-1 >= 0 && cur.y-1 < N &&
      matrix[cur.x-1][cur.y-1].free == 1) {
        nbrs[0] = 1;
        nbrs_x[0] = cur.x-1;
        nbrs_y[0] = cur.y-1;
      } else {
        nbrs[0] = -1;
        nbrs_x[0] = -1;
        nbrs_y[0] = -1;
      }

    if (cur.x-1 >= 0 && cur.x-1 < N && cur.y   >= 0 && cur.y   < N &&
      matrix[cur.x-1][cur.y].free == 1) {
        nbrs[1] = 1;
        nbrs_x[1] = cur.x-1;
        nbrs_y[1] = cur.y;
      } else {
        nbrs[1] = -1;
        nbrs_x[1] = -1;
        nbrs_y[1] = -1;
      }

    if (cur.x+1 >= 0 && cur.x+1 < N && cur.y-1 >= 0 && cur.y-1 < N &&
      matrix[cur.x+1][cur.y-1].free == 1) {
        nbrs[2] = 1;
        nbrs_x[2] = cur.x+1;
        nbrs_y[2] = cur.y-1;
      } else {
        nbrs[2] = -1;
        nbrs_x[2] = -1;
        nbrs_y[2] = -1;
      }

    if (cur.x-1 >= 0 && cur.x-1 < N && cur.y >= 0 && cur.y < N &&
      matrix[cur.x-1][cur.y].free == 1) {
        nbrs[3] = 1;
        nbrs_x[3] = cur.x-1;
        nbrs_y[3] = cur.y;
      } else {
        nbrs[3] = -1;
        nbrs_x[3] = -1;
        nbrs_y[3] = -1;
      }

    if (cur.x+1 >= 0 && cur.x+1 < N && cur.y >= 0 && cur.y < N &&
      matrix[cur.x+1][cur.y].free == 1) {
        nbrs[4] = 1;
        nbrs_x[4] = cur.x+1;
        nbrs_y[4] = cur.y;
      } else {
        nbrs[4] = -1;
        nbrs_x[4] = -1;
        nbrs_y[4] = -1;
      }

    if (cur.x-1 >= 0 && cur.x-1 < N && cur.y+1 >= 0 && cur.y+1 < N &&
      matrix[cur.x-1][cur.y+1].free == 1) {
        nbrs[5] = 1;
        nbrs_x[5] = cur.x-1;
        nbrs_y[5] = cur.y+1;
      } else {
        nbrs[5] = -1;
        nbrs_x[5] = -1;
        nbrs_y[5] = -1;
      }

    if (cur.x >= 0 && cur.x < N && cur.y+1 >= 0 && cur.y+1 < N &&
      matrix[cur.x][cur.y+1].free == 1) {
        nbrs[6] = 1;
        nbrs_x[6] = cur.x;
        nbrs_y[6] = cur.y+1;
      } else {
        nbrs[6] = -1;
        nbrs_x[6] = -1;
        nbrs_y[6] = -1;
      }

    if (cur.x+1 >= 0 && cur.x+1 < N && cur.y+1 >= 0 && cur.y+1 < N &&
      matrix[cur.x+1][cur.y+1].free == 1) {
        nbrs[7] = 1;
        nbrs_x[7] = cur.x-1;
        nbrs_y[7] = cur.y+1;
      } else {
        nbrs[7] = -1;
        nbrs_x[7] = -1;
        nbrs_y[7] = -1;
      }

    float heuristic_cost;
    for (int i = 0; i < 8; ++i) {
      if (nbrs[i] >= 0) {
        // Given the cost of 1 for each move
        float new_cost = costs[cur.x][cur.y] + 1;
        if (new_cost < costs[nbrs_x[i]][nbrs_y[i]]) {
            heuristic_cost = h(nbrs_x[i],nbrs_y[i],goal.x,goal.y);

          // paths with lower expected cost are explored first
          float priority = new_cost + heuristic_cost;
          nodes_to_visit.push(Node(nbrs_x[i], nbrs_y[i], priority));

          costs[nbrs_x[i]][nbrs_y[i]] = new_cost;
          pos p;
          p.x = cur.x;
          p.y = cur.y;
          path.push_back(p);
        }
      }
    }
  }

  for(int i = 0; i < N; ++i) {
    delete [] costs[i];
  }
  delete [] costs;
  delete [] nbrs;
  delete [] nbrs_x;
  delete [] nbrs_y;

  return;
}
