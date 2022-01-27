/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/ 
#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "register.h"
#include "defines.h"

/* Estrutura do nó da lista */
typedef struct NodeStruct{
    REGISTER * reg;
    struct NodeStruct * next;
}NODE;

/* Estrutura da lista */
typedef struct{
    NODE * start;
    int qnt;
}LIST;

/* Funções de manipulação da lista */

//Escopo da lista
LIST * list_create();                                   //Criacao da lista
void list_delete(LIST * l);                             //Delecao da lista

//Pesquisa por valores
int list_find_by_name(LIST*l, char * name);             //Pesquisa por nome na lista
int list_find_by_pair(LIST* l,int cod1, int cod2);      //Pequisa por um certo par na lista (estacoes)

//Insercao e remocao
int list_append(LIST * l,REGISTER * reg);               //Insercao no final da lista
REGISTER * list_pop_front(LIST * l);                    //Retorna primeiro valor da lista logo apos remove-lo
int list_remove_equivalent(LIST *l,REGISTER * reg);     //Remocao de um valor equivalente da lista (para manutencao do header)

#endif