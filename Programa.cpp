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

///Efetua uma busca local trocando vertices adjacentes que possuam um mesmo cluster
void buscaLocal3(No* solucao){

    No * ultimo=solucao;
    while(ultimo->proximo!=NULL){///encontra o ultimo No da solucao;
        ultimo=ultimo->proximo;
    }
    No* noAnt=solucao;
    No* noAtual=solucao->proximo;
    No* noProx=solucao->proximo->proximo;
    int custoAtual=calculaCustoSolucao(solucao);
    while(noProx!=NULL){
        if(noAtual->vertice->getIndiceCluster()==noProx->vertice->getIndiceCluster()){
            noAnt->proximo=noProx;
            noAtual->proximo=noProx->proximo;
            noProx->proximo=noAtual;
            int custo = calculaCustoSolucao(solucao);
            if(custo<custoAtual){
                custoAtual=custo;
                noAnt=ultimo;
                noAtual=solucao;

            }else{
                noProx->proximo=noAtual->proximo;
                noAtual->proximo=noAnt->proximo;
                noAnt->proximo=noAtual;

                noAnt=noAtual;
                noAtual=noProx->proximo;

                if(noProx->proximo!=NULL){
                    noProx=noProx->proximo->proximo;
                }else break;
            }
        }
        noAnt=noAtual;
        noAtual=noAtual->proximo;
        if(noProx!=NULL){
            noProx=noProx->proximo;
        } else break;
    }
}

///Funcao que efetua a troca de dois clustes adjacentes tanto na solucao quanto na lista duplamente encadeada atual
void trocaClusters(Cluster *cAtual){
    Cluster* cAnt=cAtual->anterior;
    Cluster* cProx=cAtual->proximo;


    cAnt->fim->proximo=cProx->inicio;

    ///o fim do atual aponta para o fim do proximo
    cAtual->fim->proximo=cProx->fim->proximo;

    ///o fim do proximo aponta para o inicio do atual
    cProx->fim->proximo=cAtual->inicio;

    ///liga o proximo do anterior no proximo
    cAnt->proximo=cProx;

    ///liga o anterior do proximo no proximo
    cProx->anterior=cAnt;

    ///liga o proximo do atual ao proximo do proximo
    cAtual->proximo=cProx->proximo;

    ///liga o proximo do atual ao atual
    cProx->proximo=cAtual;

    ///liga o anterior do atual no proximo
    cAtual->anterior=cProx;

}

///Efetua uma busca local trocando clusters adjacentes baseado em uma lista duplamente encadeada de clusters
void buscaLocal2(No* solucao){

    Cluster* clusterFinal = NULL;///ultimo cluster da solucao
    int controle=1;
    int idClusterAtual=-1;

    No* ultimo=solucao;
    while(ultimo->proximo!=NULL){///encontra o ultimo No da solucao;
        ultimo=ultimo->proximo;
    }
    No* aux = solucao;
    No* anterior=solucao;

    ///Cria uma lista de clusters e encadeia pelos anteriores
    while(aux!=NULL){
        if(aux->vertice->getIndiceCluster()!=idClusterAtual){
            if(controle==1){
                controle=0;
                Cluster* no= new Cluster();
                no->inicio=anterior;
                no->anterior=clusterFinal;
                clusterFinal=no;
                idClusterAtual=aux->vertice->getIndiceCluster();
                anterior=aux;
                aux=aux->proximo;

            }else{
                clusterFinal->fim=anterior;
                controle=1;
                anterior=aux;
                idClusterAtual=-1;
            }

        }else{
            anterior=aux;
            aux=aux->proximo;
        }

    }
    clusterFinal->fim=ultimo;
    Cluster *clusterInicial=clusterFinal;
    Cluster *auxcc=NULL;

    ///percorre a lista encadeada por anteriores ligando os proximos
    while(clusterInicial->anterior!=NULL){
            auxcc=clusterInicial;
            clusterInicial=clusterInicial->anterior;
            clusterInicial->proximo=auxcc;

    }
    clusterInicial->proximo=auxcc;
    Cluster *ultimoAdicionado=NULL;

    int custoAtual=calculaCustoSolucao(solucao);
    Cluster* cAtual=clusterInicial->proximo;
    int i=0;
    while(cAtual!=NULL){///efetua a busca local a partir da lista duplamente encadeada obtida
            Cluster* ant=cAtual->anterior;
            if(cAtual->anterior!=NULL&&cAtual->proximo!=NULL){
                trocaClusters(cAtual);
                int custo = calculaCustoSolucao(solucao);
                if(custo<custoAtual){
                    custoAtual=custo;
                    cAtual=clusterInicial->proximo;
                }else{
                    trocaClusters(ant->proximo);
                    cAtual=cAtual->proximo;
                }
            }else{
                cAtual=cAtual->proximo;
            }

    }

    ///desaloca a lista de clusters criada
    Cluster *c;
    c = clusterInicial;
    while(c!=NULL){
        c=c->proximo;
        delete clusterInicial;
        clusterInicial=c;
    }
    clusterInicial=NULL;
    clusterFinal=NULL;
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

///salva a solucao no arquivo de saida
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

///desaloca uma solucao com o ponteiro inicial dado pelo parametro solucao
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
