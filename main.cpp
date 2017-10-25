#include <iostream>
#include "Programa.h"
using namespace std;

///estrutura para encadear as solucoes geradas
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
    ///inicia as variaveis do programa
    inicializa(arquivoEntrada,arquivoSaida);

    ///inicia uma lista encadeada de solucoes
    NoSolucao *ultimaSolucao;
    ultimaSolucao = new NoSolucao();
    ultimaSolucao->proxima = NULL;
    ultimaSolucao->solucao = NULL;

    int minimo=99999999;///melhor de todas as solucoes
    No* melhorSolucao;
    for(int i=0;i<1;i++){
        No* solucao = construtivo();///obtem uma solucao inicial

        ///efetua uma busca VNS na solucao atual
        int custoAtual = calculaCustoSolucao(solucao);
        int numMaxSemMelhoras=10;
        int controle=1;
            while(controle!=4){
                if(controle==0) buscaLocal(solucao);
                if(controle==1) buscaLocal2(solucao);
                if(controle==2) buscaLocal3(solucao);
                int custo = calculaCustoSolucao(solucao);
                if (custo< custoAtual){
                    custoAtual=custo;
                    controle=1;
                }else{
                    if(controle==0) controle=2;
                    if(controle==1) controle=0;
                    if(controle==2) controle=4;
                }
            }

        ///adiciona a solucao atual na lista
        NoSolucao* nova = new NoSolucao();
        nova->proxima=ultimaSolucao;
        nova->solucao=solucao;
        ultimaSolucao=nova;

        ///verifica se a solucao atual e melhor do que a melhor solucao encontrada
        int custo = calculaCustoSolucao(solucao);
        if(custo<minimo){///se for melhor atualiza a melhor solucao
            minimo=custo;
            melhorSolucao=solucao;
        }
    }
    salvarSolucao(melhorSolucao);///salva a melhor de todas as solucoes

    ///desaloca a lista de solucoes
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
    ///desaloca os vertice es os clusters
    desalocaMemoria(NULL);

    ///fecha o arquivo de saida
    fclose(arquivoSaida);
    return 0;
}

