#!make -f
CXX=clang++-9 # cpp compiler
CXX_FLAGS=-std=c++2a
LD_FLAGS = -shared   # linking flags
RM = rm -f   # rm command
TARGET_LIB = design_pattern_lib.so  # target lib - shared (dynamic) library

SOURCES = Deque.cpp guard.cpp Object_active.cpp pipeline.cpp reactor.cpp singleton.cpp  # source files
OBJS=$(subst .cpp,.o,$(SOURCES))

.PHONY: all
all: ${TARGET_LIB}

$(TARGET_LIB): $(OBJS)
	$(CXX) ${LD_FLAGS} -o $@ $^

%.o: %.cpp
	$(CXX) $(CXX_FLAGS) --compile $< -o $@

.PHONY: clean
clean:
	-${RM} ${TARGET_LIB} ${OBJS} $(SOURCES:.cpp=.d)