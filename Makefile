PROJ_NAME=main
CC=g++
CCFLAGS=-std=c++11 -Wall -g -c -fstack-protector
LIBS=-lsfml-graphics -lsfml-window -lsfml-audio -lsfml-network -lsfml-system
SOURCES :=  $(wildcard **/*.cpp) $(wildcard *.cpp)
OBJS := $(patsubst %.cpp, %.o, $(SOURCES))

# precomipled headers
PCH_SRC = include/stdafx.h
PCH_HEADERS =
PCH_OUT = include/stdafx.h.gch

all: $(PROJ_NAME)
	@echo Running application
	@echo
	@./$(PROJ_NAME)

$(PROJ_NAME): $(OBJS)
	@echo linking...
	$(CC) -o $@ $^ $(LIBS)


# Compiles your PCH
$(PCH_OUT): $(PCH_SRC) $(PCH_HEADERS)
	$(CC) $(CCFLAGS) -o $@ $<

%.o: %.cpp $(PCH_OUT)
	@echo Compiling and generating object $@ ...
	$(CC) $< $(CCFLAGS) -include $(PCH_SRC) -o $@

clean:
	@echo Remove
	@rm -rf $(OBJS) $(PROJ_NAME)
	@find . -name "*~" -delete
	@find . -name "*.o" -delete
	@echo done!
