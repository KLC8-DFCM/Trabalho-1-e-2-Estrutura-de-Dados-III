/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/ 
#include "Edge.h"

Edge::Edge(string name, int dist, string line){
/*------------------------------------------
 Construtor da aresta, seta os metadados relevantes a estrutura

 @Params: name -> nome da estacao a ser adicionada (na conexao) ; 
 dist -> distancia entre as estacoes ; line -> linha que conecta estas ligacoes
 @Return: 
------------------------------------------*/
    this->stationName = name;
    this->dist = dist;
    this->lineNames.push_back(line);
}

string Edge::getStation(){
/*------------------------------------------
 Retorna o nome da estacao correspondente a esta aresta

 @Params: 
 @Return: nome da estacao
------------------------------------------*/
    return this->stationName;
}

uint Edge::getDist(){
/*------------------------------------------
 Retorna a distancia da estacao correspondente a esta aresta

 @Params: 
 @Return: distancia da estacao
------------------------------------------*/
    return this->dist;
}

vector<string> Edge::getLines(){
/*------------------------------------------
 Retorna o conjunto das linhas que passam por esta conexao

 @Params: 
 @Return: vetor de strings contendo estes nomes
------------------------------------------*/
    return this->lineNames;
}

void Edge::insertNewLine(string line){
/*------------------------------------------
 Modo organizado de inserir uma nova linha, em ordenacao lexicografica

 @Params: line -> linha a ser inserida
 @Return: 
------------------------------------------*/

    for(int i=0; i < (int)this->lineNames.size();i++){
        if(line.compare(this->lineNames[i]) < 0){
            this->lineNames.insert(this->lineNames.begin() + i,line);
            return;
        }
    }

    this->lineNames.push_back(line);
}