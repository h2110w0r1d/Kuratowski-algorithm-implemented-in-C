#ifndef TARJANDFS_H_INCLUDED
#define TARJANDFS_H_INCLUDED
#include"LinkList.h"
#include "AdjList.h"
#include "stack.h"
#include "AdjList.h"

int JohnsonLeastVertex; //to find the least vertex within each SCC for Johnson algorithm
int JohnsonWhereIsLeastV;

typedef struct Component
{
    int numberOfComp; //store the number of component
    LinkList listOfCmp[50]; //each element in the array stores the head of each component, head->data = number of vertex in the component
}Component, *cmp;



typedef struct CommonEdgeVertex
{
    int numberOfCommonE; //store the total number of common edges
    LinkList listOfCommonV; //store the total number of common vertex (in head->data) and the indexes
}CommonEdgeVertex, *CommonEV;



void Tarjan(Graph *graph, Stack stack1, int k, cmp component);

//initialize SCCList, extract SCC from the original graph and store the SCC in adjacent list
GraphList *Extract_Vertex_induced_subgraph(cmp component,Graph *graph); //component, original graph, array to store adjacent list of SCC
void findSCC(Graph *graph,Stack stack1, int k, cmp component);
Graph *Extract_Vertex_induced_K_subgraph_DirectedGraph(CommonEV ListOfVertexes,Graph *graph); //component, original graph, array to store adjacent list of SCC


#endif // TARJANDFS_H_INCLUDED
