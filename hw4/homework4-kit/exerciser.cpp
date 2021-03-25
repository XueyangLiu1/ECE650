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
    query1(C, 1, 35, 36, 0, 0, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    query1(C, 0, 35, 36, 0, 0, 0, 1, 5, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0);
    query1(C, 0, 35, 36, 0, 0, 0, 0, 5, 6, 0, 0, 0, 1, 0, 0, 0, 0, 0);
    query1(C, 0, 35, 36, 0, 0, 0, 0, 5, 6, 0, 0, 0, 0, 0, 0, 1, 0, 0);
    query2(C,"Red");
    query3(C,"Syracuse");
    query4(C,"NC","DarkBlue");
    query5(C,4);
}


