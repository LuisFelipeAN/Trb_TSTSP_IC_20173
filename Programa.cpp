#include "Programa.h"
#include "Vertice.h"


///vertor de tabus do problema
static Tabu* tabus;

///variaveis para a quantidade de tabus,vertices e clusters do problema
static int numTotalTabus;
static int numTotalClusters;
static int numTotalVertices;

///Ponteiro para a lista encadeada de vertices
static Vertice *primeiro;
static Vertice *ponteiro;

static FILE* arqSaida;

///calcula o custo da solucao
int calculaCustoSolucao(No* solucao){

    double somaCusto=0;
    No * aux ;
    if(solucao!=NULL){
        for(aux = solucao; aux->proximo!=NULL;aux=aux->proximo){
                somaCusto+=aux->vertice->calculaCusto(aux->proximo->vertice);
        }
        ///calcula a distancia do penultimo ao ultimo
        somaCusto+=aux->vertice->calculaCusto(aux->vertice);

        ///calcula a distancia do primeiro ao ultimo
        somaCusto+=solucao->vertice->calculaCusto(aux->vertice);
    }

    return floor(somaCusto);
}

void imprimeSolucao(No* solucao){
     fprintf(arqSaida,"Rota -> ");
     for(No* aux = solucao; aux!=NULL;aux=aux->proximo){
            fprintf(arqSaida,"%d ", aux->vertice->getIDVertice());
    }
    fprintf(arqSaida,"\n");
}
///Funcao calcula aondo o novo Vertice sera inserido na lista de solucao
void calculaInsercao(No* solucao,Vertice *v){
     No *anterior;
     No *novo = new No();

     novo->vertice=v;
     novo->proximo=NULL;
     novo->tabu=&tabus[v->getIndiceTabu()];

     double custoMinimo = 9999999;
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
No* construtivo(){
    int numTabusVisitados = 0;
    No *solucao;
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
                calculaInsercao(solucao,ponteiro);
            }
        }
    }
    for(int i=0;i<numTotalTabus;i++){
        tabus[i].visitado=false;
    }
    return solucao;
}
///percorre o vetor de tabus imprimindo os seus respectivos vertices
void imprimeTabus(No* solucao){
    fprintf(arqSaida,"Tabus e seus Vertices:\n");
    for(int i=0;i<numTotalTabus;i++){
      fprintf(arqSaida,"T:%d ->",i+1);
      tabus[i].imprimeVertices(arqSaida);
      fprintf(arqSaida,"\n");
    }
}
 ///imprime os ids dos vertices seus tabus e clusters
