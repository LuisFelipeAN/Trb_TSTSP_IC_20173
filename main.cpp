#include <iostream>
#include "Programa.h"
using namespace std;
struct NoSolucao{
    No* solucao;
    NoSolucao *proxima;
};

int main(int argc, char** args)
{

    FILE* arquivoEntrada = stdin;
    FILE* arquivoSaida = stdout;
    if (argc == 4){
        arquivoEntrada = fopen(args[1], "r");
        arquivoSaida = fopen(args[2], "w");
            if(!arquivoEntrada){
                fprintf(stderr, "Arquivo invalido!");
                exit(1);
            }
        srand(atoi(args[3]));
    } else if ( argc == 3 ) {
            arquivoEntrada = fopen(args[1], "r");
            if(!arquivoEntrada){
                fprintf(stderr, "Arquivo invalido!");
                exit(1);
            }
            srand(atoi(args[2]));
        } else if( argc == 2){
                srand(atoi(args[1]));

            }else if(argc > 3){
                     fprintf(stderr, "Argumento invalido exemplo de uso:\n");
                     fprintf(stderr, "<exe> <arqEntrada> <arqSaida> <semente>\n");
                     fprintf(stderr, "<exe> <arqEntrada> <semente> \n");
                     fprintf(stderr, "<exe> <semente>\n");
                      exit(1);
                  }

    inicializa(arquivoEntrada,arquivoSaida);
    NoSolucao *ultimaSolucao;
    ultimaSolucao = new NoSolucao();
    ultimaSolucao->proxima = NULL;
    ultimaSolucao->solucao = construtivo();

    //int minimo=99999999;
    No* melhorSolucao=construtivo();
    /*for(int i=0;i<1;i++){
        No* solucao = construtivo();
        NoSolucao* nova = new NoSolucao();
        nova->proxima=ultimaSolucao;
        nova->solucao=solucao;
        ultimaSolucao=nova;

        //imprimeSolucao(solucao);
        //buscaLocal(solucao);
        int custo = calculaCustoSolucao(solucao);
        if(custo<minimo){
            minimo=custo;
            melhorSolucao=solucao;
        }
    }*/
    salvarSolucao(melhorSolucao);
    int custoAtual = calculaCustoSolucao(melhorSolucao);
    int numMaxSemMelhoras=7;
    int contaInteracoesSemMelhora=0;
    int controle=0;
        while(controle!=3&&contaInteracoesSemMelhora<numMaxSemMelhoras){
            if(controle==0) buscaLocal2(melhorSolucao);
            if(controle==1) buscaLocal(melhorSolucao);
            int custo = calculaCustoSolucao(melhorSolucao);
            if (custo< custoAtual){
                custoAtual=custo;
                controle=rand()%2;
                contaInteracoesSemMelhora=0;
            }else{
                contaInteracoesSemMelhora++;
                controle=rand()%2;
            }
        }
    salvarSolucao(melhorSolucao);

    NoSolucao* aux;
    if(ultimaSolucao!=NULL){
        aux=ultimaSolucao->proxima;
        desalocaSolucao(ultimaSolucao->solucao);
        delete ultimaSolucao;
        while(aux!=NULL){
            ultimaSolucao = aux;
            aux = aux->proxima;
            delete ultimaSolucao;
        }
        ultimaSolucao=NULL;
    }
    desalocaMemoria(NULL);
    fclose(arquivoSaida);
    return 0;
}

