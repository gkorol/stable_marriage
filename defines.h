#ifndef DEFINES_H
#define DEFINES_H

class Agent;

// Maximum number of iterations
#define MAX_IT 30000

// Grid size
#define N 20

// Status
#define DIVORCED 3
#define TAKEN    2
#define MARRIED  1
#define SINGLE   0

// Sex
#define MALE 'm'
#define FEMALE 'f'

// Agent states
// INIT - Startup the FSM, move to WANDER_S
// WANDER_S - Wanders alone until it finds someone interesting.
//            If proposal is succesfull, proceed TO_REGISTRY
// TO_REGISTRY - Checks the nearest register around it and using A*,
//               get the shortest ROUTE_TO_REG
// ROUTE_TO_REG - In possesion of the path to guide it to the nearest registry,
//                it follows the path and WAIT_FIANCE
// WAIT_FIANCE - Waits for the parter to follow it's shortest path to same registry
// MARRY - Time to make things official. Both update positions to same location,
//         and walk together as WANDER_M
// DIVORCE - Sometimes things don't go well. Both split and start WANDER_S -
//           They make sure to walk few steps away (4x) in order to avoid re-marriage
// WANDER_M - Cople wandering - They can walk together, meet more interesting
//            people and get divorced in case their bond is not strong enough
enum ag_states {INIT, WANDER_S, TO_REGISTRY, ROUTE_TO_REG,
                WAIT_FIANCE, MARRY, DIVORCE, WANDER_M};

// agent ID
struct id{
    char sex;
    int  name;
};

// matrix position
struct pos{
    unsigned int x;
    unsigned int y;
};

// matrix cell
struct cell{
    char free;      // Make it easy to check: 1 - free, INF - else
    char wall;      // 1 - wall cell, 0 - else
    int  registry;  // # of registry, -1 - else
    char couple;    // 2 - going to registry, 1 - if a couple's position, 0 - else
    Agent* agent;   // &id if agent, NULL - else
    Agent* agent_partner;
    int path;
};

#endif
