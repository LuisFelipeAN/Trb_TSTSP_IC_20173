#ifndef PROGRAMA_H_INCLUDED
#define PROGRAMA_H_INCLUDED
#include <fstream>
#include <string.h>
#include <stdio.h>
#include "Tabu.h"
using namespace std;

///estrutura para encadear os vertices presentes na solucao
typedef struct No{
    No *proximo;
    Tabu* tabu;
    Vertice* vertice;
} No;

///estrutura para encadear os clusters presentes na solucao
typedef struct Cluster{
    No*inicio;
    No*fim;
    Cluster* proximo;
    Cluster* anterior;
}Cluster;

void inicializa(FILE* arquivoEntrada,FILE* arquivoSaida);

No* construtivo();

void imprimeSolucao(No* solucao);

void imprimeTabus(No* solucao);

void imprimeVerificacaoViabilidade(No* solucao);
void buscaLocal3(No* solucao);
void buscaLocal2(No* solucao);
void buscaLocal(No* solucao);
int calculaCustoSolucao(No* solucao);

void desalocaMemoria(No* solucao);

void desalocaSolucao(No* solucao);

void salvarSolucao(No* solucao);
#endif // PROGRAMA_H_INCLUDED
