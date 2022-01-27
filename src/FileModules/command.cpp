/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/
#include "command.h"

#include <string>

using namespace std;


/*-----Funcionalidades implementadas-----*/
void cmd_create()
{
    /*------------------------------------------
Essa função é responsável por executar o comando de criação de uma nova tabela (arquivo binário).

@Params: nenhum
@Return: nenhum
------------------------------------------*/
    char CSVFileName[STR_DEFAULT_SIZE];
    char binFileName[STR_DEFAULT_SIZE];

    scanf("%s %s", CSVFileName, binFileName);

    //Abertura do arquivo de origem
    FILE *CSVFile = fopen(CSVFileName, "r");
    if (CSVFile == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    //Abertura do arquivo destino
    FILE *binFile = fopen(binFileName, "wb");
    if (binFile == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    //Manipulações dos cabeçalhos
    CSV_skip_header(CSVFile);        // Posiciona o ponteiro de CSVFile logo apos o header
    HEADER *h = bin_header_create(); // Cria um cabeçalho com todos os dados nulos

    LIST *l = list_create(); // Cria uma lista para verificar repetição de nomes

    bin_write_header(binFile, h); // Escreve inicialmente o header

    REGISTER *reg;
    while (!feof(CSVFile)) // Enquanto nao chegou ate o final do arquivo
    {
        reg = CSV_read_register(CSVFile); // Recebe cada linha do CSV (na forma de struct registro)
        reg_set_size(reg);

        bin_atualiza_header(l,reg,h);   // Escreve os valores de nroEstacoes & nroParesEstacoes

        bin_write_new_register(binFile, reg, 0); // Escrita no arquivo binario do valor
    }

    //Atualizar o header antes de escrever, e desaloca a struct
    bin_write_header(binFile, h);
    free(h);

    //Marca o arquivo como atualizado
    bin_set_status(binFile, '1');

    //Fechamento dos arquivos utilizados
    fclose(CSVFile);
    fclose(binFile);

    //Impressao do valor de verificacao
    inter_print_binary(binFileName);
}

void cmd_select_all()
{
    /*------------------------------------------
Essa função é responsável por executar o comando de seleção geral de uma tabela (arquivo binário).

@Params: nenhum
@Return: nenhum
------------------------------------------*/
    //Leitura do nome do arquivo
    char binFileName[STR_DEFAULT_SIZE];
    scanf("%s", binFileName);

    //Abertura do Arquvivo
    FILE *binFile = fopen(binFileName, "rb");
    if (binFile == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    //Leitura do cabeçalho
    HEADER *h = bin_read_header(binFile);

    //Utiliza o cabeçalho para determinar a existencia de registros no arquivo
    if (h->nroEstacoes == 0 || h->nroEstacoes == NULLVAL)
    { 
        printf("Registro inexistente.\n");
        return;
    }

    //Leitura e impressão dos registros
    REGISTER *reg;
    while (!feof(binFile))
    {
        reg = bin_read_register(binFile); // Função que retorna o registro lido.
        if (reg != NULL)
        { // bin_read_register() pode retornar nulo (caso registro esteja logicamente removido)
            inter_print_line(reg);
            reg_delete(reg);
        }
    }

    // Remove o header
    free(h);

    //Fechamento do arquivo
    fclose(binFile);
    return;
}

void cmd_select_where()
{
    /*------------------------------------------
Essa função é responsável por executar o comando de seleção com filtro de uma tabela (arquivo binário).

@Params: nenhum
@Return: nenhum
------------------------------------------*/
    //Leitura do nome do arquivo
    char binFileName[STR_DEFAULT_SIZE];
    scanf("%s", binFileName);

    //Abertura do Arquvivo
    FILE *binFile = fopen(binFileName, "rb");
    if (binFile == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Numero de campos para busca
    int n;
    scanf("%d", &n);

    // Criação da matriz de strings para armazenar campos e valores
    SEARCH *searches = (SEARCH *)malloc(n * sizeof(SEARCH)); //Armazena todos os valores de busca
    aux_read_searches(searches, n);

    //Leitura do Cabeçalho do arquivo
    HEADER *h = bin_read_header(binFile);
    if (h->nroEstacoes <= 0)
    { // Caso o arquivo não possua registros não-removidos
        printf("Registro inexistente.\n");
        return;
    }

    LIST *results = bin_search_all_registers(binFile, searches, n); // Realiza a busca no arquivo e retorna uma lista de resultados

    if (results->qnt == 0)
    { // Caso nenhum registro atenda aos resultados
        printf("Registro inexistente.\n");
        return;
    }

    // Impressão de todos os resultados da lista
    while (results->qnt > 0)
    {
        REGISTER *aux = list_pop_front(results); // Resgata  o valor do topo da lista

        inter_print_line(aux); // Imprime o registro
        reg_delete(aux);       // Desaloca o registro
    }

    // Desaloca as entradas de busca
    bin_delete_search(searches, n);

    // Desaloca a lista
    list_delete(results);

    // Desaloca o header
    free(h);

    // Fechamento do arquivo
    fclose(binFile);
    return;
}

void cmd_delete()
{
    /*------------------------------------------
Essa função é responsável por executar o comando de delete (arquivo binário).

@Params: nenhum
@Return: nenhum
------------------------------------------*/
    //Leitura do nome do arquivo
    char binFileName[STR_DEFAULT_SIZE];
    scanf("%s", binFileName);

    //Abertura do Arquvivo
    FILE *binFile = fopen(binFileName, "rb+");

    if (binFile == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // Atualização do status do arquivo e leitura do cabeçalho
    bin_set_status(binFile, '0');
    HEADER *h = bin_read_header(binFile);

    // Numero de exclusões a serem feitas
    int n;
    scanf("%d", &n);

    SEARCH *searches;
    //Loop para remocoes
    for (int i = 0; i < n; i++)
    {
        // Numero de criterios de busca para exclusão
        int x;
        scanf("%d", &x);

        searches = (SEARCH *)malloc(x * sizeof(SEARCH)); // Armazena todos os valores de busca
        aux_read_searches(searches, x);                  // Leitura de todos os campos de busca

        if (bin_remove(binFile, h, searches, x))
        {// Caso ele tenha conseguido percorrer o arquivo para remoção
            fseek(binFile, SIZE_HEADER, SEEK_SET); // Volta o arquivo para o primeiro registro, para proxima remoção
        }
        else
        {
            printf("Erro na remocao dos registros. Verificar bin_remove()..");
            exit(BIN_ERROR);
        }
        bin_delete_search(searches, x);
    }

    //Atualizar o header antes de escrever, e desaloca a struct
    bin_write_header(binFile, h);
    free(h);

    //Seta o status como atualizado
    bin_set_status(binFile, '1');

    //Fecha o arquivo
    fclose(binFile);

    //binarioNaTela
    inter_print_binary(binFileName);
    return;
}

void cmd_insert()
{
    /*------------------------------------------
Essa função é responsável por executar o comando de insert (arquivo binário).

@Params: nenhum
@Return: nenhum
------------------------------------------*/
    //Leitura do arquivo binário
    char binFileName[STR_DEFAULT_SIZE];
    scanf("%s", binFileName);

    //Abertura do Arquivo
    FILE *binFile = fopen(binFileName, "rb+");

    //Checagem de validade do descritor de arquivo
    if (binFile == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    //Leitura e atualizacao do header (status = desatualizado)
    HEADER *h = bin_read_header(binFile);
    bin_set_status(binFile, '0');

    //Leitura quantidade de inserções
    int n;
    scanf("%d", &n);

    //Lista usada para fins de atualizacao dos valores do header (campos nroEstacoes & nroParesEstacoes)
    //passado o valor 0 para nao checar nenhuma condicao (dado que e nulo inclusive)
    LIST *l = bin_search_all_registers(binFile, NULL, 0); 

    for (int i = 0; i < n; i++)
    {   
        //Leitura de registro em registro, passando os valores
        REGISTER *reg = reg_create();
        aux_scan_reg(reg);
        reg_set_size(reg);

        //Atualiza (se preciso) o header para os campos nroEstacoes & nroParesEstacoes
        bin_atualiza_header(l,reg,h);

        //Insercao ocorre nos registros marcados como logicamente removidos
        bin_insert_file_linked_list(binFile, h, reg);
    }

    //Desaloca a lista
    list_delete(l);

    //Atualizar o header antes de escrever, e desaloca a struct
    bin_write_header(binFile, h);
    free(h);

    //Arquivo atualizado/coerente
    bin_set_status(binFile, '1');

    //Fecha o arquivo
    fclose(binFile);

    //binarioNaTela
    inter_print_binary(binFileName);

    return;
}

void cmd_update()
{
/*------------------------------------------
Essa função é responsável por executar o comando de update (arquivo binário).

@Params: nenhum
@Return: nenhum
------------------------------------------*/
    //Leitura do arquivo binário
    char binFileName[STR_DEFAULT_SIZE];
    scanf("%s", binFileName);

    //Abertura do Arquivo
    FILE *binFile = fopen(binFileName, "rb+");

    //Checagem da validade do descritor de arquivo
    if (binFile == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    //Leitura e atualizacao do header
    HEADER *h = bin_read_header(binFile);
    bin_set_status(binFile, '0');

    //Leitura quantidade de updates
    int n;
    scanf("%d", &n);

    //Declaracao de variaveis de pesquisa (x -> especificacao dos registros a serem pesquisados, y ->  especificacao das mudancas a serem feitas )
    SEARCH *searches_x, *searches_y;

    //usado para atualizacao do header, similar ao implementado no insert
    LIST *l = bin_search_all_registers(binFile, NULL, 0); 

    //Inicia no comeco do arquiv
    fseek(binFile, SIZE_HEADER, SEEK_SET);
    int x, y;
    for (int i = 0; i < n; i++)
    { // loop que realiza a leitura de n pares x, y
        scanf("%d", &x);
        searches_x = (SEARCH *)malloc(x * sizeof(SEARCH)); //Armazena todos os valores de busca
        aux_read_searches(searches_x, x);                  // Leitura de todos os campos de busca

        scanf("%d", &y);
        searches_y = (SEARCH *)malloc(y * sizeof(SEARCH)); //Armazena todos os valores de busca
        aux_read_searches(searches_y, y);                  // Leitura de todos os campos de busca

        if (bin_update(binFile, h, searches_x, x, searches_y, y, l)) // Caso tenha conseguido percorrer o arquivo e atualizado com sucesso
        {
            fseek(binFile, SIZE_HEADER, SEEK_SET); // Volta o arquivo para o primeiro registro, para proxima rodada de atualização
        }
        else
        {
            printf("Erro na atualizacao dos registros. Verificar bin_update()..");
            exit(BIN_ERROR);
        }
        bin_delete_search(searches_x, x);
        bin_delete_search(searches_y, y);
    }
    //Desaloca a lista
    list_delete(l);

    //Atualizar o header antes de escrever, e desaloca a struct
    bin_write_header(binFile, h);
    free(h);

    //Arquivo coerente 
    bin_set_status(binFile, '1');

    //Fecha o arquivo
    fclose(binFile);

    //binarioNaTela
    inter_print_binary(binFileName);

    return;
}

void cmd_create_graph(){
/*------------------------------------------
Essa função é responsável por criar um grafo e imprimir com base no 
arquivo binario que o usuario escolher.

@Params: nenhum
@Return: nenhum
------------------------------------------*/

    char binFileName[STR_DEFAULT_SIZE];
    scanf("%s", binFileName);

    //Abertura do Arquivo
    FILE *binFile = fopen(binFileName, "rb");

    //Checagem da validade do descritor de arquivo
    if (binFile == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // ignora o cabecalho
    fseek(binFile,SIZE_HEADER,SEEK_SET);
    
    Graph graph = bin_set_graph(binFile,true);

    fclose(binFile);
    
    // imprime na tela o grafo
    graph.printGraph();

    return; 
}

void cmd_search_path(){
/*------------------------------------------
Essa função é responsável por criar um grafo e encontrar a árvore 
@Params: nenhum
@Return: nenhum
------------------------------------------*/
    char binFileName[STR_DEFAULT_SIZE];
    scanf("%s", binFileName);

    //Abertura do Arquivo
    FILE *binFile = fopen(binFileName, "rb");

    //Checagem da validade do descritor de arquivo
    if (binFile == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // ignora o cabecalho
    fseek(binFile,SIZE_HEADER,SEEK_SET);
    
    Graph graph = bin_set_graph(binFile,true);

    fclose(binFile);
    
    // ler os stdin
    char tmp[STR_DEFAULT_SIZE];
    char start[STR_DEFAULT_SIZE];
    char end[STR_DEFAULT_SIZE];

    scanf("%s ",tmp);
    aux_scan_quote_string(start);

    scanf("%s ",tmp);
    aux_scan_quote_string(end);

    graph.searchPath(start,end);
}

void cmd_search_cycle(){
/*------------------------------------------
Essa função é responsável por encontrar um ciclo dado um ponto inicial

@Params: nenhum
@Return: nenhum
------------------------------------------*/
    char binFileName[STR_DEFAULT_SIZE];
    scanf("%s", binFileName);

    //Abertura do Arquivo
    FILE *binFile = fopen(binFileName, "rb");

    //Checagem da validade do descritor de arquivo
    if (binFile == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // ignora o cabecalho
    fseek(binFile,SIZE_HEADER,SEEK_SET);

    Graph graph = bin_set_graph(binFile,true);

    fclose(binFile);

    char tmp[STR_DEFAULT_SIZE];
    char target[STR_DEFAULT_SIZE];

    scanf("%s ",tmp);
    aux_scan_quote_string(target);

    graph.searchCycle(target);

}

void cmd_min_tree(){
/*------------------------------------------
Essa função é responsável por criar um grafo e encontrar a árvore
mínima geradora dele com base no arquivo passado pelo usuario e pelo
vertice de partida.

@Params: nenhum
@Return: nenhum
------------------------------------------*/
    char binFileName[STR_DEFAULT_SIZE];
    scanf("%s", binFileName);

    //Abertura do Arquivo
    FILE *binFile = fopen(binFileName, "rb");

    //Checagem da validade do descritor de arquivo
    if (binFile == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // ignora o cabecalho
    fseek(binFile,SIZE_HEADER,SEEK_SET);
    
    Graph graph = bin_set_graph(binFile,false);

    fclose(binFile);

    char tmp[STR_DEFAULT_SIZE];
    char start[STR_DEFAULT_SIZE];

    scanf("%s ",tmp);
    aux_scan_quote_string(start);

    graph.searchMinimalTree(start);

}

void cmd_search_all_paths(){
    char binFileName[STR_DEFAULT_SIZE];
    scanf("%s", binFileName);

    //Abertura do Arquivo
    FILE *binFile = fopen(binFileName, "rb");

    //Checagem da validade do descritor de arquivo
    if (binFile == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    // ignora o cabecalho
    fseek(binFile,SIZE_HEADER,SEEK_SET);
    
    Graph graph = bin_set_graph(binFile,true);

    fclose(binFile);
    
    // ler os stdin
    char tmp[STR_DEFAULT_SIZE];
    char start[STR_DEFAULT_SIZE];
    char end[STR_DEFAULT_SIZE];

    scanf("%s ",tmp);
    aux_scan_quote_string(start);

    scanf("%s ",tmp);
    aux_scan_quote_string(end);

    graph.searchAllPaths(start,end);
}