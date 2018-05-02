#include "environment.h"

// the top of the priority queue is the greatest element by default,
// but we want the smallest, so flip the sign
// bool operator<(const Node &n1, const Node &n2) {
//   return n1.cost > n2.cost;
// }
// bool operator==(const Node &n1, const Node &n2) {
//   return (n1.x == n2.x && n1.y == n2.y);
// }

bool operator<(const Node &n1, const Node &n2) {
  return n1.f > n2.f;
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
      grid[i][j].couple = 0;
      grid[i][j].agent_partner = NULL;
      grid[i][j].path = 0;
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
    printf("\e[47m - \e[0m");

  } else if (grid[x][y].registry > 0) {
    printf("\e[104m C \e[0m");

  } else if (grid[x][y].wall == 1) {
    printf("\e[101m # \e[0m");

  } else if (grid[x][y].couple == 1) {
    printf("\e[103m %% \e[0m");

  } else if (grid[x][y].couple == 2) {
    printf("\e[105m %c \e[0m", grid[x][y].agent->get_id().sex);

  } else if (grid[x][y].agent != NULL) {
    printf("\e[102m %c \e[0m", grid[x][y].agent->get_id().sex);
  }
}

void Environment::print_grid() {
  // Draw horizontal indexes
  printf("   ");
  for (int i=0; i<N; i++)
    printf("%2d ", i);
    cout << endl;

  // Draw vertical indexes
  for (int j=0; j<N; j++) {
    printf("%2d ", j);
    // Draw the initial grid
    for (int i=0; i<N; i++) {
      print_cell(i,j);
    }
    cout << endl;
  }
}

void Environment::print_agents() {
  int part;
  printf("\n");
  for(int i=0; i<active_ags.size(); i++) {
    if (active_ags[i]->get_partner() != NULL)
      part = active_ags[i]->get_partner()->get_id().name;
    else
      part = -1;
    printf("Agent <%c,%2d> @ %2d,%2d | status = %d | prefs: %d > %d > %d | partner: %2d | state: %d | reg: %2d,%2d\n",
      active_ags[i]->get_id().sex, active_ags[i]->get_id().name,
      active_ags[i]->get_position().x, active_ags[i]->get_position().y,
      active_ags[i]->get_status(),
      active_ags[i]->get_pref_at(0),
      active_ags[i]->get_pref_at(1),
      active_ags[i]->get_pref_at(2),
      part,
      active_ags[i]->get_state(),
      active_ags[i]->get_registry().x,active_ags[i]->get_registry().y);

  }
}

int Environment::who_is_happy() {
  int total = 0;
  for (int i=0;i<active_ags.size(); i++) {
    if (active_ags[i]->get_partner() != NULL) {
      if (active_ags[i]->get_pref_at(0) == active_ags[i]->get_partner()->get_id().name) {
        total ++;
        printf("Agent <%c,%d> is happy with <%c,%d>.\n",
                  active_ags[i]->get_id().sex, active_ags[i]->get_id().name,
                  active_ags[i]->get_partner()->get_id().sex, active_ags[i]->get_partner()->get_id().name);
      }
    }
  }
  printf("Total of happy agents %d\n", total);
  return total;
}

int Environment::finished() {
   int total = 0;
   for (int i=0;i<active_ags.size(); i++) {
	    if (active_ags[i]->get_state() == WANDER_M)
	     total++;
   }
  //  printf("TOTAL AGS MARRIED -> %d and %ld ACTIVE\n", total, active_ags.size());
   return (total == active_ags.size());
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
      pos p;
      p.x = x;
      p.y = y;
      walls.push_back(p);
    }
  }
}

