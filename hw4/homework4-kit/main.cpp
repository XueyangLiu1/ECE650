#include <iostream>
#include <pqxx/pqxx>


#include "exerciser.h"
#include "general_funcs.h"

using namespace std;
using namespace pqxx;

#define CREATE_TABLE "createTables.txt"
#define DROP_TABLE "dropTables.txt"

int main (int argc, char *argv[]) 
{

  //Allocate & initialize a Postgres connection object
  connection *C;

  try{
    //Establish a connection to the database
    //Parameters: database name, user name, user password
    C = new connection("dbname=ACC_BBALL user=postgres password=passw0rd");
    if (C->is_open()) {
      cout << "Opened database successfully: " << C->dbname() << endl;
    } else {
      cout << "Can't open database" << endl;
      return 1;
    }
    dropTable(DROP_TABLE,C);
    createTable(CREATE_TABLE,C);
    addColorFromFile("color.txt",C);
    addStateFromFile("state.txt",C);
    addTeamFromFile("team.txt",C);
    addPlayerFromFile("player.txt",C);
    exercise(C);
    C->disconnect();
  } catch (const std::exception &e){
    cerr << e.what() << std::endl;
    return 1;
  }
  return 0;
}


