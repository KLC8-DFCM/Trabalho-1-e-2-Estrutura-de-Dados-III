/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/
#include "binfile.h"

/*-----Funções para manipulação do arquivo binário-----*/

//-----Tratamento do header-----
HEADER *bin_read_header(FILE *binfp)
{
    /*------------------------------------------
Lê todo o cabeçalho do arquivo Binário

@Params: binfp -> ponteiro do arquivo binário
@Return: ponteiro para o cabeçalho lido
------------------------------------------*/
    HEADER *ret = (HEADER *)malloc(sizeof(HEADER));

    if (ret == NULL)
    {
        printf("> Erro de alocação de memoria na funcao bin_read_header(). Encerrando programa...\n");
        exit(MEM_ALLOC_ERROR);
    }

    fseek(binfp, 0, SEEK_SET);

    fread(&(ret->status), 1, sizeof(char), binfp);
    fread(&(ret->topoLista), 1, sizeof(size_t), binfp);
    fread(&(ret->nroEstacoes), 1, sizeof(int), binfp);
    fread(&(ret->nroParesEstacoes), 1, sizeof(int), binfp);

    return ret;
}

int bin_write_header(FILE *binfp, HEADER *h)
{
    /*------------------------------------------
Sobrescreve os 17 primeiros bytes do arquivo binário, correspondendo aos dados do cabeçalho.

@Params: binfp -> ponteiro do arquivo binário; h -> cabeçalho para gravar
@Return: 0 -> erro na gravação do arquivo; 1 -> gravação realizada com sucesso
------------------------------------------*/
    if (binfp == NULL || h == NULL)
    {
        return 0;
    }
    //Posiciona o  ponteiro no inicio do arquivo binário
    fseek(binfp, 0, SEEK_SET);

    // Escreve os campos do cabeçalho
    fwrite(&(h->status), 1, sizeof(char), binfp);
    fwrite(&(h->topoLista), 1, sizeof(size_t), binfp);
    fwrite(&(h->nroEstacoes), 1, sizeof(int), binfp);
    fwrite(&(h->nroParesEstacoes), 1, sizeof(int), binfp);

    return 1;
}

void bin_set_status(FILE *binfp, char s)
{
    /*------------------------------------------
Alterna o status do arquivo para o valor passado em "s". Só realiza se s = '0' ou s = '1'.

@Params: binfp -> ponteiro do arquivo binário; s -> valor do status
@Return: 0 -> erro na gravação do arquivo; 1 -> gravação realizada com sucesso
------------------------------------------*/
    if (s == '0' || s == '1')
    {
        fseek(binfp, 0, SEEK_SET);
        fwrite(&s, 1, sizeof(char), binfp);
    }

    return;
}

//-----Leitura e escrita de registros-----
REGISTER *bin_read_register(FILE *binfp)
{
    /*------------------------------------------
Le próximo registro presente no arquivo binário e aloca na memória a estrutura referente ao registro

@Params: binfp -> ponteiro do arquivo binário
@Return: ponteiro para o registro lido, NULL caso esteja logicamente removido
------------------------------------------*/
    REGISTER *ret = reg_create();

    if (ret == NULL)
    {
        printf("> Erro de alocação de memoria na funcao: bin_read_register(). Encerrando programa...\n");
        exit(MEM_ALLOC_ERROR);
    }

    // Leitura dos campos fixos
    if (fread(&(ret->removido), 1, sizeof(char), binfp))
    { // Se conseguir ler, continua a leitura
        if (ret->removido == '0')
        { // Se o registro não foi removido, lê
            fread(&(ret->tamanhoRegistro), 1, sizeof(int), binfp);
            fread(&(ret->proxLista), 1, sizeof(size_t), binfp);
            fread(&(ret->codEstacao), 1, sizeof(int), binfp);
            fread(&(ret->codLinha), 1, sizeof(int), binfp);
            fread(&(ret->codProxEstacao), 1, sizeof(int), binfp);
            fread(&(ret->distProxEstacao), 1, sizeof(int), binfp);
            fread(&(ret->codLinhaIntegracao), 1, sizeof(int), binfp);
            fread(&(ret->codEstIntegracao), 1, sizeof(int), binfp);

            // Leitura dos campos variáveis
            ret->nomeEstacao = bin_read_string(binfp); // Função auxiliar para leitura de string
            ret->nomeLinha = bin_read_string(binfp);

            bin_set_next_regs(binfp); // Lê e ignora o lixo no final do registro
            return ret;
        }
        else
        { // Se não for, posiciona o ponteiro no próximo registro
            fread(&(ret->tamanhoRegistro), 1, sizeof(int), binfp);
            fseek(binfp, ret->tamanhoRegistro, SEEK_CUR);
            reg_delete(ret); // remove o espaco alocado caso esteja removido logicamente

            return NULL;
        }
    }
    reg_delete(ret); // remove o espaco alocado caso nao consiga ler

    return NULL;
}

