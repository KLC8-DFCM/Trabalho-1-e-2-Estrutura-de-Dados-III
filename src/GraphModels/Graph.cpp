/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/ 
#include "Graph.h"
#include <queue>
#include <stack>
#include <algorithm>

#define UNKOWN 0
#define MARKED 1
#define VISITED 2

#define INFINITY -1

Graph::Graph(bool directioned){
/*------------------------------------------
 Construtor do grafo inicializa os metadados de contrucao

 @Params: directioned -> indica se eh ou nao direcionado
 @Return: nenhum
------------------------------------------*/
    this->totalVertices = 0;
    this->totalEdges = 0;
    this->directioned = directioned;
}

void Graph::addNewVertex(string name){
/*------------------------------------------
 Metodo chamado em toda ocasiao que deseja-se adiconar uma nova estacao/vertice
 realiza a comparacao de se ja existe esta estacao e atualiza os metadados correspondentes 
 de estrutura do grafo 

 @Params: name -> nome da estacao a ser adicionada
 @Return: nenhum
------------------------------------------*/
    Vertex v = Vertex(name);

    for(int i=0;i<(int)this->vertices.size();i++){

        if(name.compare(this->vertices[i].getName()) == 0){
            return;
        }
        
        if(name.compare(this->vertices[i].getName()) < 0){
            this->vertices.insert(this->vertices.begin() + i,v);
            this->totalVertices++;
            return;
        }
    }
    
    this->vertices.push_back(v);
    this->totalVertices++;
}

uint Graph::getTotalVertices(){
/*------------------------------------------
 Retorna o metadado da totalidade do numero de vertices

 @Params: nenhum
 @Return: quantidade de estacoes armazenadas
------------------------------------------*/
    return this->totalVertices;
}

uint Graph::getTotalEdges(){
/*------------------------------------------
 Retorna o metadado da totalidade do numero de arestas

 @Params: 
 @Return: quantidade de conexoes armazenadas
------------------------------------------*/
    return this->totalEdges;
}

uint Graph::getIdxVertex(string name){
/*------------------------------------------
 Utilizado nas ocasioes em que deseja-se acessar o indice de uma estacao
 (na lista de adjascencias) dado o seu nome

 @Params: name -> nome da estacao que deseja-se ter o indice
 @Return: indice correspondente
------------------------------------------*/

    for(int i=0; i< (int)this->vertices.size() ; i++) {
        if(name.compare(this->vertices[i].getName()) == 0){
            return i;
        }
    }

    return NULLVAL; // nao encontrada
}

void Graph::addNewEdge(string from, string to, int dist, string line){
/*------------------------------------------
 Realiza a insercao controlada (com atualizacao de metadados correspondentes)
 de novas ligacoes entre vertices

 @Params: from -> vertice de saida da conexao ; to -> vertice de chegada da conexao ; 
        dist -> distancia entre as estacoes ; line -> linha que conecta as estacoes
 @Return: nenhum
------------------------------------------*/
    int idxFrom = this->getIdxVertex(from);
    int idxTo = this->getIdxVertex(to);

    if(idxFrom != NULLVAL){
        this->vertices[idxFrom].addNewEdge(to,dist,line);

        if(idxTo == NULLVAL){
            this->addNewVertex(to);
            idxTo = this->getIdxVertex(to);
        }

        if(!(this->directioned)){
            this->vertices[idxTo].addNewEdge(from,dist,line);
        }

        this->totalEdges++;
        return;
    }

    cout << "ERRO AO INSERIR NOVA ARESTA: VERTICE NAO ENCONTRADO" << endl;
}

void Graph::printGraph(){
/*------------------------------------------
 Exibe de forma ordenada na saida padrao (de acordo com a especificacao da funcionalidade 7)
 o grafo lido do arquivo

 @Params: nenhum
 @Return: nenhum
------------------------------------------*/

    for(Vertex v : this->vertices){
        cout << v.getName();
        for(Edge edg: v.getEdges()){
            cout << " " << edg.getStation() << " " << edg.getDist() << " ";
        
            for(int i=0; i<(int)edg.getLines().size();i++){
                cout << edg.getLines()[i];
                if(i < (int)edg.getLines().size()-1 ){
                    cout << " ";
                }
            }
        }
        cout << endl;
    }
}

