#ifndef PROGRAMA_H_INCLUDED
#define PROGRAMA_H_INCLUDED
#include <fstream>
#include <string.h>
#include <stdio.h>
#include "Tabu.h"
using namespace std;

///estrutura para encadear os vertices presentes na solucao
typedef struct No{
    No *anterior;
    No *proximo;
    Tabu* tabu;
    Vertice* vertice;
} No;
double getPenalizacao();

void inicializa(FILE* arquivoEntrada,FILE* arquivoSaida);

No* construtivo();

void imprimeSolucao(No* solucao);

void imprimeTabus(No* solucao);

void imprimeVerificacaoViabilidade(No* solucao);




int calculaCustoSolucao(No* solucao);

void desalocaMemoria(No* solucao);

void desalocaSolucao(No* solucao);

void salvarSolucao(No* solucao);
#endif // PROGRAMA_H_INCLUDED
