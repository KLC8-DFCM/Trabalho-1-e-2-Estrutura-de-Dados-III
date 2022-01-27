/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/ 
#ifndef COMMAND_H
#define COMMAND_H

#include "Graph.h"
#include <string>

#include "binfile.h"
#include "interface.h"

extern "C"{
    #include "defines.h"
    #include "list.h"
    #include "CSVfile.h"
    #include "auxiliar.h"

}

/* Funcionalidades implementadas*/

void cmd_create();          //Funcionalidade 1: Leitura do csv para o binario
void cmd_select_all();      //Funcionalidade 2: Impressao na saida padrao de todos os dados
void cmd_select_where();    //Funcionalidade 3: Selecao de certos registros e impressao na saida padrao
void cmd_delete();          //Funcionalidade 4: Delecao de certos registros a partir de certos criterios
void cmd_insert();          //Funcionalidade 5: Insercao de certos registros no arquivo
void cmd_update();          //Funcionalidade 6: Atualizacao de certos registros dadas especificacoes
void cmd_create_graph();    //Funcionalidade 7: Cria um grafo com base no arquivo binario que representa os registro
void cmd_search_path();     //Funcionalidade 8: Busca o caminho mais curto entre duas estacoes, atraves do algoritmo de Djikstra
void cmd_search_cycle();    //Funcionalidade 9: Verifica a existencia de um ciclo que comeca e termina em um dado vertice 
void cmd_min_tree();        //Funcionalidade 10:
void cmd_search_all_paths();

#endif