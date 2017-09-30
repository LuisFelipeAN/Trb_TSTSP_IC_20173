#include "Vertice.h"

Vertice::Vertice(){
    id=-1;
}
Vertice::Vertice(int indice, double cordX, double cordY,int indiceCluster,int indiceTabu)
{
    id=indice;
    x=cordX;
    y=cordY;
    idTabu=indiceTabu;
    idCluster = indiceCluster;
    prox=NULL;
}

Vertice::~Vertice()
{
     prox=NULL;
}
