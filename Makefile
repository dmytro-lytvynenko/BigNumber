CXX = g++

CXXFLAGS = -std=c++17 -Wall

LIB = libbig_number.a
EXAMPLE = example

LIB_OBJ = big_number.o
EXAMPLE_OBJ = example.o

all: $(LIB) $(EXAMPLE)

$(LIB): $(LIB_OBJ)
	ar rcs $@ $^

$(EXAMPLE): $(EXAMPLE_OBJ) $(LIB)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f *.o $(LIB) $(EXAMPLE)

.PHONY: all clean
