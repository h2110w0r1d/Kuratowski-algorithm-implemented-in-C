#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#include "AdjList.h"
#include "TarjanDFS.h"



void CreateDirGraph (Graph *graph) //vertex start from 0
{

    EdgeNode *edge;
    if(!graph)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    memset(graph, 0, sizeof(Graph));

    printf("what's the number of vertexes and edges? What are the edges? format: (a b)\n");
    printf("The index of vertex should star from 0.\n");
    scanf("%d %d", &graph->numV, &graph->numE);

    if(graph->numE)
    {
        for(int i = 0; i<(graph->numV); i++) //create the list of vertexes
        {
            graph->ajList[i].data = i;
            graph->ajList[i].firstE = NULL;

        }

        int a, b;
        printf("what are the edges (Vi Vj)?\n");
        for(int i = 0; i<(graph->numE); i++) //create adjacent list
        {
            fflush(stdin); //get rid of '\n' in stdin
            //printf("what are the edges (Vi Vj)?\n");
            scanf("%d %d", &a, &b);

            edge = (EdgeNode *)malloc(sizeof(EdgeNode));
            //insert a node into the head of the list
            edge->AjV = b;
            edge->next = graph->ajList[a].firstE;
            graph->ajList[a].firstE = edge;

        }
    }

}

Graph *InitEmpDirGraph (Graph *graph) //initialize an empty graph
{
    graph = (Graph *)malloc(sizeof(Graph));

    if(!graph)
    {
        printf("fail to allocate memory in InitEmpDirGraph.\n");
        exit(1);
    }
    graph->numE = 0;
    graph->numV = 0;
    for(int i=0; i<MAX; i++)
    {
        graph->ajList[i].firstE = NULL;
        graph->ajList[i].data = -1;
    }

    return graph;
}

void printGraph(Graph *graph)
{
    if(graph == NULL)
    {
        printf("empty graph.\n");
        return;
    }
    else if(graph->numV== 0)
    {
        printf("empty graph.\n");
        return;
    }
    for (int i = 0; i< (graph->numV); i++)
    {
        printf("%d ", graph->ajList[i].data);
        EdgeNode *temp = graph->ajList[i].firstE;
        while(temp)
        {
            printf("%d ", temp->AjV);
            temp = temp->next;
        }
        printf("\n");
    }

}

void printGraphB(Graph *graph)
{
    for (int i = 0; i< (5); i++)
    {
        printf("%d ", graph->ajList[i].data);
        EdgeNode *temp = graph->ajList[i].firstE;
        while(temp)
        {
            printf("%d ", temp->AjV);
            temp = temp->next;
        }
        printf("\n");
    }

}


//remove all the nodes with data = ... given the data is in the Edge
void EdgeNodeDelete(EdgeNode *head, int value)
{
    EdgeNode *p1 = head->next;
    EdgeNode *p3 = p1;
    EdgeNode *p2 = head;

    while(p1)
    {
        if(p1->AjV == value)
        {
           p1 = p1->next;
           p2 ->next = p1;

           free(p3);

           p3 = p1;
        }
    }
}

int EdgeNodeGet (EdgeNode *head, int index) //input the index of a vertex and return if the vertex is in or not
{
    EdgeNode *traverse;
    traverse = head->next;
    while(traverse)
    {
        if(index == traverse->AjV)
        {
            return 1;
        }
    }
    return 0;
}

GraphList *CreateGraphList (GraphList *graphs, int GraphNumber) //assign memory to the list of graphs
{
  graphs = (GraphList *)malloc(sizeof(GraphList));
  graphs->numberOfGraphs = GraphNumber;
  for(int i=0; i<GraphNumber; i++)
  {
      graphs->graphList[i] = InitEmpDirGraph(graphs->graphList[i]);

  }

  return graphs;
}


//convert directed graph to undirected graph, always keep one edge adjacent to two pair of vertextes
//even if there are more than one edge adjacent to two vertexes in the original (directed) graph

