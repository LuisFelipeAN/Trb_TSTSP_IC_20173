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
double Vertice::calculaCusto(Vertice *v){
    //if(idCluster==v->getIndiceCluster()){
       return  sqrt( pow(v->getCordX()-x,2)+pow(v->getCordY()-y,2));
    /*}
    else{
         return  100*sqrt( pow(v->getCordX()-x,2)+pow(v->getCordY()-y,2));
    }*/

};
Vertice::~Vertice()
{
     prox=NULL;
}
