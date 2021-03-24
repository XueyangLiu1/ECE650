#include "general_funcs.h"
#include "query_funcs.h"

void executeQuery(string query, connection *C){
    work W(*C);
    W.exec(query);
    W.commit();
}

void dropTable(const char *fileName, connection *C){
    string tableName;
    ifstream file(fileName);

    if(file.is_open()){
      while(getline(file,tableName)){
        string query = "DROP TABLE IF EXISTS "+tableName+" CASCADE;";
        executeQuery(query,C);
      }
      file.close();
      return;
    }else{
        cerr<<"Unable to open the file for droping tables!"<<endl;
        return;
    }
}

void createTable(const char *fileName, connection *C){
  string sqlStatement, line;
  ifstream file(fileName);

  if(file.is_open()){
    while(getline(file,line)){
      sqlStatement+=line;
    }
    file.close();
    executeQuery(sqlStatement,C);

    return;
  }else{
    cerr<<"Unable to open the file for creating tables!"<<endl;
    return;
  }
}

void addColorFromFile(const char *fileName, connection *C){
  int color_id;
  string line, color_name;
  ifstream file(fileName);

  if(file.is_open()){
    while(getline(file,line)){
      stringstream converter;
      converter<<line;
      converter>>color_id>>color_name;
      add_color(C,color_name);
    }
    file.close();
    return;
  }else{
    cerr<<"Unable to open the file for color!"<<endl;
    return;
  }
}

void addStateFromFile(const char *fileName, connection *C){
  int state_id;
  string line, state_name;
  ifstream file(fileName);

  if(file.is_open()){
    while(getline(file,line)){
      stringstream converter;
      converter<<line;
      converter>>state_id>>state_name;
      add_state(C,state_name);
    }
    file.close();
    return;
  }else{
    cerr<<"Unable to open the file for state!"<<endl;
    return;
  }
}

void addTeamFromFile(const char *fileName, connection *C){
  int team_id,state_id,color_id,wins,losses;
  string line, team_name;
  ifstream file(fileName);

  if(file.is_open()){
    while(getline(file,line)){
      stringstream converter;
      converter<<line;
      converter>>team_id>>team_name>>state_id>>color_id>>wins>>losses;
      add_team(C,team_name,state_id,color_id,wins,losses);
    }
    file.close();
    return;
  }else{
    cerr<<"Unable to open the file for color!"<<endl;
    return;
  }
}

void addPlayerFromFile(const char *fileName, connection *C){
  int player_id, team_id, uniform_num, mpg, ppg, rpg, apg;
  double spg, bpg;
  string line, first_name, last_name;
  ifstream file(fileName);

  if(file.is_open()){
    while(getline(file,line)){
      stringstream converter;
      converter<<line;
      converter>>player_id>>team_id>>uniform_num>>first_name>>last_name>>mpg>>ppg>>rpg>>apg>>spg>>bpg;
      add_player(C,team_id,uniform_num,first_name,last_name,mpg,ppg,rpg,apg,spg,bpg);
    }
    file.close();
    return;
  }else{
    cerr<<"Unable to open the file for player!"<<endl;
    return;
  }
}