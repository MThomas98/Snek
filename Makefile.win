CXX = g++

SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%,obj/%,$(SRCS:.cpp=.o))

INCLUDE_PATHS = -Iinclude/
LIBRARY_PATHS = -Llib/
COMPILER_FLAGS = -std=c++11 -O3 -c -static-libgcc -static-libstdc++ -Wl,--subsystem,windows
LINKER_FLAGS = -lfreeglut -lglew32 -lopengl32

EXE_NAME = snake

all: $(EXE_NAME)

$(EXE_NAME): $(OBJS)
	$(CXX) $(LIBRARY_PATHS) -o $@ $^ $(LINKER_FLAGS)

obj/%.o: src/%.cpp
	$(CXX) $(INCLUDE_PATHS) $(COMPILER_FLAGS) -o $@ $<

clean:
	$(RM) -r obj/* ./snake
