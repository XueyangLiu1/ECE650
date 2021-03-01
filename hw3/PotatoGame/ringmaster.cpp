#include "server_actions.h"
#include <vector>

using namespace std;

void print_start(int num_players, int num_hops){
    cout<<"Potato Ringmaster"<<endl;
    cout<<"Players = "<<num_players<<endl;
    cout<<"Hops = "<<num_hops<<endl;
}

void connect_with_players(int master_fd, int num_players, vector<int> &fds, vector<int> &ports, vector<string> &addrs){
    for(int player_id = 0; player_id < num_players; player_id++){
        string ip_addr;
        int port;
        int player_fd = accept_connection(master_fd,&ip_addr);

        send(player_fd, &player_id, sizeof(player_id),0);
        send(player_fd, &num_players, sizeof(num_players),0);
        recv(player_fd,&port,sizeof(port),MSG_WAITALL);

        fds.push_back(player_fd);
        addrs.push_back(ip_addr);
        ports.push_back(port);
        cout<<"Player "<<player_id<<" is ready to play"<<endl;
    }
}

void init_player_circle(int num_players, vector<int> player_fds, vector<int> ports, vector<string> addrs){
    for(int player_id = 0; player_id < num_players; player_id++){
        //select the server player for curr player
        int player_server_index = (player_id + 1) % num_players;
        
        //format the info for socket transmission
        int player_server_port = ports[player_server_index];
        string player_server_addr = addrs[player_server_index];
        char player_server_addr_c[100];
        memset(player_server_addr_c, 0, sizeof(player_server_addr_c));
        strcpy(player_server_addr_c, player_server_addr.c_str());
        //send the server player's addr and port to curr player
        //for the curr player to operate
        send(player_fds[player_id],&player_server_port,sizeof(player_server_port),0);
        send(player_fds[player_id],&player_server_addr_c,sizeof(player_server_addr_c),0);
    }
}


void shutdown_game(Potato &potato, vector<int> player_fds, bool game_played){
    assert(potato.num_hops==0);
    for (int i = 0; i < player_fds.size(); i++) {
        send(player_fds[i], &potato, sizeof(potato), 0);
    }
    if(game_played){
        potato.print_path();
    }
    shutdown_fds(player_fds);
}

int main(int argc, char *argv[]){
    //check the argument number
    if (argc != 4) {
      cerr << "Syntax: ringmaster <port_num> <num_players> <num_hops>" << endl;
      return 1;
    }

    //check players and hops and print a msg to start the game
    int num_players = atoi(argv[2]);
    int num_hops = atoi(argv[3]);
    if(num_players<=1){
        cerr << "<num_players> must be greater than 1" << endl;
        return 1;
    }
    if(num_hops<0 || num_hops>512){
        cerr << "<num_hops> must be greater than or equal to zero and less than or equal to 512" << endl;
        return 1;
    }
    print_start(num_players,num_hops);

    //init the server on ringmaster
    const char *master_port = argv[1];
    int master_fd = init_server(master_port);

    //accept connections from players
    vector<int> player_fds;
    vector<int> player_ports;
    vector<string> player_addrs;
    connect_with_players(master_fd, num_players,player_fds,player_ports,player_addrs);
    init_player_circle(num_players,player_fds,player_ports,player_addrs);

    //start the game and wait for the potato to come back
    Potato potato;
    potato.num_hops = num_hops;
    bool game_played = false;
    if(num_hops!=0){
        game_played = true;
        srand((unsigned int)time(NULL)+num_players);
        int random = rand() % num_players;
        cout << "Ready to start the game, sending potato to player " << random << endl;
        send(player_fds[random],&potato,sizeof(potato),0);
        wait_for_potato(player_fds,potato);
    }
    assert(potato.num_hops==0);
    shutdown_game(potato,player_fds,game_played);
    close(master_fd);
    return 0;
}