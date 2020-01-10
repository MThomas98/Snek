CXX = g++

SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%,obj/%,$(SRCS:.cpp=.o))

INCLUDE_PATHS = -Iinclude/
LIBRARY_PATHS = -Llibs\lib -L/usr/X11R6/lib -Llib/
COMPILER_FLAGS = -std=c++11 -O3 -c
LINKER_FLAGS = -lfreeglut -lGL -lGLU -lGLEW -lopengl32 -lmingw32

EXE_NAME = snake

all: $(EXE_NAME)

$(EXE_NAME): $(OBJS)
	$(CXX) $(LINKER_FLAGS) -o $@ $^

obj/%.o: src/%.cpp
	$(CXX) $(COMPILER_FLAGS) -o $@ $<

clean:
	$(RM) -r obj/* ./snake