Graph *DirectToUndirect(Graph *DirectedGraph) //use BFS to avoid repeating to add undirected edge
{
    //a vertex is in the leaf iff it's in the store_vertex
    // starting from InitialVertex (vertex a) in directed graph, traverse (a->bi) for all bi adjacent to a
    //add undirected edge (a, bi), add bi to a leaf
    //add a to Vertex_Finished[]
    //starting from the leaf, continue ***

    addUndirEdges UndirGraph_index = IniUndirectedGraphFromDirGraph(DirectedGraph);

    int InitVertex;
    dirToUndir Que_A[MAX];
    int Vertex_Finished[MAX] = {0};


// starting from InitialVertex in directed graph (vertex a), traverse (a->bi)

    for(int i = 0; i<DirectedGraph->numV; i++) //traverse every connected component
    {
            if(Vertex_Finished[DirectedGraph->ajList[i].data]) continue;
            for(int i = 0; i<MAX; i++) Que_A[i] = NULL;
            int isEmpty1 = 0;
            dirToUndir Leaf = (dirToUndir)malloc(sizeof(DirToUndir));

            InitVertex = DirectedGraph->ajList[i].data;
            Leaf->block = InitVertex;
            Leaf->VertexTraversed = CreateLinkList();
            EdgeNode *AdjacentEdges = DirectedGraph->ajList[i].firstE;
        //add undirected edge (a, bi), add bi to a leaf
            while(AdjacentEdges)
            {
                isEmpty1 = 1;
                AddEdgeUndirGraph(UndirGraph_index, InitVertex, AdjacentEdges->AjV);

                InitLinkList(AdjacentEdges->AjV, Leaf->VertexTraversed);
                AdjacentEdges = AdjacentEdges->next;
            }
            if(!isEmpty1) Leaf = NULL; //in case if the graph is singular
            Que_A[0] = Leaf;
            Leaf = NULL;
        //add a to Vertex_Finished
            Vertex_Finished[InitVertex] = 1;

            //*** input: undirected graph, directed graph, que, Vertex_finished
            //start from vertex a in leaf_i, traverse (a->bi) for all bi adjacent to a in directed graph, add a to Vertex_finished
            //if bi not in Vertex_finished, add bi to a leaf, add undirected edge (a, bi)
            //if bi in Vertex_finished and if bi not equal to leaf->block, add undirected edge (a, bi)   (to avoid repeating to add multiple undirected edge into the undirected graph)
            //repeat all the above steps until all the vertexes in leaf_A are traversed, add the new leafs to que_A
            //for each leaf in the que_A, repeat the above steps, add the new leafs to another que_B
            //if all the leafs in the que_A are traversed, start from the que_B, free que_A
            //if there's no more leaf to be added, ends and output the undirected graph

            Generate_Que_B(UndirGraph_index, DirectedGraph, Que_A, Vertex_Finished);
    }

    return UndirGraph_index->UndirectedG;
}


    //generate leafs from a leaf and add the new leafs to que_A
int Generate_Leafs_from_a_leaf(addUndirEdges UndirGraph_index, Graph *DirectedGraph, dirToUndir Leaf, dirToUndir Que_A[], int whereLeaf, int Vertex_Finished[])
{
    int a, whereIsTempRootDir, b_i, nonEmpty;
    LinkList traverseLeaf = Leaf->VertexTraversed->next;
    EdgeNode *traverseDirGraph;
    dirToUndir Leaf_2;


    while(traverseLeaf != Leaf->VertexTraversed)
    {
        nonEmpty = 0;
        if(Vertex_Finished[traverseLeaf->data]) {traverseLeaf = traverseLeaf->next; continue;}
        //start from vertex a in leaf_i, traverse (a->bi) for all bi adjacent to a in directed graph
        a = traverseLeaf->data; //start from vertex a in leaf_i
        whereIsTempRootDir = UndirGraph_index->Vertex_Index[a];
        traverseDirGraph = DirectedGraph->ajList[whereIsTempRootDir].firstE;

        Vertex_Finished[a] = 1;
        Leaf_2 = (dirToUndir)malloc(sizeof(DirToUndir));
        Leaf_2->VertexTraversed = CreateLinkList();
        Leaf_2->block = a;
        while(traverseDirGraph)
        {
            nonEmpty = 1;
            b_i = traverseDirGraph->AjV;
            //if bi not in Vertex_finished, add bi to a leaf, add undirected edge (a, bi)
            if( !Vertex_Finished[b_i] ) {InitLinkList(b_i, Leaf_2->VertexTraversed); UndirGraph_index = AddEdgeUndirGraph(UndirGraph_index, b_i, Leaf_2->block);}
            //if bi in Vertex_finished and if bi not equal to leaf->block, add undirected edge (a, bi)
            else if( b_i != Leaf->block ) UndirGraph_index = AddEdgeUndirGraph(UndirGraph_index, b_i, Leaf_2->block);
            traverseDirGraph = traverseDirGraph->next;
        }
        //repeat all the above steps until all the vertexes in leaf_A are traversed
        traverseLeaf = traverseLeaf->next;
        //add the new leafs to que_A
        if(!nonEmpty) free(Leaf_2);
        else Que_A[whereLeaf++] = Leaf_2;
        Leaf_2 = NULL;
    }

    return whereLeaf; //return (how many leafs have been added to the que_A)
}

