#include "agent.h"

bool operator!=(const pos &p1, const pos &p2) {
  if(p1.x == p2.x && p1.y == p2.y){
    return false;
  }else return true;
}

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
  new_partner = 0;
  asked_divorce = 0;
  partner = NULL;
  reg.x = 0;
  reg.y = 0;
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
      printf("Single wandering <%c,%d>\n", get_id().sex, get_id().name);
      if (status == TAKEN) {
        // Said yes to someone
        reg = env->get_nearest_registry(my_position);
        ps = TO_REGISTRY;
      } else {
        step();
        neighbor = env->get_nearest_agent(my_position,get_opposed_sex());
        if ( neighbor != NULL) {
          // Found someone cool
          printf("Found agent around me <%c,%d>\n", get_id().sex, get_id().name);
          if (neighbor->marry_me(this) == 1) {
            // Uhuuul! It said YES :)
            printf("<%c,%d> said yes to <%c,%d>\n", neighbor->get_id().sex,neighbor->get_id().name, get_id().sex, get_id().name);
            partner = neighbor;
            status = TAKEN;
            reg = env->get_nearest_registry(my_position);
            ps = TO_REGISTRY;
          } else {
            printf("<%c,%d> was not interested in <%c,%d>\n", neighbor->get_id().sex, neighbor->get_id().name, get_id().sex, get_id().name);
            ps = WANDER_S;
          }
        } else {
          ps = WANDER_S;
          printf("Nobody around me <%c,%d>\n", get_id().sex, get_id().name);
        }
      }
      break;

    case TO_REGISTRY:
      if(partner->reg != reg){
        set_register(partner, reg);   // Define the same target registry to the partner
      }else{

        printf("Path to registry at %d,%d <%c,%d>\n", reg.x, reg.y, get_id().sex, get_id().name);
        env->get_path_to_reg(my_position, reg);

        PrivatePath = env->get_path();

        if (!PrivatePath.empty()){
          ps = ROUTE_TO_REG;
        }else cout << "ERROR GETTING PATH TO REGISTRY!" << endl;

      }
      break;

    case ROUTE_TO_REG:
      printf("In route to registry %d,%d <%c,%d>\n", reg.x, reg.y, get_id().sex, get_id().name);
      // code for agent to go to registry goes here
      pos p;

      p = PrivatePath.top();
      if(p.x == reg.x && p.y == reg.y){
        // Quando notar que chegou no cartório, para antes de entrar nele
        PrivatePath.pop();
        ps = WAIT_FIANCE;
      }else{
        env->update_position(this, p.x, p.y);
        my_position.x = p.x;
        my_position.y = p.y;
        PrivatePath.pop();
      }

      break;

    case WAIT_FIANCE:
      printf("Waiting for my fiance <%c,%d>\n", get_id().sex, get_id().name);
      if (env->is_agent_here(partner,reg.x, reg.y)){
        if (status == TAKEN){
          // Already completed the private path to registry, release the exclusive path cells from grid
          env->clear_path(PrivatePath);
          ps = MARRY;
        } else
          ps = WANDER_S;
      } else
          ps = WAIT_FIANCE;

      break;

    case MARRY:
      // Both update positions to same location, and walk together
      printf("Getting married <%c,%d>\n", get_id().sex, get_id().name);
      if (partner->get_partner() == this) {
	      status = MARRIED;
        partner->status = MARRIED;

        if( my_id.sex == FEMALE) {
          env->update_position_partner(this, partner->get_position().x, partner->get_position().y);
          env->clean_position(my_position.x, my_position.y);
        }
        ps = WANDER_M;
      } else {
        ps = DIVORCE;
      }
      break;

    case DIVORCE:
      printf("Getting divorce <%c,%d>\n", get_id().sex, get_id().name);
      status = SINGLE;
      env->clean_position_partner(my_position.x,my_position.y);
      partner = NULL;
      step();
      step();
      step();
      step();
      ps = WANDER_S;
      break;

    case WANDER_M:
      printf("Married wandering <%c,%d>\n", get_id().sex, get_id().name);
      if (asked_divorce) {
	      asked_divorce = 0;
	      ps = DIVORCE;
	      break;
      }

      if (new_partner) {
        ps = MARRY;
        new_partner = 0;
	      break;
      }

      if (my_id.sex == MALE)
          step();
      else
          env->update_position_partner(this, partner->get_position().x, partner->get_position().y);
      // Somebody new?
      neighbor = env->get_nearest_agent(my_position,get_opposed_sex());
      if ( neighbor != NULL && neighbor != partner ) {
          // Found someone cooler
          printf("Found agent around me <%c,%d>\n", get_id().sex, get_id().name);
          if (greater_pref(neighbor)) {
            if (neighbor->marry_me(this)  == 1) {
              // Uhuuul! It said YES :)
              reg.x = 0; reg.y = 0;
              partner->divorce_me();
              printf("<%c,%d> said yes to <%c,%d>\n", neighbor->get_id().sex,
                      neighbor->get_id().name, get_id().sex, get_id().name);
              partner = neighbor;
              ps = TO_REGISTRY;
              ps = MARRY;
            } else {
              printf("<%c,%d> was not interested in <%c,%d>\n", neighbor->get_id().sex,
                      neighbor->get_id().name, get_id().sex, get_id().name);
            }
          } else {
            printf("Was not worthy <%c,%d>\n", get_id().sex, get_id().name);
          }
        }
      ps = WANDER_M;
      break;
  }
}

