#include "Tabu.h"
using namespace std;
Tabu::Tabu()
{
    primeiroNo=NULL;
    ultimoNo  = NULL;
    visitado=false;
    numVertices=0;
}
///insere os Nos ordenados na lista encadeada
void Tabu::insereVertice(Vertice* v){
    NoTabu *novo = new NoTabu();
    novo->vertice=v;
    novo->proximo=NULL;
    numVertices++;
    if(primeiroNo==NULL){
        primeiroNo=novo;
        ultimoNo=novo;
    }
    else{
        ultimoNo->proximo=novo;
        ultimoNo = novo;
    }
}
///retorna o primeiro vertice diferente do vertice passado
Vertice* Tabu::outroVertice(Vertice *v){
    Vertice* retorno=NULL;
    NoTabu* aux;
    NoTabu* percorre;
    if(primeiroNo->vertice==v&&primeiroNo->proximo!=NULL){///retorna o segundo vertice da lista encadeada

        retorno=primeiroNo->proximo->vertice;
        aux=primeiroNo;

        ultimoNo->proximo=primeiroNo;

        primeiroNo=aux->proximo;
        aux->proximo=NULL;

    }else{
        percorre = primeiroNo;
        while(percorre->proximo!=NULL&&percorre->proximo->vertice!=v){
            percorre=percorre->proximo;
        }
        if(percorre->proximo!=NULL&&percorre->proximo->vertice==v){

            aux=percorre->proximo;

            percorre->proximo=aux->proximo;
            aux->proximo=NULL;

            ultimoNo->proximo=aux;
            ultimoNo=aux;

            retorno=primeiroNo->vertice;
        }
     }
    return retorno;
}

void Tabu::imprimeVertices(){
    for(NoTabu *aux = primeiroNo;aux!=NULL;aux=aux->proximo){
            fprintf(stdout," %d",aux->vertice->getIDVertice());
    }

}
///retorna um vertice randomico do tabu
Vertice* Tabu::getRandom(){
    int vRand = rand() % numVertices;
    NoTabu* aux=primeiroNo;
    for(int i=0;i<vRand;i++){
        aux=aux->proximo;
    }
    return aux->vertice;
}
Tabu::~Tabu()
{
    NoTabu* aux;
    aux=primeiroNo->proximo;
    delete primeiroNo;

    while(aux!=NULL){
        primeiroNo = aux;
        aux = aux->proximo;
        delete primeiroNo;
    }
    primeiroNo=NULL;
    ultimoNo =NULL;
}
