all:	chat_server.c chat_client.c
	gcc chat_server.c -lpthread -o server	
	gcc chat_client.c -lpthread -o client

clean: 
	rm server client
