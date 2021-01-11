ServerCppFiles = server.cpp ./sources/ClientManager.cpp ./sources/ClientData.cpp ./sources/ConectionManager.cpp

all : clean server client 

server : server.cpp 
		gcc $(ServerCppFiles) -o server -pthread -lstdc++
client : client.cpp
		gcc client.cpp -o client 
clean : 
	rm server client
start : 
		./client
		./server