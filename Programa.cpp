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

///calcula o custo da solucao
int calculaCustoSolucao(){

    double somaCusto=0;
    No * aux ;
    int i=0;
    if(solucao!=NULL){
        for(aux = solucao; aux->proximo!=NULL;aux=aux->proximo){
                somaCusto+=aux->vertice->calculaCusto(aux->proximo->vertice);
                i++;
        }
        ///calcula a distancia do penultimo ao ultimo
        somaCusto+=aux->vertice->calculaCusto(aux->vertice);

        ///calcula a distancia do primeiro ao ultimo
        somaCusto+=solucao->vertice->calculaCusto(aux->vertice);
    }

    return floor(somaCusto);
}

void imprimeSolucao(){
     fprintf(stdout,"Rota -> ");
     for(No* aux = solucao; aux!=NULL;aux=aux->proximo){
            fprintf(stdout,"%d ", aux->vertice->getIDVertice());
    }
    fprintf(stdout,"\n");
}
///Funcao calcula aondo o novo Vertice sera inserido na lista de solucao
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
        ///verifica se o cluster do vertice a ser inserido é igual a pelo menos um dos extremos (vertices de um mesmo cluster tem que estar consecutivos)
        if(noAtual->vertice->getIndiceCluster()==v->getIndiceCluster()||noAtual->proximo->vertice->getIndiceCluster()==v->getIndiceCluster()){
            custo=0;
            custo+=noAtual->vertice->calculaCusto(v);
            custo+=v->calculaCusto(noAtual->proximo->vertice);
            custo-=noAtual->vertice->calculaCusto(noAtual->proximo->vertice);

            if(custo<custoMinimo){
                custoMinimo=custo;
                anterior=noAtual;
            }
            controleInsecaoIntraClusters=true;
        }
     }
    ///verifica se o cluster do vertice a ser inserido é igual a pelo menos um dos extremos (vertices de um mesmo cluster tem que estar consecutivos)
    if(noAtual->vertice->getIndiceCluster()==v->getIndiceCluster()){
        custo=0;
        custo+=solucao->vertice->calculaCusto(v);
        custo+=v->calculaCusto(noAtual->vertice);
        custo-=noAtual->vertice->calculaCusto(solucao->vertice);
        if(custo<custoMinimo){
            anterior=noAtual;
        }
        controleInsecaoIntraClusters=true;
    }
    ///caso se tenha encontrado pelo menos um vertice de mesmo cluster insere na posicao calculada
    if(controleInsecaoIntraClusters){
        novo->proximo = anterior->proximo;
        anterior->proximo=novo;
    }else{ /// senao insira o vertice no final da solucao
        noAtual->proximo= novo;
     }
}
void construtivo(){
    int numTabusVisitados = 0;

    ///inicia a solucao com vertices aleatorios de 3 tabus aleatorios
    solucao = new No();
    int indiceTabu = rand() % numTotalTabus;
    solucao->vertice = tabus[indiceTabu].getRandom();
    solucao->proximo = NULL;
    solucao->tabu = &tabus[indiceTabu];
    tabus[solucao->vertice->getIndiceTabu()].visitado=true;

    No * aux;
    aux = new No();
    indiceTabu = rand() % numTotalTabus;
    while(tabus[indiceTabu].visitado){///Garante que o tabu solucionado nao foi visitado
        indiceTabu = rand() % numTotalTabus;
    }
    aux->vertice= tabus[indiceTabu].getRandom();
    aux->proximo= NULL;
    aux->tabu = &tabus[indiceTabu];
    tabus[aux->vertice->getIndiceTabu()].visitado=true;

    No * aux2;
    aux2 = new No();
    indiceTabu = rand() % numTotalTabus;
    while(tabus[indiceTabu].visitado){///Garante que o tabu solucionado nao foi visitado
        indiceTabu = rand() % numTotalTabus;
    }
    aux2->vertice=tabus[indiceTabu].getRandom();
    int controle = 0;

    ///Garante que nos tres primeiros vertices adicionados seus clusters sejam todos diferentes ou iguais consecutivos
    while(aux2->vertice->getIndiceCluster()==solucao->vertice->getIndiceCluster()&&aux2->vertice->getIndiceCluster()!=aux->vertice->getIndiceCluster()){
         aux2->vertice=tabus[indiceTabu].getRandom();
         controle++;
         if(controle>3){///se tentar tres vezes conseguir um vertice no tabu atual outro nao visitado eh sorteado
                controle=0;
                indiceTabu = rand() % numTotalTabus;
                while(tabus[indiceTabu].visitado){///Garante que o tabu solucionado nao foi visitado
                    indiceTabu = rand() % numTotalTabus;
            }
         }
    }
    aux2->proximo= NULL;
    aux2->tabu = &tabus[indiceTabu];
    tabus[aux2->vertice->getIndiceTabu()].visitado=true;

    ///Encadeia os vertices encontrados na solucao
    aux->proximo=aux2;
    solucao->proximo=aux;

    numTabusVisitados=3;
    ///Perrcorre a lista de vertices inserindo na solução
    while(numTabusVisitados<numTotalTabus){
        for(ponteiro = primeiro; ponteiro != NULL ;ponteiro=ponteiro->getProximo()){
            if(!tabus[ponteiro->getIndiceTabu()].visitado){ ///verifica se o tabu do vertice atual nao foi visitado
                tabus[ponteiro->getIndiceTabu()].visitado=true; /// marca o tabu como visitado para garantir as restricoes do problema
                numTabusVisitados++;
                calculaInsercao(ponteiro);
            }
        }
    }
}
///percorre o vetor de tabus imprimindo os seus respectivos vertices
void imprimeTabus(){
    fprintf(stdout,"Tabus e seus Vertices:\n");
    for(int i=0;i<numTotalTabus;i++){
      fprintf(stdout,"T:%d ->",i+1);
      tabus[i].imprimeVertices();
      fprintf(stdout,"\n");
    }
}
 ///imprime os ids dos vertices seus tabus e clusters