void imprimeVerificacaoViabilidade(No* solucao){
    fprintf(arqSaida,"Solucao com tabus e clusters:\n");
    for(No * aux = solucao; aux!=NULL;aux=aux->proximo){
        fprintf(arqSaida,"Vertice: %d \tTabu: %d\t Cluster:%d\n",aux->vertice->getIDVertice(),aux->vertice->getIndiceTabu()+1,aux->vertice->getIndiceCluster());
    }
}
///efetua a leitura do fluxo passado como parametro
void inicializa(FILE* arquivoEntrada, FILE* arquivoSaida){
    arqSaida=arquivoSaida;

    fscanf(arquivoEntrada,"%d %d %d",&numTotalVertices,&numTotalClusters,&numTotalTabus);
    ///Aloca o vetor de Tabus
    tabus= new Tabu[numTotalTabus];

    double x,y;
    int id,cont=0,idT,idC;

    ///cria o primeiro vertice
    fscanf(arquivoEntrada, "%d\t%lf\t%lf\t%d\t%d\n",&id,&x,&y,&idC,&idT);
    ///Converte o tabus para a forma de represetnacao
    idT=idT-1;
    Vertice* inicial = new Vertice(id,x,y,idC,idT);

    tabus[idT].insereVertice(inicial);
    ponteiro = inicial;
    primeiro = inicial;

    ///encadeia os demais vertices
    while(cont<numTotalVertices-1){
        fscanf(arquivoEntrada, "%d\t%lf\t%lf\t%d\t%d\n",&id,&x,&y,&idC,&idT);
        idT=idT-1;
        Vertice* aux = new Vertice(id,x,y,idC,idT);
        tabus[idT].insereVertice(aux);
        ponteiro->setProximo(aux);
        ponteiro = aux;
        cont++;
    }
    fclose(arquivoEntrada);

};
void trocaClusters(Cluster *c1,Cluster *c2){
   // fprintf(stdout,"Trocar: ca: %d por cp: %d\n",c1->fim->vertice->getIndiceCluster(),c2->fim->vertice->getIndiceCluster());
   if(c1->fim!=c2->inicio&&c1->inicio!=c2->fim){///verifica se os clusters nao sao adjacentes
        ///ponteiros que delimitam os clusters
        No *i1= c1->inicio;
        No *f1= c1->fim;
        No *i2= c2->inicio;
        No *f2= c2->fim;

        ///troca os dois clustes
        No* aux2 = i1->proximo;
        i1->proximo=i2->proximo;
        i2->proximo=aux2;

        ///troca a saida dos dois clusters
        No* aux=f1->proximo;
        f1->proximo=f2->proximo;
        f2->proximo=aux;

        ///atualiza o inicio o e final dos clusters
        /*c2->inicio=i1;
        c1->inicio=i2;

        c2->fim=f1;
        c1->fim=f2;*/
        ///troca a posicao dos dois cluster na lista
        Cluster *caux =c1;
        c1=c2;
        c2=caux;

    }else{
        if(c1->fim==c2->inicio){
            ///ponteiros que delimitam os clusters
            No *i1= c1->inicio;
            No *f1= c1->fim;
            No *i2= c2->inicio;
            No *f2= c2->fim;

            ///troca os dois clustes
            No* aux2 = i1->proximo;
            i1->proximo=i2->proximo;
            i2->proximo=aux2;

            ///troca a saida dos dois clusters
            f1->proximo=f2->proximo;
            f2->proximo=aux2;

            ///atualiza o inicio o e final dos clusters
            c2->inicio=i1;
            c1->inicio=f2;

            ///troca a posicao dos dois cluster na lista
            Cluster *caux =c1;
            c1=c2;
            c2=caux;

            //fprintf(stdout,"c1: ca: %d por cp: %d\n",c1->inicio->vertice->getIDVertice(),c1->fim->vertice->getIDVertice());
           // fprintf(stdout,"c2: ca: %d por cp: %d\n",c2->inicio->vertice->getIDVertice(),c2->fim->vertice->getIDVertice());
        }else{///inverte a ordem dos clusters
            trocaClusters(c2,c1);
         }
     }
}
void buscaLocal2(No* solucao){

    Cluster* inicial = NULL;
    int controle=1;
    int idClusterAtual=-1;




    No* ultimo=solucao;
    while(ultimo->proximo!=NULL){///encontra o ultimo No da solucao;
        ultimo=ultimo->proximo;
    }

    No* aux = solucao;
    No* anterior=solucao;
    No* saidaUltimoCluster=solucao;
    while(aux!=NULL){
        if(aux->vertice->getIndiceCluster()!=idClusterAtual){
            if(controle==1){
                controle=0;
                Cluster* no= new Cluster();
                no->inicio=saidaUltimoCluster;
                no->proximo=inicial;
                inicial=no;
                idClusterAtual=aux->vertice->getIndiceCluster();
                anterior=aux;
                aux=aux->proximo;

            }else{
                inicial->fim=anterior;
                saidaUltimoCluster=anterior;
                controle=1;

            }

        }else{
            anterior=aux;
            aux=aux->proximo;
        }

    }
    inicial->fim=ultimo;

    Cluster *c;
    for(c=inicial;c->proximo!=NULL;c=c->proximo);
    c->inicio=inicial->inicio;

    /*for(Cluster* c = inicial;c!=NULL;c=c->proximo){
        fprintf(stdout,"c: %d\n vi:%d\tvf:%d\n",c->fim->vertice->getIndiceCluster(),c->inicio->vertice->getIDVertice(),c->fim->vertice->getIDVertice());
    }*/

    int custoAtual = calculaCustoSolucao(solucao);
    Cluster *c1=inicial;
    while(c1!=NULL&&c1->proximo!=NULL){
            Cluster *c2=c1->proximo;
            trocaClusters(c1,c2);
            int custo = calculaCustoSolucao(solucao);
            if (custo< custoAtual){
                custoAtual=custo;
                c1=inicial;
            }else{
                trocaClusters(c2,c1);
                c1=c1->proximo->proximo;
            }
    }
}
void buscaLocal(No* solucao){
    Vertice* candidato; ///vertice candidado a ser o substitudo na solução
    Vertice* bkp; ///armazena o vertice que foi alterado na solução
    int custoAtual = calculaCustoSolucao(solucao);///armazena o custo atual da solução
    int novoCusto;///armazena o custo da nova soluçã
    No * ultimo=solucao;
    No * anterior;
    while(ultimo->proximo!=NULL){///encontra o ultimo No da solucao;
        ultimo=ultimo->proximo;
    }
    No* aux= solucao;
    anterior=ultimo;/// o anterior do primeiro do da solucao é o ultimo; usada para garantir que a substituição é válida
    while(aux!=NULL){
        if(aux==solucao){///tenta alterar o primeiro elemento da solução
            candidato=aux->tabu->outroVertice(aux->vertice);
            ///Garante a viabilidade da alteração
            if(candidato->getIndiceCluster()==aux->vertice->getIndiceCluster()|candidato->getIndiceCluster()==aux->proximo->vertice->getIndiceCluster()|candidato->getIndiceCluster()==anterior->vertice->getIndiceCluster()){
                bkp=aux->vertice;
                aux->vertice=candidato;
                novoCusto=calculaCustoSolucao(solucao);
                if(novoCusto<custoAtual){///verifica se ocorreu uma melhora na solução
                        custoAtual=novoCusto;
                        aux=solucao;///Reinicia a busca local com a nova solução
                }else{///Se não ocorreu melhora desfaz a alteração na solução
                    aux->vertice=bkp;
                    aux=aux->proximo;
                 }
            }else{
                aux=aux->proximo;
             }
        }else if(aux==ultimo){///Tenta alterar o ultimo elemento da solução
                candidato=aux->tabu->outroVertice(aux->vertice);
                ///Garante a viabilidade da alteração
                if(candidato->getIndiceCluster()==aux->vertice->getIndiceCluster()|candidato->getIndiceCluster()==solucao->vertice->getIndiceCluster()|candidato->getIndiceCluster()==anterior->vertice->getIndiceCluster()){
                    bkp=aux->vertice;
                    aux->vertice=candidato;
                    novoCusto=calculaCustoSolucao(solucao);
                    if(novoCusto<custoAtual){///verifica se ocorreu uma melhora na solucao
                        custoAtual=novoCusto;
                        aux=solucao;///reinicia a busca com a nova solucao
                     }else{///Se não ocorreu melhora desfaz a alteração na solução
                        aux->vertice=bkp;
                        aux=aux->proximo;
                      }
                }else{
                    aux=aux->proximo;
                 }
               }else{///Tenta alterar um elemento do mei da solucao
                    candidato=aux->tabu->outroVertice(aux->vertice);
                    ///Garante a viabilidade da alteração
                    if(candidato->getIndiceCluster()==aux->vertice->getIndiceCluster()|candidato->getIndiceCluster()==aux->proximo->vertice->getIndiceCluster()|candidato->getIndiceCluster()==anterior->vertice->getIndiceCluster()){
                        bkp=aux->vertice;
                        aux->vertice=candidato;
                        novoCusto=calculaCustoSolucao(solucao);
                        if(novoCusto<custoAtual){///verifica se ocorreu uma melhora na solucao
                            custoAtual=novoCusto;
                            aux=solucao;///reinicia a busca com a nova solucao

                        }else{///Se não ocorreu melhora desfaz a alteração na solução
                            aux->vertice=bkp;
                            aux=aux->proximo;
                         }
                    }else{///vai para o proximo elemento da solução
                        aux=aux->proximo;
                     }
                }
             anterior=aux;///atualiza o elemento anterior
    }
}
void salvarSolucao(No* solucao){
    Vertice *v;
    fprintf(arqSaida,"#Resultado\n");
    fprintf(arqSaida,"#Custo: %d\n",calculaCustoSolucao(solucao));
    fprintf(arqSaida,"#Rota: (idV|X|Y|idC|idT)\n");
    for(No *aux=solucao;aux!=NULL;aux=aux->proximo){
        v=aux->vertice;
        fprintf(arqSaida,"%d\t%lf\t%lf\t%d\t%d\n",v->getIDVertice(),v->getCordX(),v->getCordY(),v->getIndiceCluster(),v->getIndiceTabu());
    }
    v=solucao->vertice;
    fprintf(arqSaida,"%d\t%lf\t%lf\t%d\t%d\n",v->getIDVertice(),v->getCordX(),v->getCordY(),v->getIndiceCluster(),v->getIndiceTabu());
}
void desalocaSolucao(No* solucao){
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
void desalocaMemoria(No* solucao){
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
