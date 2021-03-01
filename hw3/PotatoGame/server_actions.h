#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>

#include "potato.h"

using namespace std;

int init_server(const char *port){
    int status;
    int socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    const char *hostname = NULL;

    memset(&host_info, 0, sizeof(host_info));

    host_info.ai_family   = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;
    host_info.ai_flags    = AI_PASSIVE;

    status = getaddrinfo(hostname, port, &host_info, &host_info_list);
    if (status != 0) {
        cerr << "Error: cannot get address info for host" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } //if

    socket_fd = socket(host_info_list->ai_family, 
		     host_info_list->ai_socktype, 
		     host_info_list->ai_protocol);
    if (socket_fd == -1) {
        cerr << "Error: cannot create socket" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } //if

    int yes = 1;
    status = setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
    status = bind(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1) {
        cerr << "Error: cannot bind socket" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } //if

    status = listen(socket_fd, 100);
    if (status == -1) {
        cerr << "Error: cannot listen on socket" << endl; 
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } //if

    freeaddrinfo(host_info_list);
    return socket_fd;
}

int init_client(const char *hostname, const char *port){
    int status;
    int socket_fd;
    struct addrinfo host_info;
    struct addrinfo *host_info_list;
    
    memset(&host_info, 0, sizeof(host_info));
    host_info.ai_family   = AF_UNSPEC;
    host_info.ai_socktype = SOCK_STREAM;

    status = getaddrinfo(hostname, port, &host_info, &host_info_list);
    if (status != 0) {
        cerr << "Error: cannot get address info for host" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        cerr<<"3.1"<<endl;
        return -1;
    } //if

    socket_fd = socket(host_info_list->ai_family, 
                host_info_list->ai_socktype, 
                host_info_list->ai_protocol);
    if (socket_fd == -1) {
        cerr << "Error: cannot create socket" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } //if

    status = connect(socket_fd, host_info_list->ai_addr, host_info_list->ai_addrlen);
    if (status == -1) {
        cerr << "Error: cannot connect to socket" << endl;
        cerr << "  (" << hostname << "," << port << ")" << endl;
        return -1;
    } //if

    freeaddrinfo(host_info_list);
    return socket_fd;
}

int accept_connection(int connection_fd, string *ip_addr){
    struct sockaddr_storage socket_addr;
    socklen_t socket_addr_len = sizeof(socket_addr);
    int client_connection_fd;
    client_connection_fd = accept(connection_fd, (struct sockaddr *)&socket_addr, &socket_addr_len);
    if (client_connection_fd == -1) {
        cerr << "Error: cannot accept connection on socket" << endl;
        return -1;
    } //if
    *ip_addr = inet_ntoa(((struct sockaddr_in *)&socket_addr)->sin_addr);
    return client_connection_fd;
}

int get_port(int socket_fd){
  struct sockaddr_in addr;
  socklen_t size_addr = sizeof(addr);
  if (getsockname(socket_fd, (struct sockaddr *)&addr, &size_addr) == -1){
      cerr<<"Error: getsockname error"<<endl;
  }
  return ntohs(addr.sin_port);
}

void init_fdset(fd_set &readfds, vector<int> fds, int &nfds){
    FD_ZERO(&readfds);
    nfds = fds[0];
    for(int i=0;i<fds.size();i++){
        FD_SET(fds[i],&readfds);
        if(fds[i]>nfds){
            nfds = fds[i];
        }
    }
}

void wait_for_potato(vector<int> fds,Potato &potato){
    fd_set readfds;
    int nfds;
    init_fdset(readfds,fds,nfds);
    select(nfds+1,&readfds,NULL,NULL,NULL);
    for (int i = 0; i<fds.size();i++) {
      if (FD_ISSET(fds[i], &readfds)) {
        recv(fds[i], &potato, sizeof(potato), MSG_WAITALL);
        break;
      }
    }
}

void shutdown_fds(vector<int> fds){
    for (int i = 0; i < fds.size(); i++) {
        close(fds[i]);
    }
}
