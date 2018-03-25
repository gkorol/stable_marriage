#include "agent.h"


Agent::agent(char sex, int name) : my_id.sex(sex), my_id.name(name) {
}

Agent::~Agent() {
}

void Agent::run() {
  switch (ps) {
    case INIT:
      // Init procedures

      ps = WANDER_S;
    break;
    case WANDER_S:
    break;
    case PROPOSE_S:
    break;
    case TO_REGISTRY:
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
    return 1
  } else if(proposer_pref > partner_pref) {
    // Will check if suitor is of higher preference than actual partner
    auto proposer_it = std::find(preferences.begin(), preferences.end(), proposer.name);
    auto partner_it = sts::find(preferences.begin(), preferences.end(), partner.name);

    if (proposer_it != std::end(preferences)) {
      int proposer_pref = std::distance(preferences.begin(), proposer_it);
    } else {
      cout << "Error receving proposition: Agent not found in prefs!" << "/n";
    }

    if (partner_it != std::end(preferences)) {
      int partner_pref = std::distance(preferences.begin(), partner_it);
    } else {
      cout << "Error receving proposition: Agent not found in prefs!" << "/n";
    }
    return 1;
  }
  return 0;
}

int Agent::propose(id proposed) {

}

int Agent::init(int x, int y, environment* env) {

}

char Agent::get_opposed_sex() {
  if( my_id.sex == MALE) {
    return FEMALE;
  } else {
    return MALE;
  }
}
