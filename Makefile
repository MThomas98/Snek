CXX = g++

SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%,obj/%,$(SRCS:.cpp=.o))

GLEWPATH = /modules/cs324/glew-1.11.0

INCLUDE_PATHS = -Iinclude -I$(GLEWPATH)/include
LIBRARY_PATHS = -Llibs\lib -L/usr/X11R6/lib -L$(GLEWPATH)/lib
COMPILER_FLAGS = -std=c++11 -O3 -c
LINKER_FLAGS = -lglut -lGL -lGLU -lX11 -lm -lGLEW

EXE_NAME = snake

all: $(EXE_NAME)

$(EXE_NAME): $(OBJS)
	$(CXX) $(LINKER_FLAGS) -o $@ $^

obj/%.o: src/%.cpp
	$(CXX) $(COMPILER_FLAGS) -o $@ $<

clean:
	$(RM) -r obj/* ./snake
