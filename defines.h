#ifndef DEFINES_H
#define DEFINES_H

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
    char wall;      // 1 - wall cell, 0 - else
    char registry;  // # of registry, 0 - else
    id* agent;      // &id if agent, NULL - else
};

// Status
#define MARRIED 1
#define SINGLE  0

// Sex
#define MALE 'm'
#define FEMALE 'f'

// Agent states
enum ag_states {INIT, WANDER_S, PROPOSE_S, TO_REGISTRY, ROUTE_TO_REG,
                WAIT_FIANCE, MARRY, DIVORCE, WANDER_M, PROPOSE_M};

#endif
