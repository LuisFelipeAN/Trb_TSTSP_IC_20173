#ifndef PROGRAMA_H_INCLUDED
#define PROGRAMA_H_INCLUDED
#include <fstream>
#include <string.h>
#include <stdio.h>
using namespace std;
/*
template <class Type>
class No {
public:
    Type exp;
    No<Type>* prox;
    No<Type>(){prox = NULL;}
};*/
void inicializa(FILE* arquivo);
void desalocaMemoria();
#endif // PROGRAMA_H_INCLUDED
