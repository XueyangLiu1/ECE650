#include "query_funcs.h"


void add_player(connection *C, int team_id, int jersey_num, string first_name, string last_name,
                int mpg, int ppg, int rpg, int apg, double spg, double bpg)
{
    work W(*C);
    stringstream statement;
    statement<<"INSERT INTO PLAYER (TEAM_ID, UNIFORM_NUM, FIRST_NAME, LAST_NAME, MPG," 
        <<"PPG, RPG, APG, SPG, BPG) VALUES ("<<team_id<<", "<<jersey_num<<", "
        <<W.quote(first_name)<<", "<<W.quote(last_name)<<", "<<mpg<<", "<<ppg
        <<", "<<rpg<<", "<<apg<<", "<<spg<<", "<<bpg<<");";
    W.exec(statement.str());
    W.commit();
}


void add_team(connection *C, string name, int state_id, int color_id, int wins, int losses)
{
    work W(*C);
    stringstream statement;
    statement << "INSERT INTO TEAM (NAME, STATE_ID, COLOR_ID, WINS, LOSSES) VALUES ("
        <<W.quote(name)<<", "<<state_id<<", "<<color_id<<", "<<wins<<", "<<losses<<");";
    W.exec(statement.str());
    W.commit();
}


void add_state(connection *C, string name)
{
    work W(*C);
    stringstream statement;
    statement << "INSERT INTO STATE (NAME) VALUES ("<<W.quote(name)<<");";
    W.exec(statement.str());
    W.commit();
}


void add_color(connection *C, string name)
{
    work W(*C);
    stringstream statement;
    statement << "INSERT INTO COLOR (NAME) VALUES ("<<W.quote(name)<<");";
    W.exec(statement.str());
    W.commit();
}

template <typename T>
string genQuery1Statement(string name, T min, T max){
    stringstream statement;
    statement<<name<<" <= "<<max<<" AND "<<name<<" >= "<<min;      
    return statement.str();
}

void query1(connection *C,
	    int use_mpg, int min_mpg, int max_mpg,
            int use_ppg, int min_ppg, int max_ppg,
            int use_rpg, int min_rpg, int max_rpg,
            int use_apg, int min_apg, int max_apg,
            int use_spg, double min_spg, double max_spg,
            int use_bpg, double min_bpg, double max_bpg
            )
{
    string names[6] = {"MPG","PPG","RPG","APG","SPG","BPG"};
    int flags[6] = {use_mpg,use_ppg,use_rpg,use_apg,use_spg,use_bpg};
    int intMins[4] = {min_mpg,min_ppg,min_rpg,min_apg};
    int intMaxs[4] = {max_mpg,max_ppg,max_rpg,max_apg};
    double doubleMins[2] = {min_spg,min_bpg}; 
    double doubleMaxs[2] = {max_spg,max_bpg};
    
    stringstream query;
    query<<"SELECT * FROM PLAYER";
    bool firstFlag = true;
    for(int i = 0;i<6;i++){
        if(flags[i]){
            if(firstFlag){
                query<<" WHERE ";
                firstFlag = false;
            }else{
                query<<" AND ";
            }
            if(i<4){
                query<<genQuery1Statement<int>(names[i],intMins[i],intMaxs[i]);
            }else{
                query<<genQuery1Statement<double>(names[i],doubleMins[i-4],doubleMaxs[i-4]);
            }
        }
    }
    query<<";";

    nontransaction N(*C);
    result R(N.exec(query.str()));
    cout<<"PLAYER_ID TEAM_ID UNIFORM_NUM FIRST_NAME LAST_NAME MPG PPG RPG APG SPG BPG"<<endl;
    for(result::iterator i = R.begin(); i!=R.end();i++){
        cout<<i[0].as<int>()<<" "<<i[1].as<int>()<<" "<<i[2].as<int>()<<" "
        <<i[3].as<string>()<<" "<<i[4].as<string>()<<" "<<i[5].as<int>()<<" "
        <<i[6].as<int>()<<" "<<i[7].as<int>()<<" "<<i[8].as<int>()<<" "
        <<fixed<<setprecision(1)<<i[9].as<double>()<<" "<<i[10].as<double>()<<endl;
    }
}


void query2(connection *C, string team_color)
{
    stringstream query;
    query<<"SELECT TEAM.NAME FROM TEAM, COLOR WHERE COLOR.NAME = \'"
        <<team_color<<"\' AND TEAM.COLOR_ID = COLOR.COLOR_ID;";
    nontransaction N(*C);
    result R(N.exec(query.str()));
    cout<<"NAME"<<endl;
    for(result::iterator i = R.begin(); i!=R.end();i++){
        cout<<i[0].as<string>()<<endl;
    }
}


void query3(connection *C, string team_name)
{
    stringstream query;
    query<<"SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME FROM TEAM, PLAYER "
        <<"WHERE TEAM.NAME = \'"<<team_name
        <<"\' AND PLAYER.TEAM_ID = TEAM.TEAM_ID ORDER BY PPG DESC;";
    nontransaction N(*C);
    result R(N.exec(query.str()));
    cout<<"FIRST_NAME LAST_NAME"<<endl;
    for(result::iterator i = R.begin(); i!=R.end();i++){
        cout<<i[0].as<string>()<<" "<<i[1].as<string>()<<endl;
    }
}


void query4(connection *C, string team_state, string team_color)
{
    stringstream query;
    query<<"SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, PLAYER.UNIFORM_NUM FROM PLAYER, TEAM, STATE, COLOR "
        <<"WHERE STATE.NAME = \'"<<team_state<<"\'"
        <<" AND COLOR.NAME = \'"<<team_color<<"\'"
        <<" AND TEAM.STATE_ID = STATE.STATE_ID AND TEAM.COLOR_ID = COLOR.COLOR_ID"
        <<" AND PLAYER.TEAM_ID = TEAM.TEAM_ID;";
        
    nontransaction N(*C);
    result R(N.exec(query.str()));
    cout<<"FIRST_NAME LAST_NAME UNIFORM_NUM"<<endl;
    for(result::iterator i = R.begin(); i!=R.end();i++){
        cout<<i[0].as<string>()<<" "<<i[1].as<string>()<<" "<<i[2].as<int>()<<endl;
    }
}


void query5(connection *C, int num_wins)
{
    stringstream query;
    query<<"SELECT PLAYER.FIRST_NAME, PLAYER.LAST_NAME, TEAM.NAME, TEAM.WINS "
        <<"FROM PLAYER, TEAM "
        <<"WHERE TEAM.WINS > \'"<<num_wins<<"\'"
        <<" AND PLAYER.TEAM_ID = TEAM.TEAM_ID;";
    
    nontransaction N(*C);
    result R(N.exec(query.str()));
    cout<<"FIRST_NAME LAST_NAME NAME WINS"<<endl;
    for(result::iterator i = R.begin(); i!=R.end();i++){
        cout<<i[0].as<string>()<<" "<<i[1].as<string>()
            <<" "<<i[2].as<string>()<<" "<<i[3].as<int>()<<endl;
    }
}