id Agent::get_id() {
  return my_id;
}

pos Agent::get_registry() {
  return reg;
}


char Agent::get_status(){
  return status;
}

int Agent::get_state() {
	return ps;
}

int Agent::marry_me(Agent* proposer) {
  if(status == SINGLE || status == DIVORCED) {
    // Neighbor will accept any proposal if single or divorced
    partner = proposer;
    status = TAKEN;
    return 1;

  }else if(status == TAKEN){
    // Neighbor is already heading to register to get married with another agent, ignores the proposal
    return 0;

  } else {
    // Married couples can find someone more interesting
    if(greater_pref(proposer)) {
      printf("<%c,%d> is accepting new partner <%c,%d>\n", proposer->get_id().sex,proposer->get_id().name, partner->get_id().sex, partner->get_id().name);
      new_partner = 1;
      partner = proposer;
      status = TAKEN;
      return 1;
    }
  }
  return 0;
}

void Agent::divorce_me() {
  //status = SINGLE;
  asked_divorce = 1;
  //partner = NULL;
  //env->clean_position(my_position.x,my_position.y);
  //env->clean_position_partner(my_position.x,my_position.y);
  //step();

  //ps = DIVORCE;
}

int Agent::greater_pref(Agent* a) {
  int partner_it = -1;
  int a_it = -1;

  for (int i=0;i<preferences.size(); i++) {
      if(a->get_id().name == preferences[i])
        a_it = i;
      if(partner->get_id().name == preferences[i])
        partner_it = i;
  }

  if(a_it == -1 || partner_it == -1) {
    printf("Error getting preferences!\n");
    return 0;
  }

  if(a_it < partner_it)
    return 1;
  else
    return 0;
}

void Agent::step() {
  // 8 possible walk patterns
  // Left/right/top/bottom & diagonals

  bool success = false;
  int temp_x = 0;
  int temp_y = 0;
  int try_out = 0;

  while(!success) {
    switch (walking_pattern) {
      // case 0: // right
      //   if(my_position.x+1 < N){
      //     temp_x = my_position.x+1;
      //     temp_y = my_position.y;
      //   }else{
      //     // Matrix bound
      //     temp_x = 0;
      //     temp_y = my_position.y+1;
      //   }
      // break;
      // case 1: // left
      //   if(my_position.x-1 >= 0){
      //     temp_x = my_position.x-1;
      //     temp_y = my_position.y;
      //   }else{
      //     // Matrix bound
      //     temp_x = N-1;
      //     temp_y = my_position.y+1;
      //   }
      // break;
      // case 2:
      // break;
      // case 3:
      // break;
      // case 4:
      // break;
      // case 5:
      // break;
      // case 6:
      // break;
      // case 7:
      // break;
      // case 8:
      //   temp_x = rand() % N-1;
      //   temp_y = rand() % N-1;
      //   break;
      // default:
      //   printf("Error walking!!!\n");
      //   break;

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
      case 3:
        temp_x = rand() % N-1;
        temp_y = rand() % N-1;
        break;
      default:
        printf("Error walking!!!\n");
        break;
    }
    if (env->free_position(temp_x,temp_y) == 1){
      env->update_position(this, temp_x, temp_y);
      my_position.x = temp_x;
      my_position.y = temp_y;
      success = true;
      if (walking_pattern == 3)
        walking_pattern = 0;
    } else {
      // change walking pattern and try again
      walking_pattern = rand() % 2;
      success = false;
      try_out++;
      if (try_out >= 3)
        walking_pattern = 3; //randomize if not able to walk at any direction
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

int Agent::get_pref_at(int i) {
  return preferences[i];
}

Agent* Agent::get_partner() {
  return partner;
}

char Agent::get_opposed_sex() {
  if( my_id.sex == MALE) {
    return FEMALE;
  } else {
    return MALE;
  }
}

void Agent::set_register(Agent* partner, pos reg){
  partner->reg = reg;
}
