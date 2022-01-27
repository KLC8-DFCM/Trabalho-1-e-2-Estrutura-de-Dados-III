/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/ 

#ifndef REGISTER_H
#define REGISTER_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "defines.h"

/* Funções de manipulação do registro */

//Escopo
REGISTER * reg_create();                                                            //Criacao
void reg_delete(REGISTER * reg);                                                    //Remocao

//Modificacao de valores
void reg_set_size(REGISTER * reg);                                                  //Muda o valor do campos tamanhoRegistro
void reg_update_reg(REGISTER *novo, REGISTER *antigo, SEARCH *updates, int qtd);    //Atualiza um registro

#endif