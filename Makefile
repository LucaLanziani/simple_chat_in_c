OBJS=chatinc.o  gestconnection.o  gestsocket.o  operation.o  listener.o     
OBJS_CLIENT=client.o   
OBJS_TEST=sessione_chat.o
CFLAGS=-g -pthread
              
all: chat_server chat_client sessione_chat

delete: delete_sessione delete_server delete_client

clean: 
	rm -f *.o *.~

sessione_chat: $(OBJS_TEST)
	$(CC) $(CFLAGS) -o $@  $^ 
	strip $@

delete_sessione: clean
	rm -f sessione_chat

chat_server: $(OBJS)
	$(CC) $(CFLAGS) -o $@  $^ 
	strip $@

chat_server_debug: $(OBJS)
	$(CC) $(CFLAGS) -o $@  $^ -lm

delete_server: clean 
	rm -f chat_server

chatinc.gz: chat_server
	gzip -c $^  > $@
        
chat_client: $(OBJS_CLIENT)
	$(CC) $(CFLAGS) -o $@  $^ 
	strip $@

chat_client_debug: $(OBJS_CLIENT)
	$(CC) $(CFLAGS) -o $@  $^

delete_client: clean
	rm -f chat_client chat_client_debug

chat_client.gz: chat_client
	gzip -c $^  > $@       