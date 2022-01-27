#/*------------------------------------------\
    NOME1: Diego Fleury Correa De Moraes\
    NUSP:  11800584\
    NOME2: Gabriel Sotto Rodrigues\
    NUSP2: 11800952\
------------------------------------------*/ 
CC= gcc
CPP = g++
INC= -I ./src/headers
FLAGSC= -Wall -g -std=c11
FLAGSCPP= -Wall -g -std=c++11
SRC= ./src
FILE_MODULES= ./src/FileModules
GRAPH_MODELS= ./src/GraphModels

CD=gdb
EXEC=programaTrab

command:
	$(CPP) -c $(FILE_MODULES)/command.cpp $(INC) $(FLAGSCPP)

register:
	$(CC) -c $(FILE_MODULES)/register.c $(INC) $(FLAGSC)

list:
	$(CC) -c $(FILE_MODULES)/list.c $(INC) $(FLAGSC)

csvfile:
	$(CC) -c $(FILE_MODULES)/CSVfile.c $(INC) $(FLAGSC)

auxiliar:
	$(CC) -c $(FILE_MODULES)/auxiliar.c $(INC) $(FLAGSC)

binfile:
	$(CPP) -c $(FILE_MODULES)/binfile.cpp $(INC) $(FLAGSCPP)

interface:
	$(CPP) -c $(FILE_MODULES)/interface.cpp $(INC) $(FLAGSCPP)

edge:
	$(CPP) -c $(GRAPH_MODELS)/Edge.cpp $(INC) $(FLAGSCPP)

vertex: 
	$(CPP) -c $(GRAPH_MODELS)/Vertex.cpp $(INC) $(FLAGSCPP)

graph: 
	$(CPP) -c $(GRAPH_MODELS)/Graph.cpp $(INC) $(FLAGSCPP)

test: all
	clear
	./$(EXEC) < teste.in

clean:
	rm *.o $(EXEC)

#https://stackoverflow.com/questions/2876357/determine-the-line-of-code-that-causes-a-segmentation-fault
debug:
	$(CD) -q $(EXEC)

all: register auxiliar list binfile csvfile command interface edge vertex graph 
	$(CPP) *.o $(SRC)/main.cpp $(INC) $(FLAGSCPP) -o $(EXEC)

run:
	./$(EXEC)


