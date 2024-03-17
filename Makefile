# This file is part of the INFO0902 course given by Pr. Geurts 
# Macros
CC = gcc
CFLAGS = -std=c99 --pedantic -Wextra -Wmissing-prototypes

ALL = NNSketch

# Object files
%.o: %.c $(wildcard *.h)
	$(CC) $(CFLAGS) -c -o $@ $< -lm

# Executables
all: $(ALL)

NNSketch: main.o BoundedPriorityQueue.o DynamicTimeWarping.o NearestNeighbours.o Sketch.o easyppm.o

$(ALL):
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Phony
.PHONY: all clean run

clean:
	rm -f $(ALL) *.o ./results/*

run: $(ALL)
	./NNSketch trainingset.txt testset.txt


