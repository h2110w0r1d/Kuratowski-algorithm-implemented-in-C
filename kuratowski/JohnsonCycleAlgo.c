#include "JohnsonCycleAlgo.h"
#include"AdjList.h"
#include"LinkList.h"
#include "TarjanDFS.h"

#define TRUE 1
#define FALSE 0

static int plannar;

static int blocked[MAX]; //initially = {0}
static int s, number_of_cycles; //initially: s = 1, num_of_cycles = 0;

static int numOfVInCycle; //numOfCycles


void UNBLOCK (int u, Graph *B) //numV and numE in B is uninitialized
{
    blocked[u] = FALSE;
    VNode tempNode;
    int record1;
    for(int i = 0; i<MAX; i++)
    {
        tempNode = B->ajList[i];
        if(tempNode.data == u){record1 = i; break;}
    } //find B(u)
    EdgeNode *p2 = B->ajList[record1].firstE;

    while(B->ajList[record1].firstE)
    {
        int w = B->ajList[record1].firstE->AjV;
        B->ajList[record1].firstE = B->ajList[record1].firstE->next;
        free(p2);
        p2 = B->ajList[record1].firstE;
        (B->numV)--;
        (B->numE)--;

        if(blocked[w])
        {
            UNBLOCK(w, B);
        }
    }
}



int CIRCUIT(int v, Graph *SCC, Cycle *cycles, Stack stack2, Graph *B)//need to malloc + initiate the LinkList!!!! before the function is called
{
   int f = FALSE;
   push(v, stack2);
   blocked[v] = TRUE;
   EdgeNode *tmp = NULL;


   for(int VertexIndex = 0; VertexIndex<SCC->numV; VertexIndex++) //SCC->ajList[v].data may not be v
   {
        int vertex = SCC->ajList[VertexIndex].data;
        if(v == vertex) {tmp = SCC->ajList[VertexIndex].firstE; break;}
   }//find A_k[v]

   while(tmp)
   {
       int w = tmp->AjV;
       if (w == s)//find a cycle
       {
          f = TRUE;
          LinkListNode *tmpCycle;
          Stack p = stack2->next;
          int VertexCycle = w;

         do
         {
          VertexCycle = p->data;

          tmpCycle = (LinkListNode *)malloc(sizeof(LinkListNode));
          tmpCycle->data = VertexCycle;
          tmpCycle->next = cycles->listOfCycles[number_of_cycles]->next;
          cycles->listOfCycles[number_of_cycles]->next = tmpCycle;
          numOfVInCycle++;

          p = p->next;
         }while(p);

         if(numOfVInCycle <= 4)
         {
            free(cycles->listOfCycles[number_of_cycles]);
            cycles->listOfCycles[number_of_cycles] = CreateLinkList();

         }
         else{

            cycles->listOfCycles[number_of_cycles]->data = numOfVInCycle;
            number_of_cycles++;
            plannar = FALSE;
         }
        numOfVInCycle = 0;
       }
       else if (!blocked[w]) {if (CIRCUIT(w, SCC, cycles, stack2, B)) f = TRUE;}
       tmp = tmp->next;
   }
   if(f) {UNBLOCK(v, B);}
   else
   {
      VNode Temp;
      for(int i = 0; i<SCC->numV; i++)
      {
          Temp = SCC->ajList[i];
          if(Temp.data == v){break;}
      }//find A_K[v]
      EdgeNode *tmpSCC = Temp.firstE; //find A_k(v)

      while(tmpSCC)
      {
          int w = tmpSCC->AjV;

          VNode tempNode;
          EdgeNode *tmpB;
          int record;
          for(int i = 0; i<MAX; i++)
          {
            tempNode = B->ajList[i];
            if(tempNode.data == w){tmpB = B->ajList[i].firstE; record = i; break;}
          }//find B(w)


          int In = FALSE;
          while(tmpB) //traverse B(w)
          {
            if(v == tmpB->AjV)
            {
               In = TRUE;
               break;
            }
            tmpB = tmpB->next;
          }

          if(!In)
          {
              //printf("\nadd %d into B(%d): yes\n", v, w);

              EdgeNode *a_Node;
              a_Node = (EdgeNode *)malloc(sizeof(EdgeNode));
              a_Node->AjV = v;
              a_Node->next = B->ajList[record].firstE;
              B->ajList[record].firstE = a_Node; //insert the Node to the head of B(w)
              (B->numV)++;
              (B->numE)++;

          }
          tmpSCC = tmpSCC->next;
      }
   }
   DeleteInStack(stack2, v); //unstack v, but v may not be in the top element of the stack
   return f;
}

cmp GenerateListOfVertex (int n, int s) //NumVertex = number of vertexes, to store the {s, s+1, ..., n} in cmp component
{
    cmp component = (cmp)malloc(sizeof(Component));
    component->listOfCmp[0] = CreateLinkList();
    component->numberOfComp = 1;
    component->listOfCmp[0]->data = n - s + 1; //store the number of vertex
    while(s<=n)
    {
        InitLinkList(s, component->listOfCmp[0]);
        s++;
    }
    return component;
}


