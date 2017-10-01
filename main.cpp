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
                     fprintf(stderr, "Argumento invalido!");
                      exit(1);
                  }

    inicializa(arquivo);
    construtivo();
    imprimeVerificacaoViabilidade();
    imprimeTabus();
    fprintf(stdout,"Custo da Solucao: %d\n",calculaCustoSolucao());
    imprimeSolucao();
    buscaLocal();
    imprimeTabus();

    return 0;
}
