#ifndef ADJLIST_H_INCLUDED
#define ADJLIST_H_INCLUDED

#include "LinkList.h"

#define MAX 20

typedef struct EdgeNode
{
    int AjV; //index of adjacent vertex to the edge
    struct EdgeNode *next;
} EdgeNode;

 typedef struct VNode //vertex node
{
    int data; //to store the vertex which AjList[i] represents
    EdgeNode *firstE; //adjacent edge
} VNode, AjList[MAX];

typedef struct Graph {
    AjList ajList;
    int numV, numE;
} Graph;

typedef struct Graph_Extra //find the maximum vertex-induced subgraph
{
    Graph *graphs;
    int MarkedVertex[MAX];
} GraphExtra, *GraphEx;

typedef struct GraphList //store a list of graphs in an array as well as the number of graphs
{
    Graph *graphList[MAX]; //array of pointers to graphs
    int numberOfGraphs;
} GraphList;

//convert an undirected graph to a directed graph
typedef struct DirToUndir
{
    int block;
    LinkList VertexTraversed;
}DirToUndir, *dirToUndir;

typedef struct AddUndirEdges
{
    Graph *UndirectedG;
    int Vertex_Index[MAX]; //Vertex_Index[Vertex] = Index of the vertex in undirected graph->ajList[Index]
} AddUndirEdges, *addUndirEdges;

void CreateDirGraph (Graph *graph);
void printGraph(Graph *graph);
Graph *DirectToUndirect(Graph *DirectedGraph);

//generate leafs from a leaf and add the new leafs to que_A
int Generate_Leafs_from_a_leaf(addUndirEdges UndirGraph_index, Graph *DirectedGraph, dirToUndir Leaf, dirToUndir Que_A[], int whereLeaf, int Vertex_Finished[]);
void Generate_Que_B(addUndirEdges UndirGraph_index, Graph *DirectedGraph, dirToUndir Que_A[], int Vertex_Finished[]);
addUndirEdges IniUndirectedGraphFromDirGraph(Graph *DirectedGraph);
addUndirEdges AddEdgeUndirGraph (addUndirEdges UndirGraph_index, int a, int b); //add edge (a, b)

//convert directed graph to undirected graph by BFS
addUndirEdges IniUndirectedGraphFromDirGraph(Graph *DirectedGraph);
addUndirEdges AddEdgeUndirGraph (addUndirEdges UndirGraph_index, int a, int b); //add edge (a, b)


#endif // ADJLIST_H_INCLUDED