SAS *BucketSort(Graph *graph)//sort the vertexes in the graph from small to large and store the vertexes in an integer list
{
    SAS *SortVertex = (SAS *)malloc(sizeof(SAS));

    int maximum = graph->numV;
    int sorting[MAX]; //in a subgraph, the maximum index of vertexes may be > numV
    memset(sorting, 0, sizeof(sorting));

    SortVertex->sorted = (int *)malloc(MAX*sizeof(int));
    SortVertex->antisorted = (int *)malloc(MAX * sizeof(int));

    for(int i = 0; i<maximum; i++)
    {
      int Vertex = graph->ajList[i].data;
      sorting[Vertex] = 1;
    }

    for(int i= 0; i<MAX-1; i++)
    {
        if(sorting[i]) {SortVertex->antisorted[(i+1)] = i; SortVertex->sorted[i] = i+1;} //i+1 = New vertex, i = original vertex
    }

    SortVertex->numOfVertex = maximum;

    return SortVertex;
}

Graph *VertexStartFromOne(Graph *graph, SAS *VertexSorted) //convert original vertex to vertexes start from 1, then 2, 3, 4, ..
{

    for(int i = 0; i<graph->numV; i++)
    {
        int OriginalVertex = graph->ajList[i].data;
        int ConvertedVertex = VertexSorted->sorted[OriginalVertex];
        graph->ajList[i].data = ConvertedVertex;

        EdgeNode* tmp = graph->ajList[i].firstE;
        while(tmp)
        {
            int OriginalVertex = tmp->AjV;
            int ConvertedVertex = VertexSorted->sorted[OriginalVertex];
            tmp->AjV = ConvertedVertex;
            tmp = tmp->next;
        }
    }

    return graph;
}

Cycle *ReturnToIniVertex (Cycle *cycles, SAS *VertexSorted) //return to initial vertex
{
    for(int i = 0; i<cycles->number_of_Cycles; i++)
    {
        LinkList tmp = cycles->listOfCycles[i]->next;

        while(tmp!= cycles->listOfCycles[i])
        {
            int ConvertedVertex = tmp->data;
            int OriginalVertex = VertexSorted->antisorted[ConvertedVertex];
            tmp->data = OriginalVertex;
            tmp = tmp->next;
        }
    }

    return cycles;
}

Graph *IniDirGraphB (Graph *B)
{
    B = (Graph *)malloc(sizeof(Graph));
    B->numE = B->numV = 0;
    for(int i = 0; i<MAX; i++)
    {
       B->ajList[i].data = (i+1);
       B->ajList[i].firstE = NULL;
    }
    return B;
}

//declare Graph *ConvertedGraph, set n = numV in original graph,the least index in ConvertedGraph is 1

Cycle *JohnsonFindCycles(Graph* ConvertedGraph, Stack stack2, Graph *B)
{
    B = IniDirGraphB(B);
    number_of_cycles = 0;
    numOfVInCycle = 0;
    int n = ConvertedGraph->numV;
    int plannar = FALSE;
    cmp component;
    cmp Temporary;
    Stack stack3;
    GraphList *ListOfSCC;
    GraphList *ListOfSubgraph;

    Cycle *cycles = (Cycle *)malloc(sizeof(Cycle));
    for(int i=0; i<MAX2; i++)
    {
        cycles->listOfCycles[i]=CreateLinkList();
    }
    s = 1;
    while(s<n)
    {
        plannar = FALSE;
        if(n - s + 1<= 4)
        {
            break; //we want only cycles with number of vertexes >=5
        }
        component = GenerateListOfVertex(n, s);
        cmp TarjanSCC = (cmp)malloc(sizeof(Component));
        stack2 = CreateStack();
        if(B == NULL)
        {
          B = IniDirGraphB(B);
        }

        //retrieve the SCC in the subgraph induced by {s, s+1, ..., n}
        ListOfSubgraph = Extract_Vertex_induced_subgraph(component, ConvertedGraph); //only one subgraph generated in this case

        stack3 = CreateStack();
        findSCC(ListOfSubgraph->graphList[0], stack3, s, TarjanSCC);
        free(stack3);
        stack3 = NULL;

        //printf("Tarjan->numberOfComp = %d\n", TarjanSCC->numberOfComp);
        if(!TarjanSCC->numberOfComp){plannar = TRUE;}

        if(!plannar)//to ensure nonempty A_k
        {
                //find the SCC whose least vertex is the least among all SCC belonging to the same subgraph {s, s+1, ..., n}
                Temporary = (cmp)malloc(sizeof(Component));
                Temporary->numberOfComp = 1;
                Temporary->listOfCmp[0] = TarjanSCC->listOfCmp[JohnsonWhereIsLeastV];


                ListOfSCC = Extract_Vertex_induced_subgraph(Temporary, ConvertedGraph);
                free(Temporary);
                Temporary = NULL;

                Graph *A_k = ListOfSCC->graphList[0];
                        //iterate through the A_k (SCC) belong to the subgraph

                        //find s = least vertex in V_k, and traverse V_k, set B(i) = empty
                int Dummy = 10000;

                for(int tmpIndex = 0; tmpIndex<A_k->numV; tmpIndex++)
                {
                                    int least = A_k->ajList[tmpIndex].data;
                                    if(Dummy > least) Dummy = least;
                                    blocked[least] = FALSE;
                }
                s = Dummy;
                int dummy = CIRCUIT(s, A_k, cycles, stack2, B);

        }
        s++;

        free(TarjanSCC);
        TarjanSCC = NULL;
        free(B);
        B = NULL;
        free(stack2);
        stack2 = NULL;
    }
    cycles->number_of_Cycles = number_of_cycles;

    free(component);
    component = NULL;
    return cycles;
    }



void PrintAllCycle (Cycle *cycles)
{
    for(int i = 0; i<cycles->number_of_Cycles; i++)
    {
        printf("the %d th cycle:", i);
        printLinkList(cycles->listOfCycles[i]);
        printf("\n");
    }
}


