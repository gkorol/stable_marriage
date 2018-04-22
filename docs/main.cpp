/*
To complie: g++ main.cpp -o astar
Souce: https://www.geeksforgeeks.org/a-search-algorithm/
Modified to reflect the scenario of stable_marriage
*/

#include <iostream>
#include <stdio.h>
#include <cstddef>
#include <string>
#include <math.h>
#include <time.h>
#include <queue>
#include <limits>
#include<bits/stdc++.h>

// Grid size
#define N 15
#define MALE 'm'
#define FEMALE 'f'

using namespace std;

struct pos{
  unsigned int x;
  unsigned int y;
};

struct cell{
  int free;
  int wall;
  int registry;
  int agent;
};

class node{
  public:
    int x;
    int y;
    float f;
    float g;
    float h;
  node(){}
  node(int x, int y, float f, float g, float h) : x(x), y(y), f(f), g(g), h(h) {}
};

// Operator overflow to be used on the Astar priority queue
// This is required because we are not using a default type in
// priority queue (node), therefore, we need to provide the
// updated operators in order to allow the comparisson and sort to happen
bool operator<(const node &n1, const node &n2) {
  return n1.f > n2.f;
}

bool operator==(const node &n1, const node &n2) {
  return (n1.x == n2.x && n1.y == n2.y);
}

void print_grid(cell grid[N][N]);
void print_cell(int x, int y, cell grid[N][N]);
float h(int x1, int y1, int x2, int y2);
bool isWithinGrid(int cur_x, int cur_y);
bool isUnBlocked(cell grid[N][N], int cur_x, int cur_y);
bool isDestination(int cur_x, int cur_y, int dest_x, int dest_y);
void tracePath(node costs[N][N], pos goal);
void astar(cell grid[N][N], pos start, pos goal);


/*MAIN PROGRAM*/
int main(){
  cell grid[N][N];
  pos start;
  pos goal;
  vector<pos> path;

  printf("Hello world! \n");
  // Initialize the grid
  for(int i=0;i<N;i++){
    for(int j=0;j<N;j++){
      grid[i][j].free = 1;
      grid[i][j].wall = 0;
      grid[i][j].registry = 0;
      grid[i][j].agent = 0;
    }
  }

  start.x = 13;
  start.y = 1;
  goal.x = 8;
  goal.y = 4;

  for(int i=0;i<9;i++){
    grid[6][i].free = 0;
    grid[6][i].wall = 1;
    grid[10][i].free = 0;
    grid[10][i].wall = 1;
  }

  for(int i=7;i<11;i++){
    grid[i][8].free = 0;
    grid[i][8].wall = 1;
  }

  // Set a few objects
  grid[goal.x][goal.y].registry = 1;
  grid[goal.x][goal.y].free = 0;
  grid[start.x][start.y].agent = 1;
  grid[start.x][start.y].free = 0;

  print_grid(grid);

  astar(grid, start, goal);

  return 0;
}

/*FUNCTIONS*/
void print_grid(cell grid[N][N]){
  // Draw horizontal indexes
  printf("   ");
  for(int i=0; i<N; i++){printf("%2d ", i);}
  cout << endl;

  // Draw vertical indexes
  for(int j=0; j<N; j++){
    printf("%2d ", j);
    // Draw the initial grid
    for(int i=0; i<N; i++){
      print_cell(i,j, grid);
    }
    cout << endl;
  }
}

void print_cell(int x, int y, cell grid[N][N]){
    if(grid[x][y].free == 1){
      printf("\e[47m - \e[0m");
    }else if(grid[x][y].registry == 1){
      printf("\e[106m C \e[0m");
    }else if(grid[x][y].wall == 1){
      printf("\e[91m # \e[0m");
    }else if(grid[x][y].agent == 1){
      printf("\e[102m A \e[0m");
    }
}

// HEURISTIC FUNCTION
float h(int x1, int y1, int x2, int y2) {
  return sqrt( pow( (x2-x1),2) + pow( (y2-y1),2) );
}

bool isWithinGrid(int cur_x, int cur_y){
  if(cur_x >= 0 && cur_x < N && cur_y >=0 && cur_y < N){
    return true;
  }
  else return false;
}

bool isUnBlocked(cell grid[N][N], int cur_x, int cur_y){
  if(grid[cur_x][cur_y].free == 1){
    return true;
  }
  else return false;
}

bool isDestination(int cur_x, int cur_y, int dest_x, int dest_y){
  if(cur_x == dest_x && cur_y == dest_y){
    return true;
  }
  else return false;
}

void tracePath(node costs[N][N], pos goal){
  printf("\nThe path from (A) to target (C) is \n");
  int row = goal.x;
  int col = goal.y;

  pos p_temp;
  p_temp.x = 0;
  p_temp.y = 0;

  stack<pos> Path;

  while(!(costs[row][col].x == row && costs[row][col].y == col)){

    p_temp.x = row;
    p_temp.y = col;
    Path.push(p_temp);
    int temp_x = costs[row][col].x;
    int temp_y = costs[row][col].y;
    row = temp_x;
    col = temp_y;
  }

  p_temp.x = row;
  p_temp.y = col;
  Path.push(p_temp);

  while(!Path.empty()){
    pos p = Path.top();
    Path.pop();
    printf("-> (%d,%d) \n", p.x, p.y);

  }

  return;
}


// MAIN ASTAR
void astar(cell grid[N][N], pos start, pos goal){

  // Pre-check
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
  // that no cell has visited yet
  // This closed list is implemented as a boolean 2D array
  bool closedList[N][N];
  memset(closedList, false, sizeof (closedList));


  // 2D array to hold the node cost details
  node costs[N][N];

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
  node start_node(start.x, start.y, 0., 0., 0.);
  node goal_node(goal.x, goal.y, 0., 0., 0.);

  priority_queue<node> nodes_to_visit;

  nodes_to_visit.push(start_node);

  bool path_found = false;
  while(!nodes_to_visit.empty()){

    //Copy the first node and removes it from the queue
    node cur = nodes_to_visit.top();

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

          nodes_to_visit.push(node(x-1, y-1, fNew, gNew, hNew));

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
          nodes_to_visit.push(node(x, y-1, fNew, gNew, hNew));

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
          nodes_to_visit.push(node(x+1, y-1, fNew, gNew, hNew));

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
          nodes_to_visit.push(node(x-1, y, fNew, gNew, hNew));

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
          nodes_to_visit.push(node(x+1, y, fNew, gNew, hNew));

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
          nodes_to_visit.push(node(x-1, y+1, fNew, gNew, hNew));

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
          nodes_to_visit.push(node(x, y+1, fNew, gNew, hNew));

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
          nodes_to_visit.push(node(x+1, y+1, fNew, gNew, hNew));

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
