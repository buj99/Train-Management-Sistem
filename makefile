ServerCppFiles = server.cpp ./sources/Request.cpp ./sources/ClientManager.cpp ./sources/ClientData.cpp ./sources/RequestManager.cpp ./sources/ConectionManager.cpp

all : clean server client 

server : server.cpp 
		g++ $(ServerCppFiles) -o server -pthread -lstdc++
client : client.cpp
		g++ client.cpp -o client 
clean : 
	rm server client
start : 
		./client
		./server