void Graph::searchPath(string start, string end){
/*------------------------------------------
Implementação do algoritmo de Djikstra para realizar a busca do menor
caminho entre duas estações passadas nos parâmetros.

@Params: start -> nome da estacao de origem; end -> nome da estacao de destino
@Return: nenhum
------------------------------------------*/

    /* Resgata os indices referentes as duas estacoes */
    uint idxStart = this->getIdxVertex(start);
    uint idxEnd = this->getIdxVertex(end);

    /* Vetores utilizados no algoritmo */
    vector<int> vertexStatus(this->totalVertices);        // Armazena o status do vertice ( Visitado, marcado (na fila), desconhecido (fora da fila)).
    vector<int> ant(this->totalVertices);            // Armazena os antes de cada vertice visitado
    vector<uint> dist(this->totalVertices);     // Armazena a menor distancia para chegar ao vertice indicado

    /* Seta todos os vetores para iniciar a busca */
    for(uint i=0;i<this->totalVertices;i++){
        vertexStatus[i] = UNKOWN;
        ant[i] = NULLVAL;
        dist[i] = INFINITY; 
    }

    dist[idxStart] = 0;

    /** Fila de prioridades da busca: armazena um par de dados contendo a distância até a estação e o nome da estação 
     * Ordena de forma decrescente pela menor distância. Em caso de empate, ordena em ordem alfabética.
    */
    priority_queue<pair<int,string>,vector<pair<int,string>>,greater<pair<int,string>>> queue;
    uint actVertex;

    /* Coloca o vertice de partida na lista para entrar no looping*/
    queue.push(make_pair(0,this->vertices[idxStart].getName()));

    /* Inicia o looping de busca enquanto nao visitar o vertice final (ou enquanto a fila nao estiver vazia)*/
    while(vertexStatus[idxEnd] != VISITED && !queue.empty()){
        /* Resgata o vertice do topo da lista */
        actVertex = this->getIdxVertex(queue.top().second);
        queue.pop();

        /* Percorre todas as arestas que partem deste vertice */
        for(Edge edg : this->vertices[actVertex].getEdges()){
            /* Resgata o vertice da aresta */
            uint edgeVertex = this->getIdxVertex(edg.getStation());
            if(vertexStatus[edgeVertex] != VISITED){ // Faz as verificações apenas se não for um vertice ja visitado

                if(dist[edgeVertex] > edg.getDist() + dist[actVertex]){ // Atualiza a distancia, se necessário
                    dist[edgeVertex] = edg.getDist() + dist[actVertex];
                    ant[edgeVertex] = actVertex;
                }

                if(vertexStatus[edgeVertex] == UNKOWN){ // Coloca-o na lista, se necessário
                    queue.push(make_pair(dist[edgeVertex],this->vertices[edgeVertex].getName()));
                    vertexStatus[edgeVertex] = MARKED;
                }
            }
        }
        /* Ao final, seta o estado do vertice como visitado */
        vertexStatus[actVertex] = VISITED; 
    }

    if(vertexStatus[idxEnd] != VISITED){ // Caso tenha saído do looping por que não existe mais nenhum vertice a ser explorado na lista
        cout << "Não existe caminho entre as estações solicitadas." << endl;
        return;
    }

    /* Pilha que registra o caminho até o vertice */
    stack<string> path;

    /* Le o caminho de tras para frente e empilha */
    int next = idxEnd;
    while(ant[next] != NULLVAL){
        path.push(this->vertices[next].getName());
        next = ant[next];
    }

    /* Empilha o vertice de partida */
    path.push(this->vertices[next].getName());

    /* Imprime os dados */
    cout << "Numero de estacoes que serao percorridas: " << path.size() - 1 << endl;
    cout << "Distancia que sera percorrida: " << dist[idxEnd] << endl;
    while(!path.empty()){
        cout << path.top();
        path.pop();
        if(path.size() >  0){
            cout << ", ";
        }else{
            cout << endl;
        }
    }

}

