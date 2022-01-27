/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/ 
#ifndef AUXILIAR_H
#define AUXILIAR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "defines.h"

/* Funções para manipulação e processamento de dados */

//Tratamento de dados realizado nas buscas
int aux_string_to_code(char * str);                     //Codifica os dados de uma string para inteiros              
int aux_is_a_string_field(int code);                    //Retorna TRUE se o codigo refere-se a uma string

//Processamento de dados da entrada padrao 
void aux_read_searches(SEARCH * searches, int qnt);     //Leitura dos valores da entrada padrao no formato das buscas realizadas     
void aux_scan_reg(REGISTER * reg);                      //Leitura dos valores da entrada padrao no formato dos registros
void aux_scan_quote_string(char *str);                  //scan_quote_string (fornecida)
#endif