void Environment::add_registries(int number) {
  const float INF = std::numeric_limits<float>::infinity();
  // Registries should be close to walls
  for(int i=0; i<number; i++) {
    for(;;){
      int x = rand() % (N-1);
      int y = rand() % (N-1);

      printf("Values of X and Y of registries: %d %d\n", x, y);

      if (grid[x][y].free == 1) {
        grid[x][y].free = INF;
        grid[x][y].registry = i+1;
        pos p;
        p.x = x;
        p.y = y;
        registries.push_back(p);
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
      active_ags.push_back(a);
      grid[x][y].agent = active_ags.back();
      break;
    }
  }
}

Agent* Environment::get_agent(int i) {
  return active_ags.at(i);
}

stack<pos> Environment::get_path(){
  return path;
}

void Environment::clear_path(stack<pos> pathDone){
  pos p;

  while(!pathDone.empty()){
    p = pathDone.top();
    grid[p.x][p.y].path = 0;
    pathDone.pop();
  }
}

Agent* Environment::get_nearest_agent(pos p, char s) {
  int x = p.x;
  int y = p.y;

  for (int i=x-2;i<x+3;i++) {
    for (int j=y-2;j<y+3;j++) {
      if ( i >= 0 && i < N && j >= 0 && j < N) {
        // printf("Visting %d,%d\n", i,j);
        if (grid[i][j].agent != NULL && i != p.x && j != p.y)
          if (grid[i][j].agent->get_id().sex == s)
            // Returns the first agent of sex s
            return grid[i][j].agent;
      }
    }
  }
  return NULL;
}

pos Environment::get_nearest_registry(pos p) {
  // Uses heuristic to return "the closest" registry
  float shorter_dist = N*N; // Big number
  float temp_dist = 0.0;
  pos   closest_pos;

  for (int i=0; i<registries.size(); i++) {
    temp_dist = sqrt( pow( ((float)p.x-(float)registries.at(i).x),2.0) + pow( ((float)p.y-(float)registries.at(i).y),2.0) );
    // printf("Registry #%d @ %d,%d is %f cells from %d,%d\n", i, registries.at(i).x,
        // registries.at(i).y, temp_dist, p.x, p.y);
    if (temp_dist < shorter_dist) {
      shorter_dist = temp_dist;
      closest_pos.x = registries.at(i).x;
      closest_pos.y = registries.at(i).y;
    }
  }

  return closest_pos;
}

int Environment::free_position(int x, int y) {
  const float INF = std::numeric_limits<float>::infinity();

  if (x >= 0 && x < N && y >= 0 && y < N)
    return grid[x][y].free;
  else
    return INF;
}

void Environment::update_position(Agent* a, int new_x, int new_y) {
  const float INF = std::numeric_limits<float>::infinity();

  grid[a->get_position().x][a->get_position().y].agent = NULL;
  grid[a->get_position().x][a->get_position().y].free = 1;
  grid[a->get_position().x][a->get_position().y].couple = 0;

  if( a->get_status() == TAKEN) {
    grid[new_x][new_y].couple = 2;
  }

  if( a->get_status() == MARRIED) {
    grid[new_x][new_y].couple = 1;
  }

  grid[new_x][new_y].free = INF;
  grid[new_x][new_y].agent = a;
}

void Environment::update_position_partner(Agent* a, int new_x, int new_y) {
  const float INF = std::numeric_limits<float>::infinity();

  grid[a->get_position().x][a->get_position().y].free = 1;
  grid[a->get_position().x][a->get_position().y].couple = 0;
  grid[a->get_position().x][a->get_position().y].agent_partner = NULL;

  if( a->get_status() == TAKEN) {
    grid[new_x][new_y].couple = 2;
  }

  if( a->get_status() == MARRIED) {
    grid[new_x][new_y].couple = 1;
  }

  grid[new_x][new_y].free = INF;
  grid[new_x][new_y].agent_partner = a;
}

void Environment::clean_position(int x, int y) {
  grid[x][y].free = 1;
  grid[x][y].agent = NULL;
}

void Environment::clean_position_partner(int x, int y){
  grid[x][y].agent_partner = NULL;
  grid[x][y].couple = 0;
}

void Environment::set_couple(int x, int y) {
  grid[x][y].couple = 1;
}

int Environment::is_agent_here(Agent* a, int x, int y) {
  for (int i=x-1;i<x+2;i++) {
    for (int j=y-1;j<y+2;j++) {
      if ( i >= 0 && i < N && j >= 0 && j < N) {
        //printf("Looking for <%c,%d> at %d,%d around %d,%d\n", a->get_id().sex, a->get_id().name, i, j, x, y);
        if( grid[i][j].agent == a || grid[i][j].agent_partner == a)
          return 1;
        }
    }
  }
  return 0;
}

void Environment::get_path_to_reg(pos start, pos target) {
  // Run A* from start to nearest registry
  astar(grid, start, target);
}



bool Environment::isWithinGrid(int cur_x, int cur_y){
  if(cur_x >= 0 && cur_x < N && cur_y >=0 && cur_y < N){
    return true;
  }
  else return false;
}

bool Environment::isUnBlocked(cell grid[N][N], int cur_x, int cur_y){
  // Ignore cells that are not free (occupied by walls, agents or registries)
  // or that are currently being locked for a path (to avoid two agents using the same path)
  if(grid[cur_x][cur_y].free == 1 && grid[cur_x][cur_y].path == 0){
    return true;
  }
  else return false;
}

bool Environment::isDestination(int cur_x, int cur_y, int dest_x, int dest_y){
  if(cur_x == dest_x && cur_y == dest_y){
    return true;
  }
  else return false;
}

void Environment::tracePath(Node costs[N][N], pos goal){

  //printf("\nThe path from (A) to target (C) is \n");
  int row = goal.x;
  int col = goal.y;

  pos p_temp;
  pos p;
  p_temp.x = 0;
  p_temp.y = 0;

  while(!(costs[row][col].x == row && costs[row][col].y == col)){

    p_temp.x = row;
    p_temp.y = col;
    path.push(p_temp);
    int temp_x = costs[row][col].x;
    int temp_y = costs[row][col].y;
    //grid[row][col].free = 0;
    grid[row][col].path = 1;
    row = temp_x;
    col = temp_y;
  }

  p_temp.x = row;
  p_temp.y = col;
  path.push(p_temp);

  // while(!path.empty()){
  //   p = path.top();
  //   printf("-> (%d,%d) \n", p.x, p.y);
  //   path.pop();
  // }

  return;
}

float Environment::h(int x1, int y1, int x2, int y2) {
  return sqrt( pow( (x2-x1),2) + pow( (y2-y1),2) );
}

void Environment::astar(cell grid[N][N], pos start, pos goal) {

    // Pre-checks
    if(!isWithinGrid(start.x, start.y)){
      printf("Source is invalid\n");
      return;
    }

    if(!isWithinGrid(goal.x, goal.y)){
      printf("Target is invalid\n");
      return;
    }

    if(isDestination(start.x, start.y, goal.x, goal.y)){
      printf("We are already at the destination!");
      return;
    }

    // Create a closed list and initialise it to false which means
    // that no cell has been visited yet
    // This closed list is implemented as a boolean 2D array
    bool closedList[N][N];
    memset(closedList, false, sizeof (closedList));


    // 2D array to hold the node cost details
    Node costs[N][N];

    for(int i=0; i < N; i++){
      for(int j=0; j < N; j++){
        costs[i][j].x = i;
        costs[i][j].y = j;
        costs[i][j].f = 1000;
        costs[i][j].g = 1000;
        costs[i][j].h = 1000;
      }
    }

    // Initializing the start and goal node
    Node start_node(start.x, start.y, 0., 0., 0.);
    Node goal_node(goal.x, goal.y, 0., 0., 0.);

    priority_queue<Node> nodes_to_visit;

    nodes_to_visit.push(start_node);

    bool path_found = false;
    while(!nodes_to_visit.empty()){

      //Copy the first node and removes it from the queue
      Node cur = nodes_to_visit.top();

      nodes_to_visit.pop();

      // Check neighbors for free cells, bounds and walls
      // The order of check is top to bottom, left to right

      float gNew, hNew, fNew;
      gNew = hNew = fNew = 0.;
      int x = cur.x;
      int y = cur.y;

      // Already visited the current node
      closedList[x][y] = true;

      //NW
      if(isWithinGrid(x-1, y-1) == true){
        //printf("\nNW - ");
        if(isDestination(x-1, y-1, goal.x, goal.y)){
          // Set the parent cell of the current cell - destination found!
          costs[x-1][y-1].x = x;
          costs[x-1][y-1].y = y;
          printf("The destination cell is found\n");
          tracePath(costs,goal);
          path_found = true;
          return;
        }
        // If node has been already visited (i.e closedList[x][y] = true), then do nothing
        // Else, do the below
        else if(closedList[x-1][y-1] == false && isUnBlocked(grid, x-1, y-1)){
          gNew = costs[x][y].g + 1.0;
          hNew = h(x, y, x-1, y-1);
          fNew = gNew + hNew;
          if(costs[x-1][y-1].f == 1000 || costs[x-1][y-1].f > fNew){
            //printf(" Current (%d,%d) - Checking (%d,%d) | gNew = %2.2f | hNew = %2.2f | fNew = %2.2f\n", x, y, x-1, y-1, gNew, hNew, fNew);

            nodes_to_visit.push(Node(x-1, y-1, fNew, gNew, hNew));

            costs[x-1][y-1].x = x;
            costs[x-1][y-1].y = y;
            costs[x-1][y-1].f = fNew;
            costs[x-1][y-1].g = gNew;
            costs[x-1][y-1].h = hNew;

          }
        }
      }

      //N
      if(isWithinGrid(x  , y-1) == true){
        //printf("\nN  - ");
        if(isDestination(x, y-1, goal.x, goal.y)){
          // Set the parent cell of the current cell - destination found!
          costs[x][y-1].x = x;
          costs[x][y-1].y = y;
          printf("The destination cell is found\n");
          tracePath(costs,goal);
          path_found = true;
          return;
        }
        // If node has been already visited (i.e closedList[x][y] = true), then do nothing
        // Else, do the below
        else if(closedList[x][y-1] == false && isUnBlocked(grid, x, y-1)){
          gNew = costs[x][y].g + 1.0;
          hNew = h(x, y, x, y-1);
          fNew = gNew + hNew;

          if(costs[x][y-1].f == 1000 || costs[x][y-1].f > fNew){
            //printf(" Current (%d,%d) - Checking (%d,%d) | gNew = %2.2f | hNew = %2.2f | fNew = %2.2f\n", x, y, x, y-1, gNew, hNew, fNew);
            nodes_to_visit.push(Node(x, y-1, fNew, gNew, hNew));

            costs[x][y-1].x = x;
            costs[x][y-1].y = y;
            costs[x][y-1].f = fNew;
            costs[x][y-1].g = gNew;
            costs[x][y-1].h = hNew;

          }
        }
      }
      //NE
      if(isWithinGrid(x+1, y-1) == true){
        //printf("\nNE - ");
        if(isDestination(x+1, y-1, goal.x, goal.y)){
          // Set the parent cell of the current cell - destination found!
          costs[x+1][y-1].x = x;
          costs[x+1][y-1].y = y;
          printf("The destination cell is found\n");
          tracePath(costs,goal);
          path_found = true;
          return;
        }
        // If node has been already visited (i.e closedList[x][y] = true), then do nothing
        // Else, do the below
        else if(closedList[x+1][y-1] == false && isUnBlocked(grid, x+1, y-1)){
          gNew = costs[x][y].g + 1.0;
          hNew = h(x, y, x+1, y-1);
          fNew = gNew + hNew;

          if(costs[x+1][y-1].f == 1000 || costs[x+1][y-1].f > fNew){
            //printf(" Current (%d,%d) - Checking (%d,%d) | gNew = %2.2f | hNew = %2.2f | fNew = %2.2f\n", x, y, x+1, y-1, gNew, hNew, fNew);
            nodes_to_visit.push(Node(x+1, y-1, fNew, gNew, hNew));

            costs[x+1][y-1].x = x;
            costs[x+1][y-1].y = y;
            costs[x+1][y-1].f = fNew;
            costs[x+1][y-1].g = gNew;
            costs[x+1][y-1].h = hNew;
          }
        }
      }
      //W
      if(isWithinGrid(x-1, y)   == true){
        //printf("\nW  - ");
        if(isDestination(x-1, y, goal.x, goal.y)){
          // Set the parent cell of the current cell - destination found!
          costs[x-1][y].x = x;
          costs[x-1][y].y = y;
          printf("The destination cell is found\n");
          tracePath(costs,goal);
          path_found = true;
          return;
        }
        // If node has been already visited (i.e closedList[x][y] = true), then do nothing
        // Else, do the below
        else if(closedList[x-1][y] == false && isUnBlocked(grid, x-1, y)){
          gNew = costs[x][y].g + 1.0;
          hNew = h(x, y, x-1, y);
          fNew = gNew + hNew;

          if(costs[x-1][y].f == 1000 || costs[x-1][y].f > fNew){
            //printf(" Current (%d,%d) - Checking (%d,%d) | gNew = %2.2f | hNew = %2.2f | fNew = %2.2f\n", x, y, x-1, y, gNew, hNew, fNew);
            nodes_to_visit.push(Node(x-1, y, fNew, gNew, hNew));

            costs[x-1][y].x = x;
            costs[x-1][y].y = y;
            costs[x-1][y].f = fNew;
            costs[x-1][y].g = gNew;
            costs[x-1][y].h = hNew;

          }
        }
      }
      //E
      if(isWithinGrid(x+1, y)   == true){
        //printf("\nE  - ");
        if(isDestination(x+1, y, goal.x, goal.y)){
          // Set the parent cell of the current cell - destination found!
          costs[x+1][y].x = x;
          costs[x+1][y].y = y;
          printf("The destination cell is found\n");
          tracePath(costs,goal);
          path_found = true;
          return;
        }
        // If node has been already visited (i.e closedList[x][y] = true), then do nothing
        // Else, do the below
        else if(closedList[x+1][y] == false && isUnBlocked(grid, x+1, y)){
          gNew = costs[x][y].g + 1.0;
          hNew = h(x, y, x+1, y);
          fNew = gNew + hNew;
          if(costs[x+1][y].f == 1000 || costs[x+1][y].f > fNew){
            //printf(" Current (%d,%d) - Checking (%d,%d) | gNew = %2.2f | hNew = %2.2f | fNew = %2.2f\n", x, y, x+1, y, gNew, hNew, fNew);
            nodes_to_visit.push(Node(x+1, y, fNew, gNew, hNew));

            costs[x+1][y].x = x;
            costs[x+1][y].y = y;
            costs[x+1][y].f = fNew;
            costs[x+1][y].g = gNew;
            costs[x+1][y].h = hNew;

          }
        }
      }
      //SW
      if(isWithinGrid(x-1, y+1) == true){
        //printf("\nSW - ");
        if(isDestination(x-1, y+1, goal.x, goal.y)){
          // Set the parent cell of the current cell - destination found!
          costs[x-1][y+1].x = x;
          costs[x-1][y+1].y = y;
          printf("The destination cell is found\n");
          tracePath(costs,goal);
          path_found = true;
          return;
        }
        // If node has been already visited (i.e closedList[x][y] = true), then do nothing
        // Else, do the below
        else if(closedList[x-1][y+1] == false && isUnBlocked(grid, x-1, y+1)){
          gNew = costs[x][y].g + 1.0;
          hNew = h(x, y, x-1, y+1);
          fNew = gNew + hNew;
          if(costs[x-1][y+1].f == 1000 || costs[x-1][y+1].f > fNew){
            //printf(" Current (%d,%d) - Checking (%d,%d) | gNew = %2.2f | hNew = %2.2f | fNew = %2.2f\n", x, y, x-1, y+1, gNew, hNew, fNew);
            nodes_to_visit.push(Node(x-1, y+1, fNew, gNew, hNew));

            costs[x-1][y+1].x = x;
            costs[x-1][y+1].y = y;
            costs[x-1][y+1].f = fNew;
            costs[x-1][y+1].g = gNew;
            costs[x-1][y+1].h = hNew;

          }
        }
      }
      //S
      if(isWithinGrid(x,   y+1) == true){
        //printf("\nS  - ");
        if(isDestination(x, y+1, goal.x, goal.y)){
          // Set the parent cell of the current cell - destination found!
          costs[x][y+1].x = x;
          costs[x][y+1].y = y;
          printf("The destination cell is found\n");
          tracePath(costs,goal);
          path_found = true;
          return;
        }
        // If node has been already visited (i.e closedList[x][y] = true), then do nothing
        // Else, do the below
        else if(closedList[x][y+1] == false && isUnBlocked(grid, x, y+1)){
          gNew = costs[x][y].g + 1.0;
          hNew = h(x, y, x, y+1);
          fNew = gNew + hNew;

          if(costs[x][y+1].f == 1000 || costs[x][y+1].f > fNew){
            //printf(" Current (%d,%d) - Checking (%d,%d) | gNew = %2.2f | hNew = %2.2f | fNew = %2.2f\n", x, y, x, y+1, gNew, hNew, fNew);
            nodes_to_visit.push(Node(x, y+1, fNew, gNew, hNew));

            costs[x][y+1].x = x;
            costs[x][y+1].y = y;
            costs[x][y+1].f = fNew;
            costs[x][y+1].g = gNew;
            costs[x][y+1].h = hNew;

          }
        }
      }
      //SE
      if(isWithinGrid(x+1, y+1) == true){
        //printf("\nSE - ");
        if(isDestination(x+1, y+1, goal.x, goal.y)){
          // Set the parent cell of the current cell - destination found!
          costs[x+1][y+1].x = x;
          costs[x+1][y+1].y = y;
          printf("The destination cell is found\n");
          tracePath(costs,goal);
          path_found = true;
          return;
        }
        // If node has been already visited (i.e closedList[x][y] = true), then do nothing
        // Else, do the below
        else if(closedList[x+1][y+1] == false && isUnBlocked(grid, x+1, y+1)){
          gNew = costs[x][y].g + 1.0;
          hNew = h(x, y, x+1, y+1);
          fNew = gNew + hNew;

          if(costs[x+1][y+1].f == 1000 || costs[x+1][y+1].f > fNew){
            //printf(" Current (%d,%d) - Checking (%d,%d) | gNew = %2.2f | hNew = %2.2f | fNew = %2.2f\n", x, y, x+1, y+1, gNew, hNew, fNew);
            nodes_to_visit.push(Node(x+1, y+1, fNew, gNew, hNew));

            costs[x+1][y+1].x = x;
            costs[x+1][y+1].y = y;
            costs[x+1][y+1].f = fNew;
            costs[x+1][y+1].g = gNew;
            costs[x+1][y+1].h = hNew;
          }
        }
      }

    }

    // Could not find any paths to destination
    if(path_found == false){
      printf("Failed to find the destination cell\n");
    }

    return;
  }
