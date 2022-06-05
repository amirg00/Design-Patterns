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

test_guard: guard_app_test.o guard.o
	$(CXX) $(CXX_FLAGS) -o test_guard guard.o guard_app_test.o -lpthread

guard_app_test.o: guard_app_test.cpp guard.hpp
	$(CXX) $(CXX_FLAGS) -c guard_app_test.cpp

test_reactor: reactor.o pollserver.o
	$(CXX) $(CXX_FLAGS) -o test_reactor reactor.o pollserver.o -pthread

main1.o: main1.cpp
	$(CXX) $(CC_FLAGS) -c main1.cpp

pollserver.o: pollserver.cpp
		$(CXX)  $(CXX_FLAGS) -c pollserver.cpp

pollclient.o: pollclient.cpp
		$(CXX) $(CC_FLAGS) -c pollclient.cpp

server.o: server.cpp
		$(CXX) $(CC_FLAGS) -c server.cpp

client.o: client.cpp
		$(CXX) $(CC_FLAGS)  -c client.cpp

.PHONY: clean
clean:
	-${RM} *.o *.so testSingleton singleton_text.txt test_guard
