#include "agent.h"


// Agent::Agent(char sex, int name) : my_id.sex(sex), my_id.name(name) {
Agent::Agent(char sex, int name, Environment* e){
  static bool seeded = false;

  if(!seeded) {
      srand(time(NULL));
      seeded = true;
  }

  walking_pattern = rand() % 2;

  my_id.sex = sex;
  my_id.name = name;
  my_position.x = -1;
  my_position.y = -1;
  env = e;
  ps = INIT;
  status = SINGLE;
}

Agent::~Agent() {
}

void Agent::init_position(int x, int y) {
  my_position.x = x;
  my_position.y = y;
}

void Agent::run() {
  Agent* neighbor = NULL;

  switch (ps) {
    case INIT:
      // Init procedures ?
      ps = WANDER_S;
      break;
    case WANDER_S:
      step();
      neighbor = env->get_nearst_agent(my_position,get_opposed_sex());
      if (status == MARRIED) {
        // Said yes to someone
        ps = TO_REGISTRY;
      } else {
        if ( neighbor != NULL) {
          // Found someone cool
          printf("Found agent around me <%c,%d> @ %d,%d\n", get_id().sex,
                  get_id().name, my_position.x, my_position.y);
          if (neighbor->marry_me(this) == 1) {
            reg.x = 0; reg.y = 0;
            // It said YES :)
            printf("<%c,%d> said yes to <%c,%d>\n", neighbor->get_id().sex,
                    neighbor->get_id().name, get_id().sex, get_id().name);
            status = MARRIED;
            partner = neighbor;
            ps = TO_REGISTRY;
          } else {
            printf("<%c,%d> was not interested in <%c,%d>\n", neighbor->get_id().sex,
                    neighbor->get_id().name, get_id().sex, get_id().name);
            ps = WANDER_S;
          }
        } else {
          ps = WANDER_S;
          printf("Nobody around me <%c,%d> @ %d,%d\n", get_id().sex, get_id().name,
                  my_position.x, my_position.y);
        }
      }
      break;
    case PROPOSE_S:
      // ?
    break;
    case TO_REGISTRY:
      reg = env->get_nearest_registry(my_position);
      printf("Path to registry at %d,%d <%c,%d>\n", reg.x, reg.y, get_id().sex, get_id().name);
      // env->get_path_to_reg(my_position, reg, path);
      // if (!path.empty())
        ps = ROUTE_TO_REG;
      // else
        // cout << "ERROR GETTING PATH TO REGISTRY!" << endl;
      break;
    case ROUTE_TO_REG:
      printf("En route to registry %d,%d <%c,%d>\n",
              reg.x, reg.y, get_id().sex, get_id().name);
      ps = WAIT_FIANCE;
      break;
    case WAIT_FIANCE:
      printf("Waiting for my fiance <%c,%d> @ %d,%d\n", get_id().sex,
            get_id().name, my_position.x, my_position.y);
      if (env->is_agent_here(partner,reg.x, reg.y))
        ps = MARRY;
      else
        ps = WAIT_FIANCE;
      break;
    case MARRY:
      // Both update positions to same location, and walk together
      printf("Getting married <%c,%d> @ %d,%d\n", get_id().sex,
            get_id().name, my_position.x, my_position.y);

      status = MARRIED;
      if( my_id.sex == FEMALE) {
        env->update_position_partner(this, partner->get_position().x, partner->get_position().y);
        env->clean_position(my_position.x, my_position.y);
      }

      ps = WANDER_M;
      break;
    case DIVORCE:
      printf("Getting divorce <%c,%d> @ %d,%d\n", get_id().sex,
            get_id().name, my_position.x, my_position.y);
      ps = WANDER_S;
      break;
    case WANDER_M:
      printf("Married wandering <%c,%d> @ %d,%d\n", get_id().sex,
            get_id().name, my_position.x, my_position.y);
      if( my_id.sex == MALE) {
        step();
      } else {
        env->update_position_partner(this, partner->get_position().x, partner->get_position().y);
      }
      // ps = DIVORCE;
      break;
    case PROPOSE_M:
      break;
  }
}

id Agent::get_id() {
  return my_id;
}

char Agent::get_status(){
  return status;
}

int Agent::marry_me(Agent* proposer) {
  if(status == SINGLE) {
    // Will accept any proposal if single
    partner = proposer;
    status = MARRIED;
    return 1;
  }
  else {
    // Will check if suitor is of higher preference than actual partner
    // std::vector<int>::iterator proposer_it = std::find(preferences.begin(), preferences.end(), proposer.name);
    // std::vector<int>::iterator partner_it = std::find(preferences.begin(), preferences.end(), partner.name);
    //
    // if (proposer_it != preferences.end() || partner_it != preferences.end()) {
    //   cout << "Error receving proposition: Agent not found in prefs!" << "/n";
    //   return -1;
    // }
    //
    // int proposer_pref = std::distance(preferences.begin(), proposer_it);
    // int partner_pref = std::distance(preferences.begin(), partner_it);
    //
    // if(proposer_pref > partner_pref) {
    //   return 1;
    // }
    return 0;
  }

}

void Agent::step() {
  // 3 possible walk patterns
  // walks in line by line, column by column or in diagonals
  // int pattern = rand() % 2;
  bool success = false;
  int temp_x = 0;
  int temp_y = 0;

  while(!success) {
    switch (walking_pattern) {
      case 0:
        // keeps y and x incremenents
        if (my_position.x+1 < N) {
          temp_x = my_position.x+1;
          temp_y = my_position.y;
        } else {
          // Matrix bound
          temp_x = 0;
          temp_y = my_position.y+1;
        }
        break;
      case 1:
        // keeps x and y incremenents
        if (my_position.y+1 < N) {
          temp_x = my_position.x;
          temp_y = my_position.y+1;
        } else {
          // Matrix bound
          temp_x = my_position.x+1;
          temp_y = 0;
        }
        break;
      case 2:
        // y and x incremenent
        if (my_position.x+1 < N && my_position.y+1 < N) {
          temp_x = my_position.x+1;
          temp_y = my_position.y+1;
        } else {
          // Matrix bound
          temp_x = my_position.x+1;
          temp_y = my_position.y+1;
        }
        break;
    }
    if (env->free_position(temp_x,temp_y) == 1){
      env->update_position(this, temp_x, temp_y);
      my_position.x = temp_x;
      my_position.y = temp_y;
      success = true;
    } else {
      // change walking pattern and try again
      walking_pattern = rand() % 2;
    }
  }
}

int Agent::propose(Agent* proposed) {
  proposed->marry_me(this);
}

pos Agent::get_position() {
  return my_position;
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
