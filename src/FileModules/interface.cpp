/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/ 
#include "interface.h"

/*-----Exibicao de valores de forma organizada na saida padrao-----*/
void inter_print_binary(char *name)
{
    /*------------------------------------------
Cópia da função BinarioNaTela() com o nome modificado para manter o padrão do código.

@Params: name -> nome do arquivo binário
@Return: nenhum
------------------------------------------*/
    unsigned long i, cs;
    unsigned char *mb;
    size_t fl;
    FILE *fs;
    if (name == NULL || !(fs = fopen(name, "rb")))
    {
        fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função inter_print_binary): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
        return;
    }
    fseek(fs, 0, SEEK_END);
    fl = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    mb = (unsigned char *)malloc(fl);
    fread(mb, 1, fl, fs);

    cs = 0;
    for (i = 0; i < fl; i++)
    {
        cs += (unsigned long)mb[i];
    }
    printf("%lf\n", (cs / (double)100));
    free(mb);
    fclose(fs);
}

void inter_print_header(HEADER * h)
{
/*------------------------------------------
Imprime os dados do cabeçalho

@Params: h -> ponteiro para o cabeçalho
@Return: nenhum
------------------------------------------*/
	printf("status: %c |",h->status);
	printf("topoDaLista: %ld |",h->topoLista);
	printf("nroEstacoes: %d |",h->nroEstacoes);
	printf("nroPares: %d\n",h->nroParesEstacoes);

	return;
}

void inter_print_register(REGISTER * reg)
{
/*------------------------------------------
Imprime os dados do registro

@Params: reg -> ponteiro para o registro
@Return: nenhum
------------------------------------------*/
	printf("%c %d %ld | ",reg->removido,reg->tamanhoRegistro,reg->proxLista);
	inter_print_line(reg);
}

void inter_print_line(REGISTER * reg)
{
/*------------------------------------------
Imprime a tupla na forma requisitada para o problema

@Params: reg -> ponteiro para o registro
@Return: nenhum
------------------------------------------*/
	printf("%d ",reg->codEstacao);
	printf("%s ",reg->nomeEstacao);
	printf("%d ",reg->codLinha);
	printf("%s ",reg->nomeLinha);
	if(reg->codProxEstacao != NULLVAL){
		printf("%d ",reg->codProxEstacao);
	}else{
		printf("NULO ");
	}

	if(reg->distProxEstacao != NULLVAL){
		printf("%d ",reg->distProxEstacao);
	}else{
		printf("NULO ");
	}

	if(reg->codLinhaIntegracao != NULLVAL){
		printf("%d ",reg->codLinhaIntegracao);
	}else{
		printf("NULO ");
	}

	if(reg->codEstIntegracao != NULLVAL){
		printf("%d\n",reg->codEstIntegracao);
	}else{
		printf("NULO\n");
	}
}