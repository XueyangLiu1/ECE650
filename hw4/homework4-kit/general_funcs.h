#ifndef __GENERAL_FUNCS__
#define __GENERAL_FUNCS__

#include <string>
#include <fstream>
#include <iostream>
#include <pqxx/pqxx>

using namespace std;
using namespace pqxx;



void dropTable(const char *fileName, connection *C);
void createTable(const char *fileName, connection *C);
void excuteQuery(string query, connection *C);



#endif