void imprimeVerificacaoViabilidade(){
    fprintf(stdout,"Solucao com tabus e clusters:\n");
    for(No * aux = solucao; aux!=NULL;aux=aux->proximo){
        fprintf(stdout,"Vertice: %d \tTabu: %d\t Cluster:%d\n",aux->vertice->getIDVertice(),aux->vertice->getIndiceTabu()+1,aux->vertice->getIndiceCluster());
    }
}
void inicializa(FILE* arquivo){
    ///Efetua a leitura do cabeçalho da instancia
    char palavra[30];
    fscanf(arquivo,"%d %d %d",&numTotalVertices,&numTotalClusters,&numTotalTabus);
    ///Aloca o vetor de Tabus
    tabus= new Tabu[numTotalTabus];

    double x,y;
    int id,cont=0,idT,idC;

    ///cria o primeiro vertice
    fscanf(arquivo, "%d\t%lf\t%lf\t%d\t%d\n",&id,&x,&y,&idC,&idT);
    ///Converte o tabus para a forma de represetnacao
    idT=idT-1;
    Vertice* inicial = new Vertice(id,x,y,idC,idT);

    tabus[idT].insereVertice(inicial);
    ponteiro = inicial;
    primeiro = inicial;

    ///encadeia os demais vertices
    while(cont<numTotalVertices-1){
        fscanf(arquivo, "%d\t%lf\t%lf\t%d\t%d\n",&id,&x,&y,&idC,&idT);
        idT=idT-1;
        Vertice* aux = new Vertice(id,x,y,idC,idT);
        tabus[idT].insereVertice(aux);
        ponteiro->setProximo(aux);
        ponteiro = aux;
        cont++;
    }
    fclose(arquivo);
    solucao = NULL;

};
void buscaLocal(){
    Vertice* candidato;
     Vertice* bkp;
    double custoAtual = calculaCustoSolucao();
    int novoCusto;
    double distanciaAnterior;
    double distanciaNovoVertice;
    No * ultimo=solucao;
    No * anterior;
    while(ultimo->proximo!=NULL){///encontra o ultimo No da solucao;
        ultimo=ultimo->proximo;
    }
    No* aux= solucao;
    anterior=ultimo;
   while(aux->proximo!=NULL){
        if(aux==solucao){
            candidato=aux->tabu->outroVertice(aux->vertice);
            if(candidato->getIndiceCluster()==aux->vertice->getIndiceCluster()|candidato->getIndiceCluster()==aux->proximo->vertice->getIndiceCluster()|candidato->getIndiceCluster()==aux->proximo->vertice->getIndiceCluster()){
                bkp=aux->vertice;
                aux->vertice=candidato;
                novoCusto=calculaCustoSolucao();
                if(novoCusto<custoAtual){
                        fprintf(stdout,"Trocou 1\n");
                        custoAtual=novoCusto;
                        aux=solucao;
                }else{
                    aux->vertice=bkp;
                    aux=aux->proximo;
                 }
            }else{
                aux=aux->proximo;
             }
        }else if(aux==ultimo){

                candidato=aux->tabu->outroVertice(aux->vertice);
                if(candidato->getIndiceCluster()==aux->vertice->getIndiceCluster()|candidato->getIndiceCluster()==aux->proximo->vertice->getIndiceCluster()|candidato->getIndiceCluster()==aux->proximo->vertice->getIndiceCluster()){
                    bkp=aux->vertice;
                    aux->vertice=candidato;
                    novoCusto=calculaCustoSolucao();
                    if(novoCusto<custoAtual){
                        fprintf(stdout,"Trocou 2\n");
                        custoAtual=novoCusto;
                        aux=solucao;
                     }else{
                        aux->vertice=bkp;
                        aux=aux->proximo;
                      }
                }else{
                    aux=aux->proximo;
                 }
               }else{
                    candidato=aux->tabu->outroVertice(aux->vertice);
                    if(candidato->getIndiceCluster()==aux->vertice->getIndiceCluster()|candidato->getIndiceCluster()==aux->proximo->vertice->getIndiceCluster()|candidato->getIndiceCluster()==aux->proximo->vertice->getIndiceCluster()){
                        bkp=aux->vertice;
                        aux->vertice=candidato;
                        novoCusto=calculaCustoSolucao();
                        if(novoCusto<custoAtual){
                            fprintf(stdout,"Trocou 3\n");
                            custoAtual=novoCusto;
                            aux=solucao;

                        }else{
                            aux->vertice=bkp;
                            aux=aux->proximo;
                         }
                    }else{
                        aux=aux->proximo;
                     }
                }
             anterior=aux;
    }
}
void desalocaMemoria(){
    ///Desaloca a lista encadeada de vertices
    ponteiro=primeiro;
    while(ponteiro!=NULL){
        ponteiro=primeiro->getProximo();
        primeiro->~Vertice();
        primeiro=ponteiro;
    }

    ///Desaloca o vetor de tabus
    delete ponteiro;
    for(int i=0;i<numTotalTabus;i++){
        tabus[i].~Tabu();
    }

    ///Desaloca os elementos da solucao
    No* aux;
    if(solucao!=NULL){
        aux=solucao->proximo;
        delete solucao;
        while(aux!=NULL){
            solucao = aux;
            aux = aux->proximo;
            delete solucao;
        }
        solucao=NULL;
    }
}
