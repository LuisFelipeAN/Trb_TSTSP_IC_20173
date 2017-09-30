#ifndef PROGRAMA_H_INCLUDED
#define PROGRAMA_H_INCLUDED
#include <fstream>
#include <string.h>
#include <stdio.h>
using namespace std;


void inicializa(FILE* arquivo);

void construtivo();

void imprimeSolucao();

void imprimeTabus();

void imprimeVerificacaoViabilidade();

int calculaCustoSolucao();

void desalocaMemoria();

#endif // PROGRAMA_H_INCLUDED
