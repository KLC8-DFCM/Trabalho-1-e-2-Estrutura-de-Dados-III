/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/ 
#ifndef INTERFACE_H
#define INTERFACE_H

#include "binfile.h"

extern "C"{
    #include <stdio.h>
    #include <stdlib.h>

    #include "defines.h"
    #include "register.h"
}


/* Exibicao de valores de forma organizada na saida padrao */
void inter_print_binary(char * name);                     //binarioNaTela        
void inter_print_header(HEADER * h);
void inter_print_register(REGISTER * reg);
void inter_print_line(REGISTER * reg);

#endif