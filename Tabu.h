#ifndef TABU_H
#define TABU_H
#include "Vertice.h"

struct NoTabu{
    NoTabu* proximo;
    Vertice* vertice;
};
class Tabu
{
    public:
        bool visitado;
        void insereVertice(Vertice *v);
        Vertice * outroVertice(Vertice *v);
        Tabu();
        virtual ~Tabu();

    protected:


    private:
    NoTabu *primeiroNo;
    NoTabu *ultimoNo;

};

#endif // TABU_H
