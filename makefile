#!make -f
CC = gcc # c compiler for the run
CC_FLAGS= -Wall -g
CXX=clang++-9 # cpp compiler for the run
CXX_FLAGS=-std=c++2a
LD_FLAGS = -shared   # linking flags
THREAD_FLAGS = -fPIC -pthread
RM = rm -f   # rm command
TARGET_LIB = design_pattern_lib.so  # target lib - shared (dynamic) library

LIB_OBJECTS = Deque.o guard.o Object_active.o pipeline.o reactor.o singleton.o  # library's files


.PHONY: all
all: design_pattern testSingleton pollserver pollclient server client testGuard

design_pattern_lib.so: $(LIB_OBJECTS)
	$(CC) ${LD_FLAGS} -o $(TARGET_LIB) $(LIBOBJECTS) ${THREAD_FLAGS} -lm

design_pattern: design_pattern_lib.so

# C files linking
Deque.o: Deque.c Deque.h
	$(CC) $(CC_FLAGS) -c Deque.c

Object_active.o: Object_active.c Object_active.h
	$(CC) $(CC_FLAGS) -c Object_active.c

pipeline.o: pipeline.c pipeline.h
	$(CC) $(CC_FLAGS) -c pipeline.c

reactor.o: reactor.c reactor.h
	$(CC) $(CC_FLAGS) -c reactor.c

# CPP files linking
singleton.o: singleton.cpp singleton.hpp
	$(CXX) $(CXX_FLAGS) -c singleton.cpp

guard.o: guard.cpp guard.hpp
	$(CXX) $(CXX_FLAGS) -c guard.cpp -pthread

testSingleton: singleton_app_test.cpp $(TARGET_LIB)
		$(CXX) $(CXX_FLAGS) -o testSingleton singleton_app_test.cpp ./$(TARGET_LIB) $(THREAD_FLAGS) -lm

testGuard: guard_app_test.cpp $(TARGET_LIB)
		$(CXX) $(CXX_FLAGS) -o testGuard guard_app_test.cpp ./$(TARGET_LIB) $(THREAD_FLAGS) -lm

main1.o: main1.cpp $(TARGET_LIB)
	$(CXX) $(CC_FLAGS) -c main1.cpp

pollserver.o: pollserver.c $(TARGET_LIB)
		$(CC) $(CC_FLAGS) -o pollserver.c ./$(TARGET_LIB) $(THREAD_FLAGS) -lm

pollclient.o: pollclient.c $(TARGET_LIB)
		$(CC) $(CC_FLAGS) -o pollclient.c ./$(TARGET_LIB) $(THREAD_FLAGS) -lm

server.o: server.c $(TARGET_LIB)
		$(CC) $(CC_FLAGS) -o server.c ./$(TARGET_LIB) $(THREAD_FLAGS) -lm

client.o: client.c $(TARGET_LIB)
		$(CC) $(CC_FLAGS) -o client.c ./$(TARGET_LIB) $(THREAD_FLAGS) -lm

.PHONY: clean
clean:
	-${RM} *.o *.so testSingleton