void Graph::searchCycle(string target){ 
/*------------------------------------------
Implementação do algoritmo DFS modificado de forma a encontrar um
ciclo que comeca e termina em um dado vertice.

@Params: target -> nome da estacao que deseja-se verificar a existencia de um ciclo
@Return: nenhum
------------------------------------------*/
    
    //inicializacao de valores relevantes a busca
    int idxTarget = this->getIdxVertex(target);

    vector<int> vertexStatus(this->totalVertices);
    vector<int> ant(this->totalVertices);
    vector<int> dist(this->totalVertices);

    for(int i=0;i< (int)this->totalVertices;i++){
        vertexStatus[i] = UNKOWN; 
        ant[i] = NULLVAL;
        dist[i] = INFINITY; 
    }

    dist[idxTarget] = 0;

    stack<int> stackVertex;

    // inicialmente temos apenas o No target na stack
    stackVertex.push(idxTarget);

    int idxActual;
    bool found = false;

    // enquantos ainda existirem vertices nao explorados E nao for encontrado o No target novamente
    while(!stackVertex.empty() && !found){

        // recupera-se, a cada iteracao, o primeiro elemento da stack
        idxActual = stackVertex.top();
        stackVertex.pop();
        
        // apenas se for um no 'branco' (nao explorado) segue-se para os proximos passo
        if(vertexStatus[idxActual] == VISITED || vertexStatus[idxActual] == MARKED)
        continue;

        // a partir deste instante temos que o no fora visitado
        vertexStatus[idxActual] = MARKED;

        // itera-se por cada um dos vizinhos deste Vertice (a ordem esta reversa para
        // seguir a ordenacao proposta na descricao, em que vertices com menor ordenacao
        // lexicografica estao mais ao topo da pilha )
        vector<Edge> v = this->vertices[idxActual].getEdges();
        for( vector<Edge>::reverse_iterator ptr = v.rbegin(); ptr != v.rend();ptr++){

            // para cada vizinho registra-se da onde veio e qual a distancia total ao Vertice original
            // COM RELACAO A ORDENACAO DFS ENCONTRADA
            int idViz = this->getIdxVertex((*ptr).getStation());
            
            ant[idViz] = idxActual;
            dist[idViz] = (*ptr).getDist() + dist[idxActual];

            // caso tenha encontrado o vertice desejado
            if((*ptr).getStation() == target){
                // interrompe o laco externo, o interno e salva-se o id
                found = true;
                idxActual = idViz;
                break;
            }
            
            // armazenam-se os vizinhos na stack
            stackVertex.push(idViz);
        }

        // neste ponto todos os vizinhos foram ja armazenados na stack, vertice ja fora 
        // completamente visitado
        vertexStatus[idxActual] = VISITED;
      
    }

    // caso tenha encontrado, segue-se com o armazenamento de cada um dos nomes
    // de forma que siga o padrao de saida do exercicio. Senao imprime-se a mensagem
    // que informa que nao fora encontrado o loop desejado 
    if(found){
        stack<string> path;

        int next = idxActual;
        do{
            path.push(this->vertices[next].getName());
            next = ant[next];
        }while(next != idxTarget);

        path.push(this->vertices[idxActual].getName());

        cout << "Numero de estacoes que serao percorridas: " << path.size() - 1 << endl;
        cout << "Distancia que sera percorrida: " << dist[idxActual] << endl;
        
        while(!path.empty()){
            cout << path.top();
            path.pop();
            if(path.size() >  0){
                cout << ", ";
            }else{
                cout << endl;
            }
        }

    }else{
        cout << "Nao existe um ciclo a partir da estacao de origem." << endl;
    }

    return;
}

