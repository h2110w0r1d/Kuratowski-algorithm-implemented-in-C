#ifndef JOHNSONCYCLEALGO_H_INCLUDED
#define JOHNSONCYCLEALGO_H_INCLUDED
#include"AdjList.h"
#include"LinkList.h"
#include "TarjanDFS.h"
#include <stdio.h>
#include <stdlib.h>
#include<math.h>

#define MAX2 1000

typedef struct Cycle
{
    int number_of_Cycles;
    LinkList listOfCycles[MAX2]; //head->data = number of vertexes in the cycle, the rest stores the cycle, the first cycle in the list: listOfCycles[0]
} Cycle;

typedef struct SAS
{
    int *sorted; //sorted[original Vertex] = Vertex start from 1
    int *antisorted; //antisorted[vertex start from 1] = original vertex
    int numOfVertex;
} SAS;


//declare Graph *ConvertedGraph, set n = numV in original graph,the least index in ConvertedGraph is 1
Cycle *JohnsonFindCycles(Graph* ConvertedGraph, Stack stack2, Graph *B);
cmp GenerateListOfVertex (int n, int s); //NumVertex = number of vertexes, to store the {s, s+1, ..., n} in cmp component

int CIRCUIT(int v, Graph *SCC, Cycle *cycles, Stack stack2, Graph *B);//need to malloc + initiate the LinkList!!!! before the function is called
void UNBLOCK (int u, Graph *B); //numV and numE in B is uninitialized
void PrintAllCycle (Cycle *cycles);
SAS *BucketSort(Graph *graph);//sort the vertexes in the graph from small to large and store the vertexes in an integer list
Graph *VertexStartFromOne(Graph *graph, SAS *VertexSorted); //convert original vertex to vertexes start from 1, then 2, 3, 4, ..
Cycle *ReturnToIniVertex (Cycle *cycles, SAS *VertexSorted); //return to initial vertex


#endif // JOHNSONCYCLEALGO_H_INCLUDED
