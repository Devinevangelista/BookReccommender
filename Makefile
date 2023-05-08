CXX = g++
OPTIONS = -Wall
PROGRAM = main.out

all: $(PROGRAM)

run: $(PROGRAM)
	./$(PROGRAM)

$(PROGRAM): main.cpp BookRecommender.cpp
	$(CXX) $(OPTIONS) main.cpp BookRecommender.cpp -o $(PROGRAM) 

clean:
	rm -f $(PROGRAM)
