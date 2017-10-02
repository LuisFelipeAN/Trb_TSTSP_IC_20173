#include <iostream>
#include "Programa.h"
using namespace std;

int main(int argc, char** args)
{
    atexit(desalocaMemoria);
    FILE* arquivo = stdin;
    if ( argc == 3 ) {
        arquivo = fopen(args[1], "r");
        if(!arquivo){
            fprintf(stderr, "Arquivo invalido!");
            exit(1);
        }

    } else if( argc == 2){
                srand(atoi(args[1]));

            }else if(argc > 3){
                     fprintf(stderr, "Argumento invalido exemplo de uso:\n");
                     fprintf(stderr, "nomeExecutavel nomeArquivo semente\n");
                     fprintf(stderr, "nomeExecutavel semente < arquivo  \n");
                      exit(1);
                  }

    inicializa(arquivo);
    construtivo();
    fprintf(stdout,"Custo da Solucao Antes da Busca Local: %d\n",calculaCustoSolucao());
    buscaLocal();
    fprintf(stdout,"Custo da Solucao Depois da Busca Local: %d\n",calculaCustoSolucao());

    return 0;
}
