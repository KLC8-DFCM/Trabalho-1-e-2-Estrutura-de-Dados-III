/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/ 
#ifndef CSVFILE_H
#define CSVFILE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defines.h"
#include "register.h"

/* Funções de manipulação do CSV */
REGISTER * CSV_read_register(FILE * CSV);   //Leitura de um registro do csv
void CSV_skip_header(FILE * CSV);           //Posiciona o file descriptor logo apos o header do csv

#endif