char *bin_read_string(FILE *binfp)
{
    /*------------------------------------------
 Lê o próximo campo do registro em forma de string.
 Lê todos os caracteres em uma string auxiliar até encontrar "|"
 Contabiliza os caracteres lidos e gera uma string dinamicamente alocada e retorna o ponteiro para ela.

 [LEMBRETE: DAR FREE NA VARIÁVEL QUE RECEBE O RETORNO DA FUNÇÃO]

 @Params: binfp -> ponteiro de acesso ao arquivo CSV
 @Return: ponteiro para a string gerada
------------------------------------------*/

    char tmp[STR_DEFAULT_SIZE];
    char c;

    int size = 0;

    fread(&c, 1, sizeof(char), binfp);

    while (c != '|')
    {
        tmp[size] = c;
        size++;

        fread(&c, 1, sizeof(char), binfp);
    }

    tmp[size] = '\0';

    if (size > 0)
    {
        char *ret = (char *)malloc((size + 1) * sizeof(char));

        if (ret == NULL)
        {
            printf("> Erro de alocação de memoria na função bin_read_string(). Encerrando programa...\n");
            exit(MEM_ALLOC_ERROR);
        }
        strcpy(ret, tmp);

        return ret;
    }

    return NULL;
}

void bin_set_next_regs(FILE *binfp)
{
    /*------------------------------------------
Pula todo o lixo de memória presente no final do registro

 @Params: binfp-> ponteiro para o arquivo
 @Return: Tamanho do registro compatível. Se não encontrar nenhum, retorna -1.
------------------------------------------*/
    if (feof(binfp))
    { //Não tem o que ler
        return;
    }

    char trash = TRASH;
    while (!feof(binfp) && trash == TRASH)
    { // Lê o arquivo enquanto for lixo
        fread(&trash, 1, sizeof(char), binfp);
    }

    if (!feof(binfp))
    {
        fseek(binfp, -1, SEEK_CUR); // Se chegou aqui, começou a ler o próximo registro, ou seja, precisa voltar 1 byte para ser posicionado corretamente
    }

    return;
}

int bin_write_new_register(FILE *binfp, REGISTER *reg, int qntTrash)
{
    /*------------------------------------------
Rebece o registro "reg" e e grava-o onde o ponteiro estiver posicionado

@Params: binfp -> ponteiro do arquivo binário; reg -> registro para gravar; qntTrash -> quantidade de lixo necessário para preencher o registro
@Return: 0 -> erro na gravação do arquivo; 1 -> gravação realizada com sucesso
------------------------------------------*/

    // Verificação dos parâmetros enviados
    if (binfp == NULL || reg == NULL)
    {
        return 0;
    }

    // Escrita dos campos fixos
    fwrite(&(reg->removido), 1, sizeof(char), binfp);
    fwrite(&(reg->tamanhoRegistro), 1, sizeof(int), binfp);
    fwrite(&(reg->proxLista), 1, sizeof(size_t), binfp);
    fwrite(&(reg->codEstacao), 1, sizeof(int), binfp);
    fwrite(&(reg->codLinha), 1, sizeof(int), binfp);
    fwrite(&(reg->codProxEstacao), 1, sizeof(int), binfp);
    fwrite(&(reg->distProxEstacao), 1, sizeof(int), binfp);
    fwrite(&(reg->codLinhaIntegracao), 1, sizeof(int), binfp);
    fwrite(&(reg->codEstIntegracao), 1, sizeof(int), binfp);

    // Escrita dos campos variáveis
    char delimiter = NULLCHAR;

    for (int i = 0; i < (int) strlen(reg->nomeEstacao); i++)
    {
        fwrite(&(reg->nomeEstacao[i]), 1, sizeof(char), binfp);
    }
    fwrite(&delimiter, 1, sizeof(char), binfp);

    if (reg->nomeLinha != NULL)
    {
        for (int i = 0; i < (int) strlen(reg->nomeLinha); i++)
        {
            fwrite(&(reg->nomeLinha[i]), 1, sizeof(char), binfp);
        }
    }

    fwrite(&delimiter, 1, sizeof(char), binfp);

    //Preenche o resto com lixo
    char trash = TRASH;
    for (int i = 0; i < qntTrash; i++)
    {
        fwrite(&(trash), 1, sizeof(char), binfp);
    }

    return 1;
}

