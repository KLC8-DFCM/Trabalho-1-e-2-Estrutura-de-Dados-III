/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/
#include "list.h"

/*-----Funções de manipulação da lista-----*/

//-----Escopo da lista-----
LIST *list_create()
{
    /*------------------------------------------
Aloca uma lista de registros vazias na memória. Caso não consiga, emite uma saída 2 para o programa.

@Params: nenhum
@Return: ponteiro para a lista alocada
------------------------------------------*/
    LIST *ret = (LIST *)malloc(sizeof(LIST));
    if (ret == NULL)
    {
        printf("> Erro de alocação de memoria na funcao list_create(). Encerrando programa...\n");
        exit(MEM_ALLOC_ERROR);
    }
    ret->start = NULL;
    ret->qnt = 0;
    return ret;
}

void list_delete(LIST *l)
{
    /*------------------------------------------
Desaloca tudo que estiver na lista encadeada

@Params: l -> ponteiro para a lista
@Return: nenhum
------------------------------------------*/
    NODE *n = l->start;
    NODE *ant;

    // Percorre todos os nós da lista
    while (n != NULL)
    {

        reg_delete(n->reg); // Exclui o registro do nó
        ant = n;
        n = n->next;
        free(ant); // Exclui o nó
    }

    free(l); // Exclui a lista
}

//-----Pesquisa por valores-----
int list_find_by_name(LIST *l, char *name)
{
    /*------------------------------------------
Busca entre os registros da lista algum que já possua como Nome da Estação a string "name"

@Params: l -> ponteiro para a lista; name -> ponteiro para a string a ser comparada
@Return: FALSE -> não encontrou o nome; TRUE-> nome encontrado
------------------------------------------*/
    if (l == NULL)
    { // Lista não alocada não possui  nenhum valor, então retorna FALSE.
        return FALSE;
    }

    NODE *n = l->start; // Recebe o nó inicial da lista

    // Busca entre todos os nós algum que possua o mesmo nome
    while (n != NULL)
    {
        if (strcmp(n->reg->nomeEstacao, name) == 0)
        { // Se encontrar algo, retorna TRUE
            return TRUE;
        }
        n = n->next;
    }

    // Se não encontrou nada, retorna FALSE
    return FALSE;
}

int list_find_by_pair(LIST *l, int cod1, int cod2)
{
    /*------------------------------------------
Busca entre os registros da lista algum que já possua o par de estações (cod1,cod2) ou (cod2,cod1)

@Params: l -> ponteiro para a lista; cod1,cod2 -> valores do par de estações
@Return: FALSE -> não encontrou o par; TRUE-> par encontrado
------------------------------------------*/
    if (l == NULL)
    { // Lista não alocada não possui  nenhum valor, então retorna FALSE.
        return FALSE;
    }

    if (cod2 == NULLVAL)
    { // Não é um par
        return FALSE;
    }

    NODE *n = l->start; // Recebe o nó inicial da lista

    // Busca entre todos os nós algum que possua o mesmo par de estações
    while (n != NULL)
    {
        if (n->reg->codEstacao == cod1 && n->reg->codProxEstacao == cod2)
        { // Se encontrar algo, retorna TRUE
            return TRUE;
        }

        if (n->reg->codEstacao == cod2 && n->reg->codProxEstacao == cod1)
        { // Se encontrar algo, retorna TRUE
            return TRUE;
        }
        n = n->next;
    }

    // Se não encontrou nada, retorna FALSE
    return FALSE;
}

//-----Insercao e remocao-----
int list_append(LIST *l, REGISTER *reg)
{
    /*------------------------------------------
Cria um no com o novo registro e adiciona-o ao final da lista. 
Caso não provenha memória para o nó, emite a saída 2.

@Params: l -> ponteiro para a lista; reg -> ponteiro para o registro a ser adicionado
@Return: 0 -> erro ao adicionar; 1-> adicionado com sucesso
------------------------------------------*/
    if (l == NULL || reg == NULL)
    {
        return 0;
    }

    // Cria o novo nó
    NODE *new = (NODE *)malloc(sizeof(NODE));
    if (new == NULL)
    {
        printf("> Erro de alocação de memoria na funcao: list_append(). Encerrando programa...\n");
        exit(MEM_ALLOC_ERROR);
    }
    new->reg = reg;
    new->next = NULL;

    l->qnt++;

    // Caso seja o primeiro nó, adiciona ele no inicio da fila e encerra a execução
    if (l->start == NULL)
    {
        l->start = new;
        return 1;
    }

    // Caso seja qualquer outro, procura o último nó da fila e adiciona-o
    NODE *last = l->start;
    while (last->next != NULL)
    {
        last = last->next;
    }

    last->next = new;
    return 1;
}

REGISTER *list_pop_front(LIST *l)
{
    /*------------------------------------------
Remove o primeiro elemento da lista e retorna o registro que ele armazena

@Params: l -> ponteiro para a lista; name -> ponteiro para a string a ser comparada
@Return: registro do antigo primeiro elemento
------------------------------------------*/
    if (l == NULL || l->start == NULL)
    { // caso a lista não esteja alocada ou esteja vazia, retorna NULL
        return NULL;
    }
    NODE *node = l->start; //Recebe o primeiro elemento

    REGISTER *ret = node->reg; //Recebe o registro do primeiro elemento
    l->start = node->next;     //Remove o primeiro elemento da lista, tirando o ponteiro para ele
    l->qnt--;

    node->reg = NULL; //Coloca o ponteiro do "reg" do ex-primeiro elemento para NULL, para não ter problemas com o registro de retorno na hora do free

    free(node); //Desaloca o ex-primeiro elemento da memória

    //Retorna o registro
    return ret;
}

int list_remove_equivalent(LIST *l,REGISTER * reg){
    /*------------------------------------------
Remove o primeiro elemento da lista que é equivalente, do ponto de vista dos campos de manutenção do header, ao passado.

@Params: l -> ponteiro para a lista; reg -> registro cujo equivalente deve ser removido da lista
@Return: nada
------------------------------------------*/
    if (l == NULL || reg == NULL)
    {
        return FALSE;
    }

    NODE *atual = l->start;
    NODE *prox = l->start->next;

    while (prox != NULL)
    {   //se encontrou um registro equivalente, deve-se encontrar ao menos 1 (o proprio que foi removido do arquivo, porem ainda nao da lista)
        if ((strcmp(prox->reg->nomeEstacao, reg->nomeEstacao) == 0) && (prox->reg->codEstacao == reg->codEstacao) && (prox->reg->codProxEstacao == reg->codProxEstacao))
        { // Se encontrar algo,remove e retorna TRUE
            
            //reorienta a lista linkada
            atual->next = prox->next;

            //remove os conteudos do no
            prox->next = NULL;
            reg_delete(prox->reg);
            free(prox);
            l->qnt--;

            return TRUE;
        }
        atual = atual->next;
        prox = prox->next;
    } 

    return FALSE; 
}