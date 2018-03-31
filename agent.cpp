#include "agent.h"


// Agent::Agent(char sex, int name) : my_id.sex(sex), my_id.name(name) {
Agent::Agent(char sex, int name, int x, int y, Environment* e){
   my_id.sex = sex;
   my_id.name = name;
   my_position.x = x;
   my_position.y = y;
   env = e;
}

Agent::~Agent() {
}

void Agent::run() {
  switch (ps) {
    case INIT:
      // Init procedures ?
      ps = WANDER_S;
    break;
    case WANDER_S:
      Agent* neighbor;
      neighbor = env->get_nearst_agent(my_position,get_opposed_sex());
      if ( neighbor != NULL) {
        // Found someone cool
        if (neighbor->marry_me(my_id) == 1) {
          // It said YES :)
          // Talvez direto para o registry
          // ps = PROPOSE_S;
          ps = TO_REGISTRY;
        }
      } else {
        ps = WANDER_S;
      }
    break;
    case PROPOSE_S:
      // ?
    break;
    case TO_REGISTRY:
      pos reg;
      reg = env->get_nearst_registry(my_position);
      env->get_path_to_reg(my_position, reg, path);
      if (!path.empty())
        ps = ROUTE_TO_REG;
      else
        cout << "ERROR GETTING PATH TO REGISTRY!" << endl;
    break;
    case ROUTE_TO_REG:
    break;
    case WAIT_FIANCE:
    break;
    case MARRY:
    break;
    case DIVORCE:
    break;
    case WANDER_M:
    break;
    case PROPOSE_M:
    break;
  }
}

id Agent::get_id() {
  return my_id;
}

int Agent::marry_me(id proposer) {
  if(status == SINGLE) {
    // Will accept any proposal if single
    return 1;
  }
  else {
    // Will check if suitor is of higher preference than actual partner
    std::vector<int>::iterator proposer_it = std::find(preferences.begin(), preferences.end(), proposer.name);
    std::vector<int>::iterator partner_it = std::find(preferences.begin(), preferences.end(), partner.name);

    if (proposer_it != preferences.end() || partner_it != preferences.end()) {
      cout << "Error receving proposition: Agent not found in prefs!" << "/n";
      return -1;
    }

    int proposer_pref = std::distance(preferences.begin(), proposer_it);
    int partner_pref = std::distance(preferences.begin(), partner_it);

    if(proposer_pref > partner_pref) {
      return 1;
    }
  }
  return 0;
}

int Agent::propose(Agent* proposed) {
  proposed->marry_me(my_id);
}

int Agent::init_prefs(vector<int> p) {
  for(int i=0; i<p.size(); i++) {
    preferences.push_back(p[i]);
  }
}

void Agent::print_prefs() {
  for(int i=0; i<preferences.size(); i++) {
    printf("Ag<%c,%d> preferences[%d] = %d\n", my_id.sex, my_id.name, i, preferences[i]);
  }
}

char Agent::get_opposed_sex() {
  if( my_id.sex == MALE) {
    return FEMALE;
  } else {
    return MALE;
  }
}
