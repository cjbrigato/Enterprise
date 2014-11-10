CC=cc
CFLAGS=-c -Wall -Werror -ansi
LDFLAGS=
SOURCES=main.c
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=install-enterprise

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS) 
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
