/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/ 

#ifndef EDGE_H
#define EDGE_H

#include <iostream>
#include <string>
#include <vector>

#define uint unsigned int

extern "C"{
    #include "defines.h"
}

using namespace std;

class Edge{
    private:
        string stationName;                             // Nome da estacao que tem ligacao com o vertice que contem esta 
        uint dist;                                      // Ponderamento de distancia entre as duas estacoes
        vector<string> lineNames;                       // Nome das linhas que passam por este caminho
    
    public:
        Edge(string name, int dist, string line);       // Construtor da classe, recebendo os parametros privados logo acima
        string getStation();                            // Retorna o nome da estacao
        uint getDist();                                 // Retorna a distancia
        vector<string> getLines();                      // Retorna o conjunto das linhas
        void insertNewLine(string line);                // Metodo utilizado para inserir uma nova linha durante a contrucao do grafo

};
#endif