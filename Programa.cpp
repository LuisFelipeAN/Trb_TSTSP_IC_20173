#include "Programa.h"
#include "Vertice.h"

typedef struct _cluster{
  int id;
} Cluster;

typedef struct _tabu{
  Vertice* primeiro;
  Cluster* cluster;
  bool visitado;
} Tabu;