void Graph::searchMinimalTree(string start){
/*------------------------------------------
 Método que encontra, replica e imprime a árvore geradora mínima presente
 no grafo, partindo do valor parametrizado em start.

 @Params: start - nome do vertice de partida
 @Return: nenhum
------------------------------------------*/
    uint idxStart = this->getIdxVertex(start);      // Resgata o índice do vertice escolhido

    if((int) idxStart != NULLVAL){ /* Caso o vertice exista, inicia o algoritmo de Prim */

        vector<bool> vertexStatus(this->totalVertices);   // Registra o status de cada vertice (TRUE: esta no conjunto; FALSE: ainda n esta)
        vector<int> vertexSet;                              // Conjunto de vertices (são salvos apenas os indices referentes a cada vertice)
        Graph minTree(false);                                // Grafo que conterá a árvore geradora mínima 

        /* Inicia o vetor de status e cria todos os vértices dentro da minTree */
        for(uint i=0;i< this->totalVertices;i++){
            vertexStatus[i] = false;
            minTree.addNewVertex(this->vertices[i].getName());
        }

        /* Coloca o vertice de partida no conjunto */
        vertexStatus[idxStart] = true;
        vertexSet.push_back(idxStart);

        /* Variaveis do Looping */
        uint minorWeight;       // Variavel que armazena o menor peso de aresta atual
        pair<int,int> nextEdge;         // Variavel que armazena o par de indices da menor aresta ( first: inidice do vertice, second: indice da aresta no vetor de arestas interno do vertice)

        /* Roda o looping enquanto houver algum vértice fora do conjunto */
        while(this->totalVertices  != (uint) vertexSet.size()){
            /* Reseta os valores das variaveis auxiliares a cada passo do looping */
            nextEdge = make_pair(NULLVAL,NULLVAL);      
            minorWeight = INFINITY;

            for(uint idxVertexFrom : vertexSet){ /* Percorre todos os vertices no conjunto */

                uint idxEdge = 0;   // Registra o indice atual da aresta no looping abaixo

                for(Edge edg : this->vertices[idxVertexFrom].getEdges()){ /* Percorre todas as arestas do vertice atual */

                    int idxVertexTo = this->getIdxVertex(edg.getStation()); // Resgata o indice do vertice indicado pela aresta

                    if(!vertexStatus[idxVertexTo] && minorWeight >= edg.getDist()){ /* Se for um vertice de fora do conjunto e com a distancia menor ou igual */
                        if(minorWeight > edg.getDist()){ /* Caso a distancia seja menor */
                            /* Salva os dados da aresta */
                            nextEdge = make_pair(idxVertexFrom,idxEdge);
                            minorWeight = edg.getDist();
                        }else if(nextEdge !=  make_pair(NULLVAL,NULLVAL)){ /* Caso a distancia seja igual, faz a comparacao pelos nomes */
                            if(this->vertices[idxVertexFrom].getName().compare(this->vertices[nextEdge.first].getName()) < 0){
                                /* Salva os dados da aresta */
                                nextEdge = make_pair(idxVertexFrom,idxEdge);
                                minorWeight = edg.getDist();
                            }else{
                                if(this->vertices[idxVertexTo].getName().compare(this->vertices[nextEdge.first].getEdges()[nextEdge.second].getStation()) < 0){
                                    /* Salva os dados da aresta */
                                    nextEdge = make_pair(idxVertexFrom,idxEdge);
                                    minorWeight = edg.getDist();
                                }
                            }
                        }else{ /* Chegou até aqui, algo deu errado */
                            exit(UNKNOWN_ERROR);
                        }
                    }

                    idxEdge++; // incrementa o indice
                } /* Fim do for de arestas */
            } /* Fim do for de vertices */

            if(nextEdge == make_pair(NULLVAL,NULLVAL)){ /* Se não encontrou nenhuma aresta válida, quebra o looping */
                break;
            }
            
            /* Resgata os dados da aresta encontrada */
            string vertexFrom = this->vertices[nextEdge.first].getName();
            string nextVertex = this->vertices[nextEdge.first].getEdges()[nextEdge.second].getStation();
            uint dist = this->vertices[nextEdge.first].getEdges()[nextEdge.second].getDist();

            /* Recria a aresta dentro da minTree */
            minTree.addNewEdge(vertexFrom,nextVertex,dist,"");

            /* Coloca o vertice indicado da aresta dentro do conjunto */
            vertexSet.push_back(this->getIdxVertex(nextVertex));
            vertexStatus[this->getIdxVertex(nextVertex)] = true;

        }/* fim do looping */
        
        minTree.dfsEdgePrint(idxStart);
        return;
    }
    /* Chegou até aqui, algo deu errado na execução da funcionalidade (vertice de origem inexistente ou nao há caminho que conecta todos os vertices) */

    cout << "Falha na execução da funcionalidade." << endl;
    return;
}

