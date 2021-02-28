#include "server_actions.h"
#include "potato.h"

using namespace std;

int main(int argc, char *argv[]){
    if (argc != 3) {
      cerr << "Syntax: player <machine_name> <port_num>\n" << endl;
      return 1;
    }
    //establish connection with ringmaster
    const char *hostname = argv[1];
    const char *port = argv[2];
    int socket_fd = init_client(hostname,port);

    //receive player id and number of players from ringmaster
    int num_players,player_id;
    recv(socket_fd, &player_id, sizeof(player_id),MSG_WAITALL);
    recv(socket_fd, &num_players, sizeof(num_players),MSG_WAITALL);
    


}