all: ringmaster player

ringmaster: ringmaster.cpp server_actions.h
	g++ -g -o ringmaster ringmaster.cpp
player: player.cpp server_actions.h
	g++ -g -o player player.cpp
clean:
	rm -rf ringmaster player