//-----Manipulacao da lista encadeada de arquivo (removidos)-----
void bin_insert_file_linked_list(FILE *binfp, HEADER *h, REGISTER *reg)
{
    /*------------------------------------------
Essa função é responsável por executar a insercao de um registro no arquivo binário.
Se atravessa a lista linkada e nao encontra uma posicao em que o registro possa ser 
inserido, insere ao final

@Params: binfp --> descritor de arquivo do binario; h --> header do arquivo; reg --> registro a ser inserido
@Return: nenhum
------------------------------------------*/
    int removedRegSize = bin_set_pointer_insert(binfp, h, reg->tamanhoRegistro);

    reg->proxLista = NULLVAL;

    if (removedRegSize == NULLVAL) //insere no final
    {
        bin_write_new_register(binfp, reg, 0);
    }
    else //insere em algum ponto no meio
    {
        int trashSize = removedRegSize - reg->tamanhoRegistro;
        reg->tamanhoRegistro = removedRegSize;
        bin_write_new_register(binfp, reg, trashSize);
    }
    return;
}

int bin_set_pointer_insert(FILE *binfp, HEADER *h, int minSize)
{
    /*------------------------------------------
Posiciona o ponteiro em um registro removido que tenha tamanho maior ou igual ao minSize.
Caso encontre algum compatível, retorna o tamanho dele e mantém o ponteiro posicionado no inicio do registro.
Caso não encontre nenhum compatível, retorna -1.

 @Params: binfp-> ponteiro para o arquivo ; h -> ponteiro para o cabeçalho ja lido minSize -> tamanho minimo de um registro compatível
 @Return: Tamanho do registro compatível. Se não encontrar nenhum, retorna -1.
------------------------------------------*/
    size_t oldByteOffset = 1;         // Armazena a posição registro removido anterior. Começa no topoLista
    size_t byteOffset = h->topoLista; // Armazena a posição do registro removido atual

    while ((int)byteOffset != NULLVAL)
    {
        fseek(binfp, byteOffset + 1, SEEK_SET); // Posiciona o ponteiro no próximo registro da lista

        int sizeReg; // Lê o tamanho do registro removido
        fread(&sizeReg, 1, sizeof(int), binfp);

        if (sizeReg >= minSize)
        { // Se for um tamanho compatível com o mínimo. Organiza o arquivo e seta o ponteiro nesse registro
            if (oldByteOffset == 1)
            { // Caso o oldByteOffset indique o topoLista, atualiza apenas a estrutura de cabeçalho (será escrita posteriormente no arquivo)
                fread(&(h->topoLista), 1, sizeof(size_t), binfp);
            }
            else
            { // Caso contrário, atualiza o proxLista do registro anterior com o valor do proxLista atual
                size_t tmp;
                fread(&tmp, 1, sizeof(size_t), binfp);
                fseek(binfp, oldByteOffset + SIZE_TOPO_REGISTRO, SEEK_SET); // Posiciona o ponteiro no proxLista do removido anterior
                fwrite(&tmp, 1, sizeof(size_t), binfp);    //  Sobrescreve o proxLista anterior com o proxLista atual
            }

            fseek(binfp, byteOffset, SEEK_SET); // Posiciona novamente o ponteiro no inicio do registro compatível
            return sizeReg;                     // Retorna o tamanho do registro compatível
        }

        oldByteOffset = byteOffset; // Se chegou até aqui, o registro lido não é compatível. Pega o próximo
        fread(&byteOffset, 1, sizeof(size_t), binfp);
    }

    fseek(binfp, 0, SEEK_END); // Se chegou até aqui, não há registro removido compatível com o tamanho necessário.
    return NULLVAL;
}