void Graph::dfsEdgePrint(int vertex){
/*------------------------------------------
 Função que imprime recursivamente todas as arestas do grafo, seguindo a busca em
 profundidade e partindo do indice indicado no parâmetro.

 @Params: vertex - indice de partida da impressao
 @Return: nenhum
------------------------------------------*/
    vector<bool> inStack(this->totalVertices);

    for(uint i=0;i<this->totalVertices;i++)
        inStack[i] = false;

    recursiveDFSEdgePrint(vertex,inStack);

}

void Graph::recursiveDFSEdgePrint(int vertex, vector<bool>  inStack){
/*------------------------------------------
 Função auxiliar recursiva para impressão de todas as arestas do grafo
 seguindo a busca em profundidade.

 @Params: vertex - indice de partida da impressao; inStack - lista booleana dos vertices ja listados
 @Return: nenhum
------------------------------------------*/
    inStack[vertex] = true;
    
    for(Edge edg: this->vertices[vertex].getEdges()){
        if(!inStack[this->getIdxVertex(edg.getStation())]){
            cout << this->vertices[vertex].getName() <<", ";
            cout << edg.getStation() <<", ";
            cout << edg.getDist() << endl;

            recursiveDFSEdgePrint(this->getIdxVertex(edg.getStation()), inStack);
        }
    }
}

bool ordenatePath(pair<vector<int>,uint> a, pair<vector<int>,uint> b){
/*------------------------------------------
 Funcao utilizada para dar a nocao de ordem na ordenacao dos caminhos encontrados, de acordo
 com a especificacao dada pela funcionalidade 11.
 
 @Params: a -> objeto de caminho a ser comparado, b -> objeto de caminho a ser comparado
 @Return: retorna true se a < b (de acordo com esta nocao)
------------------------------------------*/

    // Inicialmente verifica-se se os caminhos tem comprimento igual (em termos de numero de estacoes)
    if(a.first.size() == b.first.size()){
        
        // se os dois caminhos passam pela mesma quantidade de estacoes, devemos verificar a ordem lexicografica
        if(a.second == b.second){
            
            // esta comparacao funciona por conta de a ordem lexicografica
            // ter sido mapeada 1:1 para ordem de indices 
            
            // passam-se de palavra em palavra da estacao, e retorna-se qual eh maior na primeira instancia que isso ocorre
            for(int i=0;i< (int) a.first.size();i++){ 
                if(a.first[i] != b.first[i]){
                    return a.first[i] < b.first[i];
                }
            }
        }
        
        return a.second < b.second;  
    }
    
    // se nao forem iguais, retornam qual eh maior
    
    return a.first.size() < b.first.size();
}