void Generate_Que_B(addUndirEdges UndirGraph_index, Graph *DirectedGraph, dirToUndir Que_A[], int Vertex_Finished[])
{
    if(!Que_A[0]) return; // if there's no more leaf to be added, ends (and output the undirected graph LATTER)
    dirToUndir Que_B[MAX];
    for(int i = 0; i<MAX; i++) Que_B[i] = NULL;
    int traverseQue_A = 0, whereLeaf = 0;
    while(Que_A[traverseQue_A])
    {
        dirToUndir tmpLeaf_A = Que_A[traverseQue_A];
        //for each leaf in the que_A, repeat the above steps, add the new leafs to another que_B
        whereLeaf = Generate_Leafs_from_a_leaf(UndirGraph_index, DirectedGraph, tmpLeaf_A, Que_B, whereLeaf, Vertex_Finished);
        traverseQue_A++;
    }
    //if all the leafs in the que_A are traversed, start from the que_B, free que_A
    for(int i = 0; i<MAX; i++) {free(Que_A[i]); Que_A[i] = NULL;}
    Generate_Que_B(UndirGraph_index, DirectedGraph, Que_B, Vertex_Finished);
}

addUndirEdges IniUndirectedGraphFromDirGraph(Graph *DirectedGraph)
{
    addUndirEdges UndirGraph_index = (addUndirEdges)malloc(sizeof(AddUndirEdges));
    for(int i = 0; i<MAX; i++)
    {
        UndirGraph_index->Vertex_Index[i] = -1;
    }

    Graph *UndirectedGraph = (Graph *)malloc(sizeof(Graph));
    UndirectedGraph->numV = DirectedGraph->numV;
    UndirectedGraph->numE = 0;
    for(int i = 0; i<DirectedGraph->numV; i++)
    {
        UndirectedGraph->ajList[i].data = DirectedGraph->ajList[i].data;
        UndirectedGraph->ajList[i].firstE = NULL;
        UndirGraph_index->Vertex_Index[DirectedGraph->ajList[i].data] = i;
    }
    UndirGraph_index ->UndirectedG = UndirectedGraph;

    return UndirGraph_index;
}

addUndirEdges AddEdgeUndirGraph (addUndirEdges UndirGraph_index, int a, int b) //add edge (a, b)
{
    EdgeNode *addEdge;
    int WhereIsVertex;
//add (a, b)
    WhereIsVertex = UndirGraph_index->Vertex_Index[a];
    addEdge = (EdgeNode *)malloc(sizeof(EdgeNode));
    addEdge->AjV = b;
    addEdge->next = UndirGraph_index->UndirectedG->ajList[WhereIsVertex].firstE;
    UndirGraph_index->UndirectedG->ajList[WhereIsVertex].firstE = addEdge;
    addEdge = NULL;
//add(b,a)
    WhereIsVertex = UndirGraph_index->Vertex_Index[b];
    addEdge = (EdgeNode *)malloc(sizeof(EdgeNode));
    addEdge->AjV = a;
    addEdge->next = UndirGraph_index->UndirectedG->ajList[WhereIsVertex].firstE;
    UndirGraph_index->UndirectedG->ajList[WhereIsVertex].firstE = addEdge;
    addEdge = NULL;

    UndirGraph_index->UndirectedG->numE += 2;

    return UndirGraph_index;
}



CommonEV Vertex_From_Graph(Graph *g)
{
    CommonEV component = (CommonEV)malloc(sizeof(CommonEdgeVertex));
    component->listOfCommonV = CreateLinkList();
    component->numberOfCommonE = -1;

    for(int i = 0; i<g->numV; i++)
    {
        InitLinkList(g->ajList[i].data, component->listOfCommonV);
    }
    component->listOfCommonV->data =g->numV;
    return component;
}