//-----Pesquisas realizadas no arquivo binario, para insercao e atualizacao de registros-----
LIST *bin_search_all_registers(FILE *binfp, SEARCH *searches, int n)
{
    /*------------------------------------------
Realiza a busca no arquivo binário de acordo com a combinação de filtros em fieldCode e fieldValue.
Armazena todos os registros que condizerem com os valores buscados em uma lista encadeada

@Params: binfp -> ponteiro do arquivo binário; searches -> filtros; n-> quantidade de filtros
@Return: Lista encadeada com todos os registros buscados
------------------------------------------*/
    if (binfp == NULL)
    {
        return NULL;
    }

    fseek(binfp, SIZE_HEADER, SEEK_SET);

    LIST *results = list_create(); //Lista que armazenara os registros buscados
    REGISTER *aux = NULL;          //Armazena os valores do registro atual
    int acceptedReg;               //Flag que indicará se o registro é ou não
    while (!feof(binfp))
    {
        aux = bin_read_register(binfp); //Chama a função para ler o próximo registro do arquivo

        if (aux != NULL)
        {                       // bin_read_register() pode retornar null (removido logicamente)
            acceptedReg = TRUE; // Considera-se que o registro é válido até encontrar alguma diferença com os campos de busca
            for (int i = 0; i < n; i++)
            {
                if (!bin_verify_search(aux, searches[i]))
                { // Caso o registro não passe em algum dos filtros de busca, altera a flag
                    acceptedReg = FALSE;
                    break;
                }
            }

            if (acceptedReg)
            { // Se ele for um registro que corresponde a busca, entra na lista de resultados
                list_append(results, aux);
            }
        }
    }

    return results;
}

int bin_remove(FILE *binfp, HEADER *h, SEARCH *searches, int n)
{
/*------------------------------------------
Realiza a busca no arquivo binário de acordo com a combinação de filtros em fieldCode e fieldValue.
Remove logicamente todos os registros que atendenderem aos critérios de busca

@Params: binfp -> ponteiro do arquivo binário (em modo rb+); searches -> filtros; n-> quantidade de filtros
@Return: TRUE -> remoção (ou tentativa de remoção) bem sucedida; FALSE-> algum problema ao remover
------------------------------------------*/
    if (binfp == NULL)
    {
        return FALSE;
    }

    REGISTER *aux;   //Armazena os valores do registro atual
    int acceptedReg; //Flag que indicará se o registro é ou não

    fseek(binfp, SIZE_HEADER, SEEK_SET); //Comeca no topo dos registros

    while (!feof(binfp))
    {
        aux = bin_read_register(binfp); //Lê o proximo registro
        if (aux != NULL)
        {                       // bin_read_register() pode retornar null (removido logicamente)
            acceptedReg = TRUE; // Considera-se que o registro é válido até encontrar alguma diferença com os campos de busca
            for (int i = 0; i < n; i++)
            {
                if (!bin_verify_search(aux, searches[i]))
                { // Caso o registro não passe em algum dos filtros de busca, altera a flag
                    acceptedReg = FALSE;
                    break;
                }
            }

            if (acceptedReg)
            {                               // Se ele for um registro que corresponde a busca, entra na lista de resultados
                size_t curr = ftell(binfp); //Registra a posição atual do arquivo

                fseek(binfp, -1 * (aux->tamanhoRegistro + SIZE_TOPO_REGISTRO), SEEK_CUR); //reposiciona o ponteiro no inicio do registro lido
                size_t byteOffset = ftell(binfp);

                char rmFlag = '1';                                      // Variável auxiliar para escrita no arquivo
                fwrite(&rmFlag, 1, sizeof(char), binfp);                // Atualiza o registro para removido
                fwrite(&(aux->tamanhoRegistro), 1, sizeof(int), binfp); // Reescreve o tamanho do registro
                fwrite(&(h->topoLista), 1, sizeof(size_t), binfp);      // Atualiza o proxLista

                h->topoLista = byteOffset; // Atualiza o topo da lista de removidos

                LIST *l = bin_search_all_registers(binfp, NULL, 0); // Busca todos os registros ativos

                if (!list_find_by_name(l, aux->nomeEstacao))
                { // Se não encontrou nada, a estação por inteira foi removida
                    h->nroEstacoes = h->nroEstacoes - 1;
                }
                if (!list_find_by_pair(l, aux->codEstacao, aux->codProxEstacao) && aux->codProxEstacao != NULLVAL)
                { // Se a estação possuia um proxEstação, um par foi removido
                    h->nroParesEstacoes = h->nroParesEstacoes - 1;
                }

                fseek(binfp, curr, SEEK_SET); 
            }

        }
    }

    return TRUE;
}