void Graph::searchAllPaths(string start, string end){
/*------------------------------------------
Implementação do algoritmo de busca em profundidade para encontrar todos os caminhos
possíveis entre os dois vertices parametrizados

@Params: start -> nome da estacao de origem; end -> nome da estacao de destino
@Return: 
------------------------------------------*/

    /* Resgata os indices dos vertices parametrizados */
    uint idxStart = this->getIdxVertex(start);
    uint idxEnd = this->getIdxVertex(end);

    /* Vetores que serão utilizados na busca */
    vector<bool> visited(this->totalVertices);      // Registra se o vertice foi ou nao visitado no passo da busca
    vector<int> path(this->totalVertices);          // Registra o caminho atual durante o passo da recursão
    vector<uint> dist(this->totalVertices);         // Registra a distancia percorrida até o vertice no passo da recursão
    vector< pair<vector<int>,uint> > AllPaths;      // Armazena todos os caminhos encontrados para ordená-los (first: o caminho, second: a distancia percorrida)
    int idxPath = 0;                                // Registra o tamanho atual do caminho feito na recursão

    /* seta os vetores que precisam de um valor inicial */
    for(uint i=0;i<this->totalVertices;i++){
        visited[i] = false;
        dist[i] = INFINITY;
    }

    dist[idxStart] = 0; // Seta a distancia do ponto de partida 
    this->recursiveSearchAllPaths(idxStart,idxEnd,visited,path,dist,idxPath,AllPaths); /* Busca recursivamente todos os caminhos */

    if(AllPaths.size() == 0){ // Caso nao encontrou nenhum caminho
        cout << "Não existe caminho entre as estações solicitadas." << endl;
    }else{ /* Se encontrou, ordena e imprime os caminhos encontrados */
    
        sort(AllPaths.begin(),AllPaths.end(),ordenatePath);
        
        for(int i=0; i<(int)AllPaths.size(); i++){
            cout << "Numero de estacoes que serao percorridas: " << AllPaths[i].first.size() -1 << endl;
            cout << "Distancia que sera percorrida: " << AllPaths[i].second << endl;
            for(vector<int>::iterator v = AllPaths[i].first.begin(); v != AllPaths[i].first.end();v++){
                cout << this->vertices[*v].getName();
                if(v != AllPaths[i].first.end() -1){
                    cout << ", ";
                }else{
                    cout << endl;
                }
            }
            cout << endl;
        }
    }
    
}

void Graph::recursiveSearchAllPaths(
    uint idxStart, 
    uint idxEnd,
    vector<bool> visited, 
    vector<int>  path, 
    vector<uint> dist, 
    int & idxPath, 
    vector<pair<vector<int> ,  uint>> & allPaths)
{
/*------------------------------------------
Implementação do algoritmo de busca em profundidade de forma recursiva para encontrar todos os caminhos
possíveis entre os dois vertices parametrizados. Cada caminho encontrado é salvo dentro de AllPaths, para
ser ordenado posteriormente.

@Params: idxStart -> index da estacao de origem; idxEnd -> index da estacao de destino; 
@Params: visited -> vetor q indica os vertice ja passados; path -> vetor que empilha o caminho atual; 
@Params: dist -> vetor que registra a distancia atual; idxPath ->registra a altura atual da pilha de vertices em path; 
@Params: allPaths -> referencia ao vetor que armazena todos o caminhos ; 
@Return: 
------------------------------------------*/

    /* Seta o vetor de partida como visitado e o empilha no caminho */
    visited[idxStart] = true;
    path[idxPath] = idxStart;
    idxPath++;

    
    if(idxStart == idxEnd){ /* Se o vetor de partida for o mesmo de chegada, a recursão encontrou um caminho */
        pair<vector<int>,uint> tmp; 
        vector<int> cpy;

        for(int i=0;i<idxPath;i++){ /* Copia o caminho encontrado */
            cpy.push_back(path[i]);
        }

        /* Salva o caminho e a distancia percorrida em allPaths */
        tmp = make_pair(cpy,dist[idxEnd]); 
        allPaths.push_back(tmp);
    
    }else{ /* Se não encontrou um caminho, busca recursivamente um
     caminho entre as arestas do vertice atual */
    
        for(Edge edg : this->vertices[idxStart].getEdges()){
            uint idxNext = this->getIdxVertex(edg.getStation());
            if(!visited[idxNext]){ /* Percorre apenas vertices nao visitados */
                dist[idxNext] = dist[idxStart] + edg.getDist(); // Calcula a distancia percorrida até o vertice da aresta atual */
                /* Continua a buscar um caminho pelos vertice da aresta atula */
                recursiveSearchAllPaths(idxNext,idxEnd,visited,path,dist,idxPath,allPaths);
            }
        }
    }
    
    /* Chegando aqui, todos os caminhos pelo vertice foram encontrados,
     então desempilha-o para encontrar outro caminhos */
    idxPath--;
    visited[idxStart] = false;
    dist[idxStart] = INFINITY;
}



