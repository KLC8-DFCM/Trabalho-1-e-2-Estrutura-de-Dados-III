/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/ 
#ifndef BINFILE_H
#define BINFILE_H

#include "Graph.h"

extern "C"{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "defines.h"
    #include "register.h"
    #include "list.h"
}

/* Funções para manipulação do arquivo binário */

//Tratamento do header
HEADER * bin_read_header(FILE * binfp);                                                                     //Leitura do header do arquivo binario
int bin_write_header(FILE * binfp, HEADER *  h);                                                            //Escrita do header no arquivo binario
void bin_set_status(FILE * binfp, char s);                                                                  //Atualizacao do campo status do header do arquivo binario  

//Leitura e escrita de registros
REGISTER * bin_read_register(FILE * binfp);                                                                 //Leitura de um registro no arquivo binario
char * bin_read_string(FILE * binfp);                                                                       //Auxiliar de leitura de string
void bin_set_next_regs(FILE * binfp);                                                                       //Auxiliar de correcao de posicao de file descriptor
int bin_write_new_register(FILE * binfp, REGISTER * reg, int qntTrash);                                     //Escrita de um registro no arquivo binario

//Manipulacao da lista encadeada de arquivo (removidos)
void bin_insert_file_linked_list(FILE *binfp,HEADER*h,REGISTER*reg);                                        //Insercao de um arquivo na lista encadeada de arquivo
int bin_set_pointer_insert(FILE * binfp,HEADER * h, int minSize);                                           //Posicionamento correto do ponteiro para insercao na lista encadeada de arquivo

//Pesquisas realizadas no arquivo binario, para insercao e atualizacao de registros
LIST * bin_search_all_registers(FILE *binfp, SEARCH * searches, int n);                                     //Busca no arquivo binario de acordo com uma especificacao
int bin_remove(FILE * binfp,HEADER * h,SEARCH * searches, int n);                                           //Direcionamento da busca com o objetivo de remocao
int bin_update(FILE *binfp,HEADER *h,SEARCH *searches_x, int x, SEARCH *searches_y, int y,LIST* Regs);      //Direcionamento da busca com o objetivo de remocao                                                                                                     
Graph bin_set_graph(FILE * binFile, bool directioned);

//Funcoes de auxilio na manipulacao de valores importantes ao arquivo binario
HEADER * bin_header_create();                                                                               //Criacao do header inicializado com valores padronizados
void bin_atualiza_header(LIST *l, REGISTER *reg, HEADER *h);                                                //Atualizacao de valores do header (nroEstacoes,nroParesEstacoes)
int bin_verify_search(REGISTER * reg,SEARCH s);                                                             //Auxiliar de busca
void bin_delete_search(SEARCH *search, int tam);                                                            //Auxiliar de desalocação de busca


#endif