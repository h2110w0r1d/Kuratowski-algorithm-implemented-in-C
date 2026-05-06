#include"stack.h"
#include"AdjList.h"
#include "TarjanDFS.h"
#include "LinkList.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define TRUE 1
#define FALSE 0

static int Low[MAX], Dfn[MAX], visited[MAX];
static int TarjSteps; // the order traversed
static int count; //total number of components in Tarjan
static int JNotLeastVertex;



//**********************************************step 1  Tarjan DFS *****************************************//
//must start from the index with lowest pre-rank otherwise Low and Dfn will be undefined at kk
void Tarjan(Graph *graph,Stack stack1, int k, cmp component) //search from vertex of index k,
{                                                            //store the number of components and the vertex of components following the order as in vertex in cmp component

    Low[k] = Dfn[k] = ++TarjSteps;//initialize the vertex
    push(k, stack1); //vertex goes into the stack
    visited[k] = 1; //vertex is in the stack


    EdgeNode *temp;
    for(int A = 0; A<graph->numV; A++)
    {
        if(k == graph->ajList[A].data)
        {
            temp = graph->ajList[A].firstE;
            break;
        }
    }

    while(temp) //find all the adjacent verteces
    {
        int kk = temp->AjV;
        if(Dfn[kk] == 0) //for unvisited vertex kk
        {
            Tarjan(graph, stack1, kk, component); //traverse vertex kk
            if(Low[kk] < Low[k]) {Low[k] = Low[kk];}
        }
        //if kk was visited, and kk is in the stack
        else if(Dfn[kk] < Low[k] && visited[kk]) {Low[k] = Dfn[kk];}
        temp = temp->next; //go to next vertex adjacent to vertex k
        }

    //backtracking, find the node with (Dfn = Low), and pop out all the nodes after it in the stack to form a connected component

    if(Low[k] == Dfn[k])
    {
      int numOfV = 0; //count the number of vertex in each component

      int a_thing;
      component->listOfCmp[count] = CreateLinkList();
      do
      {
       numOfV++;
       a_thing = pop(stack1);
       visited[a_thing] = 0;
       //printf("the low=%d th component %d \n", Low[k], a_thing);
       InitLinkList(a_thing, component->listOfCmp[count]); //insert the vertex in to the node attached to the head

       if(JNotLeastVertex > a_thing){JNotLeastVertex = a_thing; }

      }while(a_thing != k);


      //only component with length >= 5 stays in the list
      if(numOfV <= 4)
      {
        free(component->listOfCmp[count]);
        component->listOfCmp[count] = CreateLinkList();

      }
      else{
        component->listOfCmp[count]->data = numOfV;
        count++;
        component->numberOfComp = count;
        if(JohnsonLeastVertex > JNotLeastVertex) {JohnsonLeastVertex = JNotLeastVertex; JohnsonWhereIsLeastV = (count-1);}
      }

     }
    }

void findSCC(Graph *graph,Stack stack1, int k, cmp component)
{
  for(int i = 0; i < MAX; i++)
  {
      component->numberOfComp = 0;
      component->listOfCmp[i] = NULL;
  }
  memset(Low, 0, sizeof(Low));
  memset(Dfn, 0, sizeof(Dfn));
  memset(visited, 0, sizeof(visited));
  TarjSteps=0; // the order traversed
  count = 0; //total number of components in Tarjan

  JNotLeastVertex = 10000;
  JohnsonLeastVertex = 10000; //to find the least vertex within each SCC for Johnson algorithm
  JohnsonWhereIsLeastV = 0;

  int listOfV[MAX];
  memset(listOfV, -1, sizeof(listOfV));

  for(int j = 0; j<graph->numV; j++)
  {
    listOfV[graph->ajList[j].data] = 1;
  }

  for(int traverseV=0; traverseV < MAX; traverseV++)
  {
      if(listOfV[traverseV] == 1 && Dfn[traverseV] == 0)
      {
          Tarjan(graph,stack1, traverseV, component);
      }
  }
}


