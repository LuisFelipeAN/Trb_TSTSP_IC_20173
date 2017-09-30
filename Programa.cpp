#include "Programa.h"
#include "Vertice.h"
#include "Tabu.h"

struct No{
    No *proximo;
    Tabu* tabu;
    Vertice* vertice;
};

//verto de tabus do problema
static Tabu* tabus;

//variaveis para a quantidade de tabus,vertices e clusters do problema
static int numTotalTabus;
static int numTotalClusters;
static int numTotalVertices;

//Ponteiro para a estrutura de ar
static Vertice *primeiro;
static Vertice *ponteiro;

//ponteiro para a lista encadeada de vertices da solucao
static No* solucao;

//ponteiros para calcular o inicio e o fim da interacao em um cluster
static No* inicio,fim;


static double custoSolucaAtual;

Vertice *procuraVertice(int indiceTabu){

    for( ponteiro = primeiro; ponteiro != NULL ;ponteiro=ponteiro->getProximo()){
        if(ponteiro->getIndiceTabu()==indiceTabu) return ponteiro;
    }
    return ponteiro;
}

int calculaCustoSolucao(){

    double somaCusto=0;
    No * aux ;
    int i=0;
    for(aux = solucao; aux->proximo!=NULL;aux=aux->proximo){
            //fprintf(stdout,"Custo %lf: %d\n", aux->vertice->calculaCusto(aux->proximo->vertice),i);
            somaCusto+=aux->vertice->calculaCusto(aux->proximo->vertice);
            i++;
    }
    //calcula a distancia do penultimo ao ultimo
    somaCusto+=aux->vertice->calculaCusto(aux->vertice);

    //calcula a distancia do primeiro ao ultimo
    somaCusto+=solucao->vertice->calculaCusto(aux->vertice);

    return floor(somaCusto);
}