int bin_update(FILE *binfp, HEADER *h, SEARCH *searches_x, int x, SEARCH *searches_y, int y, LIST *l)
{
    /*------------------------------------------
Realiza a busca no arquivo binário de acordo com a combinação de filtros em fieldCode e fieldValue.
Atualiza, com os valores presentes em searches_y, todos os registros que atendenderem aos critérios de busca

@Params: binfp -> ponteiro do arquivo binário (em modo rb+); h -> header para ser atualizado, searches_x -> filtros; x-> quantidade de filtros, searches_y -> valores a serem atualizados, y-> quantidade de campos a serem atualizados
@Return: TRUE -> atualização (ou tentativa de atualização) bem sucedida; FALSE-> algum problema ao atualizar
------------------------------------------*/

    REGISTER *aux = NULL;
    int acceptedReg;
    while (!feof(binfp))
    {
        // vai de registro em registro
        aux = bin_read_register(binfp);

        if (aux != NULL) // se o registro NAO foi logicamente removido
        {
            acceptedReg = TRUE; // Considera-se que o registro é válido até encontrar alguma diferença com os campos de busca
            for (int i = 0; i < x; i++)
            {
                if (!bin_verify_search(aux, searches_x[i]))
                { // Caso o registro não passe em algum dos filtros de busca, altera a flag
                    acceptedReg = FALSE;
                    break;
                }
            }
            if (acceptedReg == TRUE)
            { // se os requisitos condizerem com o esperado

                //cria o registro novo (updated)
                REGISTER *updated = reg_create();

                //passagem dos valores ao novo registro (updated)
                reg_update_reg(updated, aux, searches_y, y);
                reg_set_size(updated);

                //file descriptor deve estar no comeco do registro
                fseek(binfp, -(aux->tamanhoRegistro + SIZE_TOPO_REGISTRO), SEEK_CUR);

                if (updated->tamanhoRegistro > aux->tamanhoRegistro)
                { // nao cabe

                    //remocao logica do atual
                    char removido = '1';

                    //correcao do valor do comeco da lista encadeada em arquivo
                    size_t curr = ftell(binfp);

                    //escrita dos proximos dois campos
                    fwrite(&(removido), 1, sizeof(char), binfp);
                    fwrite(&(aux->tamanhoRegistro), 1, sizeof(int), binfp);

                    //antiga localizacao do byteoffset da lista encadeada e passada
                    fwrite(&(h->topoLista), 1, sizeof(size_t), binfp);

                    //colocar aqui pois sera escrito ao final
                    updated->proxLista = h->topoLista;

                    //topo da lista do header e atualizado para o atual (removido)
                    h->topoLista = curr;

                    //chamada a funcao que realiza a insercao na lista encadeada em arquivo
                    bin_insert_file_linked_list(binfp, h, updated);

                    //file descriptor deve estar no final do registro atual & comeco do proximo
                    fseek(binfp, (curr + SIZE_TOPO_REGISTRO + aux->tamanhoRegistro), SEEK_SET); 
                }
                else
                { // cabe

                    //calculo da quantidade de lixo que deve ser escrita 
                    int trashSize = aux->tamanhoRegistro - updated->tamanhoRegistro;

                    //embora o registro tenha um tamanho diferente, se cabe, temos de escrever o maior tamanho (antigo)
                    updated->tamanhoRegistro = aux->tamanhoRegistro;
                    bin_write_new_register(binfp, updated, trashSize);
                    //file descriptor ja esta no final do registro atual & comeco do proximo
                }

                //se ao menos um dos campos que sao de interesse do header mudaram, atualiza
                if ((aux->codEstacao != updated->codEstacao) || (aux->codProxEstacao != updated->codProxEstacao) || (strcmp(aux->nomeEstacao, updated->nomeEstacao) != 0))
                {

                    //checar para os pares como adaptar para essa logica !hasPair && reg->codProxEstacao != -1
                    if (list_remove_equivalent(l, aux) != TRUE) //tem de encontrar ao menos o proprio registro
                    {
                        printf("Erro na funcao list_remove_equivalent. Encerrando..\n");
                        return FALSE;
                    }

                    //A situacao neste caso (update) e um pouco mais complexa:
                    //temos de checar (para o header) nao somente com relacao
                    //aos valores do campo antigo, como tambem do novo

                    int OldNameNotUnique = list_find_by_name(l, aux->nomeEstacao);
                    int NewNameNotUnique = list_find_by_name(l, updated->nomeEstacao);

                    int OldPairNotUnique = list_find_by_pair(l, aux->codEstacao, aux->codProxEstacao);
                    int NewPairNotUnique = list_find_by_pair(l, updated->codEstacao, updated->codProxEstacao);

                    // tomar proveito do retorno se um inteiro (0 se falso, 1 se verdadeiro)
                    // para fazer a conta sem o uso de condicionais. O valor adicionado é 1
                    // se o nome retirado NAO for unico E o novo nome FOR unico, similarmente
                    // para -1. Se ambos valores forem ou nao unicos ao mesmo tempo, o valor
                    // no header permanece o mesmo. Similarmente para nroParesEstacoes
                    h->nroEstacoes = h->nroEstacoes + (OldNameNotUnique - NewNameNotUnique);
                    h->nroParesEstacoes = h->nroParesEstacoes + (OldPairNotUnique - NewPairNotUnique);

                    list_append(l, updated);
                }
                else
                {
                    reg_delete(updated); // nao pode deletar caso tenha alguma mudanca pois estamos usando o registro na lista encadeada
                }
            }
        }
    }
    return TRUE;
}

