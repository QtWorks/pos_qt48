#ifndef EPOS_STATE_HEADER
#define EPOS_STATE_HEADER

#include<stdlib.h>

#define USER_STATE_ERROR     -1
#define USER_STATE_TRUE      1
#define USER_STATE_FALSE     0

/* User state struct */
typedef struct User_State_Struct {
        int             id;
        int             in; //0 - 1
} user_state;

/* Create new state struct */
extern user_state* state_new(void);

/* Getters & setters */
extern int state_clockin_set(user_state*,int);
extern int state_clockin_get(user_state*);

#endif //#define EPOS_STATE_HEADER

