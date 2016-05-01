# Makefile to build monte_pi_sprng program
GPP=g++
CFLAGS= -g

# --- targets
all: client server
client:
	$(GPP) $(CFLAGS) client.cpp -o client
server:
	$(GPP) $(CFLAGS) server.cpp -o server -pthread -std=c++11
clean:
	rm -f *.o client server
