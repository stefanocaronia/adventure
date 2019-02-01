
APP = Adventure
DIST = dist
PRG = $(DIST)/$(APP).exe
SRC = $(wildcard *.cpp)
OBJ = $(SRC:.c=.o)
FLA = -std=c++11 -g
CC = g++

$(PRG): $(OBJ)
	$(CC) -o $@ $^ $(FLA)

.PHONY: clean

clean:
	rm -f $(OBJ) $(PRG)
