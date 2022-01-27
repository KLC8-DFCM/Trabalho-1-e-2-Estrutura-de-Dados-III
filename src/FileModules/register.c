/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/ 

#include "register.h"

/*-----Funções de manipulação do registro-----*/

//Escopo
REGISTER * reg_create()
{
/*------------------------------------------
Essa função aloca uma nova estrutura de registro e retorna um ponteiro para ela.
Caso a alocação dê errado, envia a saída 2 e encerra o programa.

@Params: nenhum
@Return: ponteiro para o novo registro alocado
------------------------------------------*/ 
    REGISTER * ret = (REGISTER * ) malloc(sizeof(REGISTER));
    if(ret == NULL){
        printf("Erro de alocacao de memoria na funcao reg_create. Encerrando o programa..\n");
        exit(MEM_ALLOC_ERROR);
    }
    ret->removido = '0';
    ret->proxLista = NULLVAL;

    return ret;
}

void reg_delete(REGISTER * reg)
{
/*------------------------------------------
Essa função desaloca um registro já criado anteriormente

@Params: ponteiro para o registro a ser desalocado
@Return: nenhum
------------------------------------------*/  

    if(reg->nomeEstacao != NULL){
        free(reg->nomeEstacao);
    }

    if(reg->nomeLinha != NULL){
        free(reg->nomeLinha);
    }

    free(reg);

    return;
}

//Modificacao de valores
void reg_set_size(REGISTER * reg)
{
/*------------------------------------------
Calcula e seta o tamanho do registro no campo reg->tamanhoRegistro

@Params: reg -> ponteiro para o registro
@Return: nenhum
------------------------------------------*/ 
    // 32 pois não conta-se o inicio (removido e tamRegistro = SIZE_TOPO_REGISTRO = 5) 
    // e tem de se contar ambos pipes logo + 2
    reg->tamanhoRegistro = 32 + strlen(reg->nomeEstacao) + 2;

    if(reg->nomeLinha != NULL){
        reg->tamanhoRegistro += strlen(reg->nomeLinha);
    }
    return;
}

void reg_update_reg(REGISTER *novo, REGISTER *antigo, SEARCH *updates, int qtd)
{
    /*------------------------------------------
Função auxiliar para transferir valores novos para um registro

@Params: novo --> registro ja atualizado; antigo --> registro a ser atualizado;
         updates --> atualizações; qtd --> quantidade de atualizações
@Return: nenhum
------------------------------------------*/

    //este vetor de tamanho estatico segue a segunsignede logica:
    //se, apos o loop logo a seguir, o valor permanece em -1
    //entao o campo associado àquele codigo recebe o valor antigo
    //caso contrario, o indice de updates sera salvo no local (>-1)
    //e assim podera ser usado para indexacao mais tarde na passagem
    //dos valores efetivamente
    int campos_atualizados[8] = {NULLVAL, NULLVAL, NULLVAL, NULLVAL, NULLVAL, NULLVAL, NULLVAL, NULLVAL};

    //Checagem de quais campos devem ser atualizados
    for (int i = 0; i < qtd; i++)
    { 
        campos_atualizados[(updates[i]).fieldCode - 1] = i;  //Pequena correcao de -1 para manter o alcance do vetor
    }

    //campos nao atualizados recebem os valores
    //se campos sao numericos, é simples a resolução (pode ser tratado com um ternario)
    
    novo->codEstacao = (campos_atualizados[0] == NULLVAL) ? antigo->codEstacao : updates[campos_atualizados[0]].numValue;
    novo->codLinha = (campos_atualizados[2] == NULLVAL) ? antigo->codLinha : updates[campos_atualizados[2]].numValue;
    novo->codProxEstacao = (campos_atualizados[4] == NULLVAL) ? antigo->codProxEstacao : updates[campos_atualizados[4]].numValue;
    novo->distProxEstacao = (campos_atualizados[5] == NULLVAL) ? antigo->distProxEstacao : updates[campos_atualizados[5]].numValue;
    novo->codLinhaIntegracao = (campos_atualizados[6] == NULLVAL) ? antigo->codLinhaIntegracao : updates[campos_atualizados[6]].numValue;
    novo->codEstIntegracao = (campos_atualizados[7] == NULLVAL) ? antigo->codEstIntegracao : updates[campos_atualizados[7]].numValue; 
    
    //se campos sao de string, deve-se ainda tomar o cuidado de efetuar as alocações dinamicas
    if (campos_atualizados[1] == NULLVAL)
    { // string MALLOC
        novo->nomeEstacao = (char *)malloc((strlen(antigo->nomeEstacao) + 1) * sizeof(char));
        if (novo->nomeEstacao == NULL)
        {
            printf("Erro de alocacao na funcao reg_update_reg. Encerrando o programa..\n");
            exit(MEM_ALLOC_ERROR);
        }
        strcpy(novo->nomeEstacao, antigo->nomeEstacao);
    }
    else
    {
        novo->nomeEstacao = (char *)malloc((strlen(updates[campos_atualizados[1]].stringValue) + 1) * sizeof(char));
        if (novo->nomeEstacao == NULL)
        {
            printf("Erro de alocacao na funcao reg_update_reg. Encerrando o programa..\n");
            exit(MEM_ALLOC_ERROR);
        }
        strcpy(novo->nomeEstacao, updates[campos_atualizados[1]].stringValue);
    }

    if (campos_atualizados[3] == NULLVAL)
    { // string MALLOC
        novo->nomeLinha = (char *)malloc((strlen(antigo->nomeLinha) + 1) * sizeof(char));
        if (novo->nomeLinha == NULL)
        {
            printf("Erro de alocacao na funcao reg_update_reg. Encerrando o programa..\n");
            exit(MEM_ALLOC_ERROR);
        }
        strcpy(novo->nomeLinha, antigo->nomeLinha);
    }
    else
    {
        novo->nomeLinha = (char *)malloc((strlen(updates[campos_atualizados[3]].stringValue) + 1) * sizeof(char));
        if (novo->nomeLinha == NULL)
        {
            printf("Erro de alocacao na funcao reg_update_reg. Encerrando o programa..\n");
            exit(MEM_ALLOC_ERROR);
        }
        strcpy(novo->nomeLinha, updates[campos_atualizados[3]].stringValue);
    }

    return;
}