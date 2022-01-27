/*------------------------------------------
    NOME1: Diego Fleury Correa De Moraes
    NUSP:  11800584
    NOME2: Gabriel Sotto Rodrigues
    NUSP2: 11800952
------------------------------------------*/ 

#include <iostream>
#include <queue>

#include "command.h"


using namespace std;


int main(){
/*------------------------------------------
Funcao principal do programa

@Params: nenhum
@Return: 0 (programa finalizado com sucesso)
------------------------------------------*/

    int cmd;
    cin >> cmd;

    switch(cmd){
        case 1:
            cmd_create();
            break;
        case 2:
            cmd_select_all();
            break;
        case 3:
            cmd_select_where();
            break;
        case 4:
            cmd_delete();
            break;
        case 5:
            cmd_insert();
            break;
        case 6:
            cmd_update();
            break;
        case 7:
            cmd_create_graph();
            break;
        case 8:
            cmd_search_path();
            break;
        case 9:
            cmd_search_cycle();
            break;
        case 10:
            cmd_min_tree();
            break;
        case 11:
            cmd_search_all_paths();
            break;
        default:
            exit(INVALID_CMD_ERROR);
    }
    
    // return EXIT_SUCCESS;
}