Graph bin_set_graph(FILE * binFile, bool directioned){
/*------------------------------------------
Realiza a leitura do arquivo de dados de forma a gerar o Objeto da classe
grafo, utilizada para as funcionalidades. Gera de forma direcionada ou nao e retorna o grafo.

@Params: binfp -> ponteiro do arquivo binário (em modo rb+); directioned -> boolean indicando grafo direcionado ou não;
@Return: graph -> grafo gerado
------------------------------------------*/

    REGISTER  * reg;
    Graph graph(directioned);

    while (!feof(binFile)){
        reg = bin_read_register(binFile); // Função que retorna o registro lido.
        if (reg != NULL){ 
            // checar removida
            graph.addNewVertex(reg->nomeEstacao); // ja verifica se existe

            if(reg->codProxEstacao != NULLVAL){ // Se existe uma proxima estacao, cria a aresta correspondente
                size_t currPosition = ftell(binFile);

                // usa a estrutura search com o valor esperado
                SEARCH * search = new SEARCH;
                search->fieldCode = 1;
                search->numValue = reg->codProxEstacao;
                
                //pesquisa no arquivo pelo valor de codproxestacao, pega o primeiro elemento (pressupoe que existe apenas 1 registro com esse código)
                REGISTER * aux = list_pop_front(bin_search_all_registers(binFile,search,1));

                // cria uma nova aresta baseando-se na primeira conexao encontrada 
                if(aux != NULL){
                    graph.addNewEdge(reg->nomeEstacao,aux->nomeEstacao,reg->distProxEstacao,reg->nomeLinha);
                    reg_delete(aux);
                }
                
                //deleta o registro e retorna o fp para a posicao onde estava antes da busca
                fseek(binFile,currPosition,SEEK_SET);
            }

            if(reg->codEstIntegracao != NULLVAL){ // Se existe uma estacao de integração, cria a aresta correspondente
                size_t currPosition = ftell(binFile);

                // usa a estrutura search com o valor esperado
                SEARCH * search = new SEARCH;
                search->fieldCode = 1;
                search->numValue = reg->codEstIntegracao;

                //pesquisa no arquivo pelo valor de codEstIntegracao, pega o primeiro elemento (pressupoe que existe apenas 1 registro com esse código)
                REGISTER * aux = list_pop_front(bin_search_all_registers(binFile,search,1));

                if(aux != NULL && strcmp(aux->nomeEstacao,reg->nomeEstacao) != 0){
                    // Cria a aresta correspondente
                    graph.addNewEdge(reg->nomeEstacao,aux->nomeEstacao,0,"Integracao");
                    reg_delete(aux);
                }

                //deleta o registro e retorna o fp para a posicao onde estava antes da busca
                fseek(binFile,currPosition,SEEK_SET);
            }
        }
    }

    return graph;
}

