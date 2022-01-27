/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/ 

#ifndef VERTEX_H
#define VERTEX_H

#include "Edge.h"

class Vertex{
    private:
        string stationName;                                             // Nome da estacao armazenada neste vertice
        vector<Edge> edges;                                             // Lista de vertices com que possui ligacao
    
    public:
        Vertex(string name);                                            // Construtor recebe o nome da estacao
        string getName();                                               // Retorna o nome da estacao
        vector<Edge> getEdges();                                        // Retorna o conjunto de todos os vizinhos deste vertice
        void addNewEdge(string station, int dist, string line);         // Adiciona um novo elemento na lista de adjascencias
        
};

#endif