CC = g++
CFLAGS = -std=c++17 -Wall

SRCS = main.cpp HTTPServer.cpp HTTPRequest.cpp HTTPResponse.cpp Common.cpp
OBJS = $(SRCS:.cpp=.o)
EXEC = server

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(EXEC)
