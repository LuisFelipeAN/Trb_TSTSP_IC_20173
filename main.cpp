#include <iostream>
#include "Programa.h"
using namespace std;

int main(int argc, char** args)
{

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
    desalocaMemoria();
    return 0;
}
