#include "general_funcs.h"

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