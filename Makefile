# Makefile to build monte_pi_sprng program
GPP=g++
CFLAGS= -g -Wall -Werror
MTOPTIONS =-pthread -std=c++11 

# --- targets
all: client server
client: client.o HttpRequest.o HttpResponse.o Helper.o
	$(GPP) client.o HttpRequest.o HttpResponse.o Helper.o -o client.out
server: server.o HttpRequest.o HttpResponse.o Helper.o
	$(GPP) server.o HttpRequest.o HttpResponse.o Helper.o -o server.out $(MTOPTIONS)
client.o:
	$(GPP) $(CFLAGS) -c client.cpp -o client.o   
server.o:
	$(GPP) $(CFLAGS) -c server.cpp -o server.o $(MTOPTIONS)
HttpRequest.o:
	$(GPP) $(CFLAGS) -c HttpRequest.cpp -o HttpRequest.o
HttpRespose.o:
	$(GPP) $(CFLAGS) -c HttpResponse.cpp -o HttpResponse.o
Helper.o:
	$(GPP) $(CFLAGS) -c Helper.cpp -o Helper.o
parsertest: HttpRequest.o HttpResponse.o
	$(GPP) $(CFLAGS) -c parserTest.cpp -o parsertest.o
	$(GPP) parsertest.o HttpRequest.o HttpResponse.o -o ptest.out	
clean:
	rm -f *.o *.out
