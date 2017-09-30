#include "Tabu.h"

Tabu::Tabu()
{
    primeiroNo=NULL;
    ultimoNo  = NULL;
    visitado=false;
}
void Tabu::insereVertice(Vertice* v){
    NoTabu *novo = new NoTabu();
    novo->vertice=v;
    novo->proximo=NULL;
    if(primeiroNo==NULL){
        primeiroNo=novo;
        ultimoNo=novo;
    }
    else{
        ultimoNo->proximo=novo;
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
