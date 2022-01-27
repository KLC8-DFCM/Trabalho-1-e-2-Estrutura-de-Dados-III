/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/ 
/* Defines necessários para o funcionamento da strsep */
#define _BSD_SOURCE
#define _DEFAULT_SOURCE
#define _GNU_SOURCE

#include "CSVfile.h"

/*-----Funções de manipulação do CSV-----*/
REGISTER * CSV_read_register(FILE * CSV)
{
/*------------------------------------------
 Faz a leitura do próximo registro presente no arquivo CSV. Funciona corretamente se executada após CSV_skip_header(CSV).

 @Params: CSV -> ponteiro de acesso ao arquivo CSV
 @Return: estrutura de dados do registro lido
------------------------------------------*/
    REGISTER * ret = reg_create();
    char *value, *token;

    value = (char * )malloc(STR_DEFAULT_SIZE *  sizeof(char));

    fscanf(CSV,"%[^\n]s",value);

    //Leitura do codEstacao
    token = strsep(&value,",\n");
    ret->codEstacao = atoi(token);

    //Leitura do nomeEstacao
    ret->nomeEstacao = strsep(&value,",\n");

    //Leitura codLinha
    token = strsep(&value,",\n");
    ret->codLinha = (strcmp(token,"") == 0)? NULLVAL: atoi(token);

    //Leitura nomeLinha
    ret->nomeLinha = strsep(&value,",\n");

    //Leitura codProxEst
    token = strsep(&value,",\n");
    ret->codProxEstacao = (strcmp(token,"") == 0)? NULLVAL : atoi(token);

    //Leitura Distancia prox Estação
    token = strsep(&value,",\n");
    ret->distProxEstacao = (strcmp(token,"") == 0)? NULLVAL: atoi(token);

    //Leitura codLinhaIntegra
    token = strsep(&value,",\n");
    ret->codLinhaIntegracao = (strcmp(token,"") == 0)? NULLVAL : atoi(token);

    //Leitura codEstIntegra
    token = strsep(&value,",\r");  
    ret->codEstIntegracao = (strcmp(token,"") == 0)? NULLVAL: atoi(token);
    
    //Limpa o \n do ponteiro para acessar o prox registro
    fgetc(CSV); 

    return ret;
}

void CSV_skip_header(FILE * CSV)
{
/*------------------------------------------
 Essa função é responsável por pular a primeira linha (cabeçalho) do CSV.

 @Params: CSV -> ponteiro de acesso ao arquivo CSV
 @Return: nenhum
------------------------------------------*/ 
    char tmp[STR_DEFAULT_SIZE];

    fscanf(CSV,"%s\n",tmp);

    return;
}