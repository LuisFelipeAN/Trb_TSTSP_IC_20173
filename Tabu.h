#ifndef TABU_H
#define TABU_H
#include "Vertice.h"
///estrutura para encadear os vertices presentes no tabu
struct NoTabu{
    NoTabu* proximo;
    Vertice* vertice;
};

class Tabu
{
    public:
        bool visitado;
        void insereVertice(Vertice *v);
        Vertice *outroVertice(Vertice *v);
        Vertice *getRandom();
        void imprimeVertices();
        Tabu();
        virtual ~Tabu();

    protected:


    private:
    int numVertices;
    NoTabu *primeiroNo;
    NoTabu *ultimoNo;

};

#endif // TABU_H
