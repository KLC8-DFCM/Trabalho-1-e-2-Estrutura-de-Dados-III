#include "Vertex.h"

Vertex::Vertex(string name){
/*------------------------------------------
 Construtor do vertice, seta o parametro nome

 @Params: name -> nome da estacao
 @Return: 
------------------------------------------*/
    this->stationName = name;
}

string Vertex::getName(){
/*------------------------------------------
 Retorna o nome

 @Params: 
 @Return: nome da estacao
------------------------------------------*/
    return this->stationName;
}

vector<Edge> Vertex::getEdges(){
/*------------------------------------------
 Retorna o conjunto dos vizinhos de uma estacao

 @Params: 
 @Return: Todos os elementos da classe Edge que contem informacao da vizinhanca
------------------------------------------*/
    return this->edges;
}

void Vertex::addNewEdge(string station, int dist, string line){
/*------------------------------------------
Adiciona uma nova aresta de acordo com a logica proposta.

@Params: station -> estacao a ser adicionada ; dist -> distancia fisica entre as estacoes ; line -> linha que faz a conexao
@Return: 
------------------------------------------*/
    int insertionPosition = 0;


    for(int i=0;i<(int)this->edges.size();i++){ // Busca entre as arestas alguma de mesma estacao

        int compare = station.compare(this->edges[i].getStation());

        if(compare == 0){ // Se existir a aresta, apenas adiciona uma nova linha a ela
            this->edges[i].insertNewLine(line);
            return;
        }else if(compare < 0){ // Se chegou em alguma aresta alfabeticamente após que a que está sendo inserida, pára a busca

            break;
        }

        insertionPosition++;
    }
    /* Chegou até aqui, deve criar uma nova aresta */

    Edge e = Edge(station,dist,line);   // Gera o nova aresta

    if(insertionPosition < (int)this->edges.size()){ // Caso tenha gravado uma posicao alfabética antes do final do vetor, insere a aresta nessa posicao
        this->edges.insert(this->edges.begin() + insertionPosition, e);
        return;
    }

    /* Chegou até aqui, deve inserir a aresta no final do vetor*/                   
    this->edges.push_back(e);

}