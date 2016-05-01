# Makefile to build monte_pi_sprng program
GPP=g++
CFLAGS= -g -Wall -Werror

# --- targets
all: client server
client:
	$(GPP) $(CFLAGS) client.cpp -o client.out    
server:
	$(GPP) $(CFLAGS) server.cpp -o server.out
ptest: HttpRespose.o HttpRespose.o
HttpRequest.o:
	$(GPP) $(CFLAGS) -c HttpRequest.cpp -o HttpRequest.o
HttpRespose.o:
	$(GPP) $(CFLAGS) -c HttpResponse.cpp -o HttpResponse.o
parsertest: HttpRequest.o HttpResponse.o
	$(GPP) $(CFLAGS) -c parserTest.cpp -o parsertest.o
	$(GPP) parsertest.o HttpRequest.o HttpResponse.o -o ptest.out	
clean:
	rm -f *.o *.out
