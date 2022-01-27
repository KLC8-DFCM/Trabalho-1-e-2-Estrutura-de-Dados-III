/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/ 
#ifndef DEFINES_H
#define DEFINES_H

/* Definição de caracteres */
#define NULLVAL -1
#define NULLCHAR '|'
#define TRASH  '$'

/* Definição de erros de saida */
#define INVALID_CMD_ERROR 1
#define MEM_ALLOC_ERROR 2
#define BIN_ERROR 3
#define UNKNOWN_ERROR 4

/* Definição de valores booleanos */
#define TRUE 1
#define FALSE 0

/* Definição de tamanhos padroes a serem utilizados */
#define STR_DEFAULT_SIZE 255        //para leitura de nomes de arquivos
#define SIZE_HEADER 17
#define SIZE_TOPO_REGISTRO 5

/* Definição da estrutura de um campo de busca */
typedef struct seachStruct{
    int fieldCode;          //Código numérico do campo escolhido (vide auxiliar.c -> aux_string_to_code())
    int numValue;           //Caso o campo seja um valor numérico, o valor de busca estará aqui
    char * stringValue;     //Caso seja um campo de string, este estará aqui
}SEARCH;

/* Definição da estrutura do registros */
typedef struct registerStruct{

    char removido;              // 1B
    int tamanhoRegistro;        // 4B
    size_t proxLista;           // 8B
    int codEstacao;             // 4B
    int codLinha;               // 4B
    int codProxEstacao;         // 4B
    int distProxEstacao;        // 4B
    int codLinhaIntegracao;     // 4B
    int codEstIntegracao;       // 4B
                                // Total: 37B
    char * nomeEstacao;
    char * nomeLinha;
}REGISTER;                      // Total: 37B + nomeEstacao + nomeLinha

/* Definição da estrutura de cabeçalho */
typedef struct headerStruct{
    char status;                    // 1B
    size_t topoLista;               // 8B
    int nroEstacoes;                // 4B
    int nroParesEstacoes;           // 4B
}HEADER;                            // Total : 17B

#endif