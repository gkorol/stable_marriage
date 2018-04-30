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
  int couples, regs, t = 0;
  int stable = 0;
  int c_inicial = 0;
  int c_final = 0;
  vector<Agent> males;
  vector<Agent> females;

  // Check if input file has been given to the program
  if(argc != 2){
    printf("[ERROR] - No input file provided.\nUsage: %s input_[Number of couples]\n", argv[0]);
    return 0;
  }

  Environment* env = new Environment();

  // ADD WALLS TO THE ENVIRONMENT
  env->add_walls();

  // Read from input file
  string line;
  ifstream myfile (argv[1]);
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
  for( t=0; t<MAX_IT; t++) {
    // Print atualizando por ciclos
    std::system("clear");
    env->print_grid();
    cout << "Cycle = " << t << endl;
    if (t>0) {
      env->print_agents();
      sleep(1);
    }

    for( int i=0; i<(couples*2); i++) {
      env->get_agent(i)->run();
    }
    // Fim: Print atualizando por ciclos

    // Print default
    // if (env->finished()){
  	// //break;
  	// if (stable == 0)
  	// 	c_inicial = t;
  	// if (stable == 5)
  	// 	c_final = t;
  	// if ((c_final - c_inicial) >= 5)
  	// 	break;
  	// else
  	// 	stable = 0;
    //
  	// stable++;
    // }

    // for( int i=0; i<(couples*2); i++) {
    //   std::system("clear");
    //   env->get_agent(i)->run();
    //   cout << "Running agent <"<<env->get_agent( i )->get_id().sex<<","<<env->get_agent( i )->get_id().name
    //   <<"> @ "<< t << " cycles" << endl;
    //   cout << "Cycle = " << t << endl;
    //   env->print_grid();
    //   if (t>0) {
    //   env->print_agents();
    //     sleep(1);
    //   }
    //   cout << endl << endl;
    // }
    // FIm - Print default

    // Print que mostra somente o estado final na tela
    // env->print_grid();
    // env->print_agents();
    // cout << endl;
    // env->who_is_happy();
    // cout << "Total cycles = " << t << endl;
    // Fim - print que mostra somente o estado final na tela

  }
  return 0;
}
