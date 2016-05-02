# Makefile to build monte_pi_sprng program
GPP=g++
CFLAGS= -g -Wall -Werror
MTOPTIONS =-pthread -std=c++11 

# --- targets
all: client.o server.o HttpRequest.o HttpResponse.o
	$(GPP) client.o HttpRequest.o HttpResponse.o -o client.out
	$(GPP) server.o HttpRequest.o HttpResponse.o -o server.out $(MTOPTIONS)
client.o:
	$(GPP) $(CFLAGS) -c client.cpp -o client.o   
server.o:
	$(GPP) $(CFLAGS) -c server.cpp -o server.o $(MTOPTIONS)
HttpRequest.o:
	$(GPP) $(CFLAGS) -c HttpRequest.cpp -o HttpRequest.o
HttpRespose.o:
	$(GPP) $(CFLAGS) -c HttpResponse.cpp -o HttpResponse.o
parsertest: HttpRequest.o HttpResponse.o
	$(GPP) $(CFLAGS) -c parserTest.cpp -o parsertest.o
	$(GPP) parsertest.o HttpRequest.o HttpResponse.o -o ptest.out	
clean:
	rm -f *.o *.out
