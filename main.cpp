#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <unistd.h>
#include "environment.h"
#include "agent.h"

void print_grid(Environment* e);

int main(int argc, char** argv) {
  int couples, regs;
  vector<Agent> males;
  vector<Agent> females;

  Environment* env = new Environment();

  // ADD WALLS TO THE ENVIRONMENT
  env->add_walls();

  // Read from input file
  string line;
  ifstream myfile ("input");
  int line_c = 0;
  if (myfile.is_open()) {

    while ( getline (myfile,line) ) {

      if (line_c == 0) {
        string::size_type pos = line.find(' ');
        if(line.npos != pos) {
          couples = std::atoi(line.substr(0, pos).c_str());
          regs = std::atoi(line.substr(pos + 1).c_str());
          cout << "Total of couples: " << couples << endl;
          cout << "Total of regs:    " << regs << endl << endl;

          // ADD REGISTRIES TO THE ENVIRONMENT
          env->add_registries(regs);
        }
      } else if (line_c > 0 && line_c <= couples) {
        // Male lines
        vector<int> pref;
        string temp = line.substr(1);
        std::stringstream iss(temp);
        int number;
        while (iss >> number)
          pref.push_back(number);

        Agent* m = new Agent(MALE,line_c,env);
        m->init_prefs(pref);
        env->add_agent(m);
        // m->print_prefs();
      } else if (line_c > couples) {
        // Female lines
        vector<int> pref;
        string temp = line.substr(1);
        std::stringstream iss(temp);
        int number;
        while (iss >> number)
          pref.push_back(number);

        Agent* f = new Agent(FEMALE,line_c-3,env);
        f->init_prefs(pref);
        env->add_agent(f);
        // f->print_prefs();
      }
      line_c++;
    }
    myfile.close();
  }

  // Main loop
  for( int t=0; t<MAX_IT; t++) {
    for( int i=0; i<(couples*2); i++) {
      // std::system("clear");
      env->get_agent( i )->run();
      // cout << "Running agent <"<<env->get_agent( i )->get_id().sex<<","<<env->get_agent( i )->get_id().name
      // <<"> @ "<< t << " cycles" << endl;
      cout << "Cycle = " << t << endl;
      print_grid(env);
      if (t>0) {
        env->print_agents();
        // sleep(1);
      }
      cout << endl << endl;
    }
  }
  env->who_is_happy();

  return 0;
}

void print_grid(Environment* e) {
  printf("   ");
  for (int i=0; i<N; i++)
    printf("%2d ", i);
  cout << endl;
  for (int i=0; i<N; i++) {
    printf("%2d ", i);
    for (int j=0; j<N; j++) {
      e->print_cell(i,j);
    }
    cout << endl;
  }
}
