#!make -f
CC = gcc # c compiler for the run
CC_FLAGS= -Wall -g
CXX=g++ # cpp compiler for the run
CXX_FLAGS=-std=c++2a
LD_FLAGS = -shared   # linking flags
THREAD_FLAGS = -fPIC -pthread
RM = rm -f   # rm command
TARGET_LIB = design_pattern_lib.so  # target lib - shared (dynamic) library

SOURCES = Deque.c guard.cpp Object_active.c pipeline.c reactor.c singleton.cpp  # source files
OBJS=$(subst .cpp .c, .o, $(SOURCES))
LIB_OBJECTS = Deque.o guard.o Object_active.o pipeline.o reactor.o singleton.o  # library's files


.PHONY: all
all: $(TARGET_LIB) testSingleton testGuard

design_pattern_lib.so: $(LIB_OBJECTS)
	$(CC) ${LD_FLAGS} -o $(TARGET_LIB) $(LIBOBJECTS) ${THREAD_FLAGS}

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) --compile $< -o $@

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
	$(CXX) $(CXX_FLAGS) -c guard.cpp


testSingleton: singleton_app_test.cpp $(TARGET_LIB)
		$(CXX) $(CXX_FLAGS) -o testSingleton singleton_app_test.cpp ./$(TARGET_LIB) -lm

testGuard: guard_app_test.cpp $(TARGET_LIB)
		$(CXX) $(CXX_FLAGS) -o testGuard guard_app_test.cpp ./$(TARGET_LIB) -lm

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS}