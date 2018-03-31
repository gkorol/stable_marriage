#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include "environment.h"
#include "agent.h"

void print_grid(Environment* e);

int main(int argc, char** argv) {
  int couples, regs;
  vector<Agent> males;
  vector<Agent> females;

  Environment* env = new Environment();

  // Read from input file
  string line;
  ifstream myfile ("input");
  int line_c = 0;
  if (myfile.is_open()) {

    while ( getline (myfile,line) ) {

      if (line_c == 0) {
        string::size_type pos = line.find(' ');
        if(line.npos != pos) {
          couples = std::atoi(line.substr(pos + 1).c_str());
          regs = std::atoi(line.substr(0, pos).c_str());
          cout << "Total of couples: " << couples << endl;
          cout << "Total of regs:    " << regs << endl;
        }
      } else if (line_c > 0 && line_c < couples) {
        // Male lines
        vector<int> pref;
        string temp = line.substr(1);
        std::stringstream iss(temp);
        int number;
        while (iss >> number)
          pref.push_back(number);

        Agent* m = new Agent('m',line_c,1,line_c,env);
        m->init_prefs(pref);
        m->print_prefs();
      } else if (line_c >= couples) {
        // Female lines
        vector<int> pref;
        string temp = line.substr(1);
        std::stringstream iss(temp);
        int number;
        while (iss >> number)
          pref.push_back(number);

        Agent* f = new Agent('f',line_c-3,1,line_c,env);
        f->init_prefs(pref);
        f->print_prefs();
      }
      line_c++;
    }
    myfile.close();
  }

  // ADD WALLS TO THE REGISTRIES
  env->add_registries(regs);

  print_grid(env);

  return 0;
}

void print_grid(Environment* e) {
  for (int i=0; i<N; i++) {
    for (int j=0; j<N; j++) {
      e->print_cell(i,j);
    }
    cout << endl;
  }
}
