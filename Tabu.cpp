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
Vertice* Tabu::outroVertice(Vertice *v){
    for(NoTabu *aux = primeiroNo;aux!=NULL;aux=aux->proximo){
        if(aux->vertice==v){
            fprintf(stdout,"OK-- v:%d  T:%d\n",v->getIDVertice(),v->getIndiceTabu());
        }
    }
    return NULL;
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
