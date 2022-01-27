/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/ 
#ifndef GRAPH_H
#define GRAPH_H

#include "Vertex.h"

class Graph{ // estrutura de classe do objeto Grafo
    private:
        uint totalVertices;                                         // Totalidade dos vertices 
        uint totalEdges;                                            // Totalidade das arestas
        vector<Vertex> vertices;                                            // Espaco de armazenamento dos vertices (Lista de Adjascencias)
        bool directioned;                                                   // Parametro utilizado na geracao do grafo, indica se e direcionado ou nao
        
        void recursiveDFSEdgePrint(int vertex, vector<bool> inStack);       // Exibicao dos valores na saida padrao de forma recursiva pelo algoritmo BFS
        void recursiveSearchAllPaths(uint idxStart, uint idxEnd, vector<bool> visited, vector<int>path, vector<uint> dist, int & idxPath, vector<pair<vector<int>,uint>> & allPaths);
    public:
        Graph(bool directioned);                                            // Construtor recebe a informacao do grafo gerado incluindo a especificacao de direcionamento
        uint getTotalVertices();                                            // Retorna total vertices
        uint getTotalEdges();                                               // Retorna total arestas
        void addNewVertex(string name);                                     // Adiciona um novo vertice
        void addNewEdge(string from, string to, int dist, string line);     // Adiciona uma nova aresta
        uint getIdxVertex(string name);                                     // Retorna o indice de um vertice dado o seu nome
        void printGraph();                                                  // Exibe o grafo na saida padrao de modo especificado na funcionalidade 7
        void searchPath(string start, string end);                          // Algoritmo de Djikstra implementado, para fins de execucao da funcionalidade 8
        void searchAllPaths(string start, string end);                      // Algoritmo de busca recursiva em DFS para encontrar todos os caminhos possíveis entre dois verticess
        void searchCycle(string target);                                    // Implementacao de deteccao de ciclo por meio de DFS modificado, funcionalidade 9
        void searchMinimalTree(string start);                               // Implementacao do algoritmo de Prim para fins de satisfazer a funcionalidade 10
        void dfsEdgePrint(int vertex);                                      // Exibicao dos valores na saida padrao de forma ordenada pelo algoritmo BFS
};

#endif