void imprimeSolucao(){
     for(No* aux = solucao; aux->proximo!=NULL;aux=aux->proximo){
            fprintf(stdout,"%d ", aux->vertice->getIDVertice());
    }
    fprintf(stdout,"\n ");
}
void calculaInsercao(Vertice *v){
     No *anterior;
     No *novo = new No();

     novo->vertice=v;
     novo->proximo=NULL;
     novo->tabu=&tabus[v->getIndiceTabu()];

     double custoMinimo = 9999999.999;
     bool controleInsecaoIntraClusters=false;
     double custo=0;
     No *noAtual;
     for(noAtual=solucao;noAtual->proximo!=NULL;noAtual=noAtual->proximo){
        //verifica se o cluster do vertice a ser inserido é igual a pelo menos um dos extremos (vertices de um mesmo cluster tem que estar consecutivos)
        if(noAtual->vertice->getIndiceCluster()==v->getIndiceCluster()|noAtual->proximo->vertice->getIndiceCluster()==v->getIndiceCluster()){
            custo=0;
            custo+=noAtual->vertice->calculaCusto(v);
            custo+=v->calculaCusto(noAtual->proximo->vertice);
            custo-=noAtual->vertice->calculaCusto(noAtual->proximo->vertice);
            fprintf(stdout,"Teste: idA:%d id:%d idP:%d\n Custo: %lf\n",noAtual->vertice->getIDVertice(),v->getIDVertice(),noAtual->proximo->vertice->getIDVertice(),custo);
            if(custo<custoMinimo){
                custoMinimo=custo;
                anterior=noAtual;
                fprintf(stdout,"Minimo %d: %lf\n Entre: idA:%d idP:%d\n",v->getIDVertice(),custoMinimo,anterior->vertice->getIDVertice(),anterior->proximo->vertice->getIDVertice());

            }
            controleInsecaoIntraClusters=true;
        }
     }
    //verifica se o cluster do vertice a ser inserido é igual a pelo menos um dos extremos (vertices de um mesmo cluster tem que estar consecutivos)
    if(noAtual->vertice->getIndiceCluster()==v->getIndiceCluster()){
        custo=0;
        custo+=solucao->vertice->calculaCusto(v);
        custo+=v->calculaCusto(noAtual->vertice);
        custo-=noAtual->vertice->calculaCusto(solucao->vertice);
        if(custo<custoMinimo){
            anterior=noAtual;
            fprintf(stdout,"Minimo %d: %lf\n Entre: idA:%d idP:%d\n",v->getIDVertice(),custoMinimo,anterior->vertice->getIDVertice(),anterior->vertice->getIDVertice());
        }
        controleInsecaoIntraClusters=true;
    }
    //caso se tenha encontrado pelo menos um vertice de mesmo cluster insere na posicao calculada
    if(controleInsecaoIntraClusters){
        novo->proximo = anterior->proximo;
        anterior->proximo=novo;
    }else{ // senao insira o vertice no final da solucao
        noAtual->proximo= novo;
     }
    imprimeSolucao();
}
void construtivo(){
    int numTabusVisitados = 0;

    int clusterAtual;

    solucao = new No();
    int indiceTabu = rand() % numTotalTabus;
    fprintf(stdout,"v1: %d \n",indiceTabu);
    solucao->vertice = procuraVertice(indiceTabu);
    solucao->proximo = NULL;
    solucao->tabu = &tabus[indiceTabu];
    tabus[solucao->vertice->getIndiceTabu()].visitado=true;

    No * aux;
    aux = new No();
    indiceTabu = rand() % numTotalTabus;
    fprintf(stdout,"v2: %d \n",indiceTabu);
    while(tabus[indiceTabu].visitado){
        indiceTabu = rand() % numTotalTabus;
        fprintf(stdout,"v2 colisao: %d \n",indiceTabu);
    }
    aux->vertice=procuraVertice(indiceTabu);
    aux->proximo= NULL;
    aux->tabu = &tabus[indiceTabu];
    tabus[ponteiro->getIndiceTabu()].visitado=true;

    No * aux2;
    aux2 = new No();
    indiceTabu = rand() % numTotalTabus;
    fprintf(stdout,"v3: %d \n",indiceTabu);
    while(tabus[indiceTabu].visitado){
        indiceTabu = rand() % numTotalTabus;
        fprintf(stdout,"v3 colisao: %d \n",indiceTabu);
    }
    aux2->vertice=procuraVertice(indiceTabu);
    aux2->proximo= NULL;
    aux2->tabu = &tabus[indiceTabu];
    tabus[ponteiro->getIndiceTabu()].visitado=true;

    aux->proximo=aux2;
    solucao->proximo=aux;

    /*int verticesFaltam = 2;
    for(ponteiro = primeiro; ponteiro != NULL ;ponteiro=ponteiro->getProximo()){
        if(ponteiro->getIndiceCluster()==clusterAtual&&!tabus[ponteiro->getIndiceTabu()].visitado) {
            aux = new No();
            aux->vertice=ponteiro;
            aux->proximo=solucao;
            tabus[ponteiro->getIndiceTabu()].visitado=true;
            solucao=aux;
            verticesFaltam--;
            if(verticesFaltam==0) break;
        }
    }*/
    numTabusVisitados=3;
    custoSolucaAtual=0;
    while(numTabusVisitados<numTotalTabus){
        for(ponteiro = primeiro; ponteiro != NULL ;ponteiro=ponteiro->getProximo()){
            if(!tabus[ponteiro->getIndiceTabu()].visitado){
                tabus[ponteiro->getIndiceTabu()].visitado=true;
                numTabusVisitados++;
                calculaInsercao(ponteiro);
            }
        }
    }
}
void criaSoculao(){

    No* novo = new No();

    novo->vertice=procuraVertice(0);
    novo->proximo = solucao;
    solucao = novo;

    for(int i=1;i<numTotalTabus;i++){
        novo = new No();
        novo->vertice=procuraVertice(i);
        novo->proximo = solucao;
        solucao = novo;
    }
}
void inicializa(FILE* arquivo){
    //Efetua a leitura do cabeçalho da instancia
    char palavra[30];
    fscanf(arquivo,"%s",palavra);
    while((string)palavra!="Coordenadas:"){
        if((string) palavra == "Dimensao:"){
            fscanf(arquivo,"%d",&numTotalVertices);
        }
          if((string) palavra == "Clusters:"){
            fscanf(arquivo,"%d",&numTotalClusters);
        }
        if((string) palavra == "Tabus:"){
            fscanf(arquivo,"%d",&numTotalTabus);
        }
        fscanf(arquivo,"%s",palavra);
      // fprintf(stdout,"%s\n",palavra);
    }
    //Aloca o vetor de Tabus
    tabus= new Tabu[numTotalTabus];

    double x,y;
    int id,cont=0,idT,idC;

    //cria o primeiro vertice
    fscanf(arquivo, "%d\t%lf\t%lf\t%d\t%d\n",&id,&x,&y,&idC,&idT);
    Vertice* inicial = new Vertice(id,x,y,idC,idT);

    tabus[idT].insereVertice(inicial);
    ponteiro = inicial;
    primeiro = inicial;

    //encadeia os demais vertices
    while(cont<numTotalVertices-1){
        fscanf(arquivo, "%d\t%lf\t%lf\t%d\t%d\n",&id,&x,&y,&idC,&idT);
        //fprintf(stdout,"%d\t%lf\t%lf\t%d\t%d\n",id,x,y,idT,idC);
        Vertice* aux = new Vertice(id,x,y,idC,idT);
        tabus[idT].insereVertice(aux);
        ponteiro->setProximo(aux);
        ponteiro = aux;
        cont++;
    }
    fclose(arquivo);

    solucao = NULL;
    construtivo();
    for(No * aux = solucao; aux!=NULL;aux=aux->proximo){
        fprintf(stdout,"id:%d idT:%d idC:%d\n",aux->vertice->getIDVertice(),aux->vertice->getIndiceTabu(),aux->vertice->getIndiceCluster());
    }
    for(No * aux = solucao; aux!=NULL;aux=aux->proximo){
        tabus[aux->vertice->getIndiceTabu()].outroVertice(aux->vertice);
    }
    fprintf(stdout,"Custo da Solucao %d\n",calculaCustoSolucao());
};

void desalocaMemoria(){

    ponteiro=primeiro;
    while(ponteiro!=NULL){
        ponteiro=primeiro->getProximo();
        primeiro->~Vertice();
        primeiro=ponteiro;
    }
    delete ponteiro;
    for(int i=0;i<numTotalTabus;i++){
        tabus[i].~Tabu();
    }

    No* aux;
    aux=solucao->proximo;
    delete solucao;

    while(aux!=NULL){
        solucao = aux;
        aux = aux->proximo;
        delete solucao;
    }
    solucao=NULL;
}