//unused!!!!!!!!!!!!!!!!!!!!!!!!!
//Initialize SCCList, extract SCC from the original graph and store the SCC in adjacent list
GraphList *Extract_Vertex_induced_subgraph(cmp component,Graph *graph) //component, original graph, array to store adjacent list of SCC
{
 //list of subgraphs
 GraphList *SCCList;
 //printf("number of cmp in extact: %d\n", component->numberOfComp);
 if(component->numberOfComp == 0) {printf("Fail to extract vertex induced subgraph.\n"); return NULL;}
 SCCList = CreateGraphList(SCCList, component->numberOfComp);
 SCCList->numberOfGraphs = component->numberOfComp;

    //initiate the subgraph according to the list of vertex (adj[i]), and mark the vertices
 int MarkedVertex[MAX], j = 0; //mark the vertex in the component, traverse the list of components
 Graph* subgraph;

 while(j<component->numberOfComp)
 {
    memset(MarkedVertex, 0, sizeof(MarkedVertex));

    LinkList Comp = component->listOfCmp[j]; //the j^th component
    int tmpArray[MAX]; //a temporary array to store vertex i and the index in subgraph->ajList[index] = i; tmpArray[i]=index, i = a vertex in one of the list of component
    memset(tmpArray, -1, sizeof(tmpArray));

    int index = 0, numberOfEdges = 0; //index in ajList[index] in subgraph, number of edges in the subgraph
    LinkList tmp = Comp->next;

    subgraph = (Graph *)malloc(sizeof(Graph));
    subgraph->numV = Comp->data;

    while(tmp != Comp)
    {
        int i = tmp->data;
        //printf("tmp->data = %d", tmp->data);
        subgraph->ajList[index].data = i;
        subgraph->ajList[index].firstE = NULL;
        tmpArray[i] = index;
        MarkedVertex[i] = 1; //mark the vertex
        index++;
        tmp = tmp->next;
    }

    //Iterate through original graph: adList[i], ->next->data.... = a
    for(int i = 0; i<graph->numV; i++)
    {
        int tmpVertex = graph->ajList[i].data;
        if(MarkedVertex[tmpVertex]) //the edge belong to SCC iff all the adjacent vertex in MarkedVertex
        {
           int tmpIndex = tmpArray[tmpVertex];
           //add all the adjacent edges to the subgraph
           EdgeNode *traverseEdge = graph->ajList[i].firstE;
           while(traverseEdge)
           {
             int addVertex = traverseEdge->AjV;
             EdgeNode* tmpNode;
             tmpNode = (EdgeNode *)malloc(sizeof(EdgeNode));
             if(MarkedVertex[addVertex])
            {
              if(tmpIndex == -1)
             {
                 printf("Extract_Vertex_induced_subgraph invalid.\n");
                 exit(1);
             }

              tmpNode->AjV = addVertex;
              tmpNode->next = subgraph->ajList[tmpIndex].firstE;
              subgraph->ajList[tmpIndex].firstE = tmpNode;

              numberOfEdges++;
            }
            traverseEdge = traverseEdge->next;
            tmpNode = NULL;
           }
        }
    }
    subgraph->numE = numberOfEdges;
    SCCList->graphList[j] = subgraph;
    subgraph = NULL;
    j++;
 }
    //return the list of subgraph
    return SCCList;
}



//Initialize SCCList, extract SCC from the original graph and store the SCC in adjacent list
Graph *Extract_Vertex_induced_K_subgraph_DirectedGraph(CommonEV ListOfVertexes,Graph *graph) //component, original graph, array to store adjacent list of SCC
{
 if(ListOfVertexes->listOfCommonV->data == 0) {printf("The number of vertexes is 0. Fail to extract vertex induced subgraph.\n"); return NULL;}

 ListOfVertexes->numberOfCommonE = 0;
 int MarkedVertex[MAX] = {0}, j = 0; //mark the vertex in the component, traverse the list of components
 Graph* subgraph;
 EdgeNode *traverseEdge, *tmpNode;
 int tmpArray[MAX] = { [0 ... (MAX-1)] = -1 }; //an array to store vertex i and the index in subgraph->ajList[index] = i; tmpArray[i]=index, i = a vertex in one of the list of component
 int index = 0, numberOfEdges = 0, tmpVertex, tmpIndex, addVertex; //index in ajList[index] in subgraph, number of edges in the subgraph
 LinkList tmp = ListOfVertexes->listOfCommonV->next;

 subgraph = (Graph *)malloc(sizeof(Graph));
 subgraph->numV = ListOfVertexes->listOfCommonV->data;

 while(tmp != ListOfVertexes->listOfCommonV)
 {
        int i = tmp->data;
        subgraph->ajList[index].data = i;
        subgraph->ajList[index].firstE = NULL;
        tmpArray[i] = index;
        MarkedVertex[i] = 1; //mark the vertex
        index++;

        tmp = tmp->next;
 }

 //Iterate through original graph
 for(int i = 0; i<graph->numV; i++)
 {
        tmpVertex = graph->ajList[i].data;
        if(MarkedVertex[tmpVertex]) //the edge belong to KSubgraph iff all the adjacent vertex in MarkedVertex
        {
           tmpIndex = tmpArray[tmpVertex];
           //add all the adjacent edges to the subgraph
           traverseEdge = graph->ajList[i].firstE;
           while(traverseEdge)
           {
             if( !MarkedVertex[traverseEdge->AjV] ) {traverseEdge = traverseEdge->next; continue;}
             addVertex = traverseEdge->AjV;
             tmpNode = (EdgeNode *)malloc(sizeof(EdgeNode));
              if(tmpIndex == -1)
              {
                 printf("Extract_Vertex_induced_subgraph invalid.\n");
                 exit(1);
              }

              tmpNode->AjV = addVertex;
              tmpNode->next = subgraph->ajList[tmpIndex].firstE;
              subgraph->ajList[tmpIndex].firstE = tmpNode;
              numberOfEdges++;
              traverseEdge = traverseEdge->next;
              tmpNode = NULL;
            }
           }
  }
 subgraph->numE = numberOfEdges;
 return subgraph;
 }





