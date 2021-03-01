#include "server_actions.h"

using namespace std;

int main(int argc, char *argv[]){
    if (argc != 3) {
      cerr << "Syntax: player <machine_name> <port_num>\n" << endl;
      return 1;
    }
    //establish connection with ringmaster
    const char *hostname = argv[1];
    const char *port = argv[2];
    int master_socket_fd = init_client(hostname,port);

    //receive player id and number of players from ringmaster
    //send the port num of player to ringmaster
    //for other players to connect 
    int num_players,player_id;
    recv(master_socket_fd, &player_id, sizeof(player_id),MSG_WAITALL);
    recv(master_socket_fd, &num_players, sizeof(num_players),MSG_WAITALL);
    cout<<"Connected as player "<<player_id<<" out of "<<num_players<<" total players"<<endl;

    //init a server and send the server's port num to the master
    int my_server_fd = init_server("");
    int my_port = get_port(my_server_fd);
    send(master_socket_fd,&my_port,sizeof(my_port),0);

    //receive (player_id+1)'s port num and ip addr from master and connect to it as a client
    int server_player_port_int;
    char server_player_addr[100];
    recv(master_socket_fd, &server_player_port_int, sizeof(server_player_port_int),MSG_WAITALL);
    recv(master_socket_fd, &server_player_addr, sizeof(server_player_addr),MSG_WAITALL);
    char server_player_port[9];
    sprintf(server_player_port, "%d", server_player_port_int);

    int server_player_fd = init_client(server_player_addr,server_player_port);
    int server_player_id = (player_id+1)%num_players;

    //act as a server and wait for (player_id-1) to connect
    string client_player_addr;
    int client_player_fd = accept_connection(my_server_fd,&client_player_addr);
    int client_player_id = (player_id-1+num_players)%num_players;

    Potato potato;
    vector<int> communication_fds;
    communication_fds.push_back(master_socket_fd);
    communication_fds.push_back(client_player_fd);
    communication_fds.push_back(server_player_fd);

    while(true){
        wait_for_potato(communication_fds,potato);
        if(potato.num_hops==0){
            break;
        }else {
            potato.num_hops--;
            potato.num_passed++;
            potato.path[potato.num_passed-1] = player_id;
            if(potato.num_hops>0){
                srand((unsigned int)time(NULL)+potato.num_passed);
                int random = rand() % 2;
                if(random==0){
                    cout<<"Sending potato to "<<server_player_id<<endl;
                    send(server_player_fd,&potato,sizeof(potato),0);
                }else{
                    cout<<"Sending potato to "<<client_player_id<<endl;
                    send(client_player_fd,&potato,sizeof(potato),0);
                }
            }else{
                cout<<"I'm it"<<endl;
                send(master_socket_fd,&potato,sizeof(potato),0);
            }
        }
    }
    shutdown_fds(communication_fds);
    return 0;
}