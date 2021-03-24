#include "exerciser.h"

void test1(connection *C){
    //add a color, a state, a team, then a player
    add_color(C,"Red");
    add_state(C,"NC");
    add_team(C,"Blue Devil",1,1,3,1);
    add_player(C,1,1,"Jerome","Robinson",34,19,4,3,1.7,0.4);
}

void exercise(connection *C)
{
    test1(C);
}