//-----Funcoes de auxilio na manipulacao de valores importantes ao arquivo binario-----
HEADER *bin_header_create()
{
    /*------------------------------------------
Aloca dinamicamente uma nova estrutura de cabeçalho com valores padrões. 
Caso não consiga fazer a alocação, emite uma saída 2 para o programa.

@Params: nenhum
@Return: ponteiro para a estrutura criada
------------------------------------------*/
    HEADER *ret = (HEADER *)malloc(sizeof(HEADER));
    if (ret == NULL)
    {
        exit(MEM_ALLOC_ERROR);
    }

    ret->status = '0';         // Um novo cabeçalho sempre é criado para um arquivo de escrita
    ret->topoLista = NULLVAL;  // Valor padrão para o topo da lista
    ret->nroEstacoes = 0;      // Valor padrão para o nro de Estações
    ret->nroParesEstacoes = 0; // Valor padrão para nro de Pares

    return ret;
}

void bin_atualiza_header(LIST *l, REGISTER *reg, HEADER *h)
{
    int hasName = list_find_by_name(l, reg->nomeEstacao);
    int hasPair = list_find_by_pair(l, reg->codEstacao, reg->codProxEstacao);

    if (!hasName || !hasPair)
    {
        list_append(l, reg);

        if (!hasName) // Verificacao dos valores de nroEstacoes
        {
            h->nroEstacoes = h->nroEstacoes + 1;
        }

        if (!hasPair && reg->codProxEstacao != NULLVAL) // Verificacao dos valores de nroParesEstacoes
        {
            h->nroParesEstacoes = h->nroParesEstacoes + 1;
        }
    }

    return;
}

int bin_verify_search(REGISTER *reg, SEARCH s)
{
    /*------------------------------------------
 Recebe um registro e um filtro de busca para verificar se o registro atende  

 @Params: reg -> registro a ser verificado; s -> seletor de busca
 @Return: TRUE -> o registro coincide com o valor de busca; FALSE -> o registro é fora do escolpo de busca
------------------------------------------*/
    switch (s.fieldCode)
    {       // Switch nos códigos dos campos de busca
    case 1: // CodEstacao
        return (reg->codEstacao == s.numValue) ? TRUE : FALSE;
    case 2: // nomeEstacao
        return (strcmp(s.stringValue, reg->nomeEstacao) == 0) ? TRUE : FALSE;
    case 3: // codLinha
        return (reg->codLinha == s.numValue) ? TRUE : FALSE;
    case 4: // nomeLinha
        return (strcmp(s.stringValue, reg->nomeLinha) == 0) ? TRUE : FALSE;
    case 5: // codProxEstacao
        return (reg->codProxEstacao == s.numValue) ? TRUE : FALSE;
    case 6: // distProxEstacao
        return (reg->distProxEstacao == s.numValue) ? TRUE : FALSE;
    case 7: // codLinhaIntegracao
        return (reg->codLinhaIntegracao == s.numValue) ? TRUE : FALSE;
    case 8: // codEstIntegracao
        return (reg->codEstIntegracao == s.numValue) ? TRUE : FALSE;
    default: // valor inválido no campo de busca
        return FALSE;
    }
}

void bin_delete_search(SEARCH *search, int tam)
{
    /*------------------------------------------
 Recebe um vetor de SEARCH e desaloca tudo que pertemne a ele

 @Params: search -> vetor a ser desalocado; tam -> tamanho do vetor
 @Return: nenhum
------------------------------------------*/
    if (search == NULL)
    {
        return;
    }

    for (int i = 0; i < tam; i++)
    {
        if (search[i].stringValue != NULL)
        {
            free(search[i].stringValue);
        }
    }

    free(search);

    return;
}

