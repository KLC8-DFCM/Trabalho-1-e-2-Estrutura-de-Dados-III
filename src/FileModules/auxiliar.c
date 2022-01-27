/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/

#include "auxiliar.h"

/*-----Funções para manipulação e processamento de dados-----*/

//-----Tratamento de dados realizado nas buscas-----
int aux_string_to_code(char *str)
{
    /*------------------------------------------
Função que converte os possíveis nome de campo para um valor numérico

@Params: str -> nome do campo
@Return: valor do código (0 -> nenhum campo com esse nome encontrado)
------------------------------------------*/
    if (strcmp(str, "codEstacao") == 0)
    {
        return 1;
    }
    else if (strcmp(str, "nomeEstacao") == 0)
    {
        return 2;
    }
    else if (strcmp(str, "codLinha") == 0)
    {
        return 3;
    }
    else if (strcmp(str, "nomeLinha") == 0)
    {
        return 4;
    }
    else if (strcmp(str, "codProxEstacao") == 0)
    {
        return 5;
    }
    else if (strcmp(str, "distProxEstacao") == 0)
    {
        return 6;
    }
    else if (strcmp(str, "codLinhaIntegra") == 0)
    {
        return 7;
    }
    else if (strcmp(str, "codEstacaoIntegra") == 0)
    {
        return 8;
    }

    return 0;
}

int aux_is_a_string_field(int code)
{
    /*------------------------------------------
Função que recebe o código de um campo e retorna se é um campo de string ou não

@Params: code -> código do nome do campo
@Return: TRUE -> é um campo de string (nomeEstação ou nomeLinha); FALSE-> não é
------------------------------------------*/
    if (code == 2 || code == 4)
    {
        return TRUE;
    }

    return FALSE;
}

//-----Processamento de dados da entrada padrao-----
void aux_read_searches(SEARCH *searches, int qnt)
{
    /*------------------------------------------
Função auxiliar para ler todos os campos de busca presentes na tela

@Params: code -> código do nome do campo
@Return: nenhum
------------------------------------------*/
    char fieldName[STR_DEFAULT_SIZE], aux[STR_DEFAULT_SIZE]; //Armazena o nome atual do campo, para convertê-lo para um código numérico
    for (int i = 0; i < qnt; i++)
    {

        scanf("%s", fieldName);                                //Lê o nome do campo
        searches[i].fieldCode = aux_string_to_code(fieldName); //Converte o nome do campo em um código numérico (simplifica o algoritmo de busca)
        aux_scan_quote_string(aux);                            // Lê o valor de busca

        if (aux_is_a_string_field(searches[i].fieldCode))
        { // Verifica se o campo de busca é um campo de texto

            searches[i].stringValue = (char *)malloc(STR_DEFAULT_SIZE * sizeof(char)); //Aloca uma string da estrutura de busca
            strcpy(searches[i].stringValue, aux);

            //valor default para numero
            searches[i].numValue =NULLVAL;
        }
        else
        { // Qualquer outro campo de dados nada mais é que um número

            if (strcmp(aux, "") == 0)
            { // Se for um campo nulo, coloca o valor de busca como -1
                searches[i].numValue = NULLVAL;
            }
            else
            { // Caso não seja nulo, apenas converte a string para inteiro
                searches[i].numValue = atoi(aux);
            }
            //valor default para string
            searches[i].stringValue = NULL;
        }
    }
    return;
}

void aux_scan_reg(REGISTER *reg)
{
    /*------------------------------------------
Função auxiliar para ler da entrada padrão um novo registro (utilizada na funcionalidade de insert)

@Params: reg -> ponteiro para o registro que gravará a leitura
@Return: nenhum
------------------------------------------*/
    //Leitura do codEstação
    scanf("%d", &(reg->codEstacao));

    char aux[STR_DEFAULT_SIZE]; // Auxiliar para gravar qualquer leitura de texto
    int size;                   // Armazena o tamanho da string lida

    // Leitura do nome da Estação
    aux_scan_quote_string(aux);

    size = strlen(aux) + 1;                                 // Calcula o tamanho da string
    reg->nomeEstacao = (char *)malloc(size * sizeof(char)); // Aloca apenas o tamanho necessário
    strcpy(reg->nomeEstacao, aux);                          // Copia a string lida para o registro

    // Leitura codLinha
    aux_scan_quote_string(aux);
    reg->codLinha = (strcmp(aux, "") == 0) ? NULLVAL : atoi(aux);

    //Leitura nomeLinha
    aux_scan_quote_string(aux);

    size = strlen(aux) + 1;                               // Calcula o tamanho da string
    reg->nomeLinha = (char *)malloc(size * sizeof(char)); // Aloca apenas o tamanho necessário
    strcpy(reg->nomeLinha, aux);                          // Copia a string lida para o registro

    //Leitura codProxEst
    aux_scan_quote_string(aux);
    reg->codProxEstacao = (strcmp(aux, "") == 0) ? NULLVAL: atoi(aux);

    //Leitura distProxEst
    aux_scan_quote_string(aux);
    reg->distProxEstacao = (strcmp(aux, "") == 0) ? NULLVAL : atoi(aux);

    //Leitura codLinhaIntegra
    aux_scan_quote_string(aux);
    reg->codLinhaIntegracao = (strcmp(aux, "") == 0) ? NULLVAL: atoi(aux);

    //Leitura codEstIntegra
    aux_scan_quote_string(aux);
    reg->codEstIntegracao = (strcmp(aux, "") == 0) ? NULLVAL: atoi(aux);

    return;
}

void aux_scan_quote_string(char *str)
{

    /*
	*	Use essa função para ler um campo string delimitado entre aspas (").
	*	Chame ela na hora que for ler tal campo. Por exemplo:
	*
	*	A entrada está da segunsignede forma:
	*		nomeDoCampo "MARIA DA SILVA"
	*
	*	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
	*		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
	*		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
	*
	*/

    char R;

    while ((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

    if (R == 'N' || R == 'n')
    { // campo NULO
        getchar();
        getchar();
        getchar();       // ignorar o "ULO" de NULO.
        strcpy(str, ""); // copia string vazia
    }
    else if (R == '\"')
    {
        if (scanf("%[^\"]", str) != 1)
        { // ler até o fechamento das aspas
            strcpy(str, "");
        }
        getchar(); // ignorar aspas fechando
    }
    else if (R != EOF)
    { // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
        str[0] = R;

        int i = 1;
        while ((R = getchar()) != EOF && R != '\n' && R != ' ')
        {
            str[i] = R;
            i++;
        }

        str[i] = '\0';
    }
    else
    { // EOF
        strcpy(str, "");
    }
}

#ifdef linux
char *dynamic_scan_quote_string()
{
    char R;
    char *str = NULL, *aux = NULL;

    while ((R = getchar()) != EOF && isspace(R))
        ; // ignorar espaços, \r, \n...

    if (R == 'N' || R == 'n')
    {
        scanf("%*s");
        str = (char *)malloc(sizeof(char) * 4);

        if (str == NULL)
        {
            return NULL;
        }

        strcpy(str, "NULO");
    }
    else if (R == '\"')
    {
        scanf("%m[^\"]", &str);
        getchar();
    }
    else if (R != EOF)
    {
        scanf("%ms", &aux);
        str = (char *)malloc(sizeof(char) * (strlen(aux) + 1));

        if (str == NULL)
        {
            if (aux != NULL)
                free(aux);
            return NULL;
        }

        strncpy(str, &R, sizeof(char));
        strcpy(str + 1, aux);

        free(aux);
    }

    return str;
}
#endif