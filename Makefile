ifdef OS
	EXT := .exe
else
	EXT :=
endif

CXX := g++
IDIR := .
ALLFLAGS := `pkg-config --cflags --libs opencv`
CPPFLAGS := -g -std=c++17 -I$(IDIR) -Wall -Wextra -pedantic $(ALLFLAGS)


source_files := $(wildcard src/*.cpp)
header_files := $(wildcard src/*.hpp)
output_files := $(patsubst src/%.cpp, build/%.o, $(source_files))

.PHONY: all run clean

all: build/main$(EXT);
	
run: build/main$(EXT)
	build/main$(EXT)

build/main$(EXT): $(output_files)
	$(CXX) $(CPPFLAGS) -o $@ $(output_files)

build/%.o: src/%.cpp $(header_files)
	$(CXX) $(CPPFLAGS) -o $@ -c $<

clean:
	rm -f build/*
