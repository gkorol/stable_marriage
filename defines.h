#ifndef DEFINES_H
#define DEFINES_H

class Agent;

// Maximum number of iterations
#define MAX_IT 300

// Grid size
#define N 11

// Total number of registries
// #define REG_TOTAL 4

// Status
#define MARRIED 1
#define SINGLE  0

// Sex
#define MALE 'm'
#define FEMALE 'f'

// Agent states
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
    char couple;    // 1 - if a couple's position, 0 - else
    Agent* agent;   // &id if agent, NULL - else
    Agent* agent_partner;
};

#endif
