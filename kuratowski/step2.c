#include"stack.h"
#include"AdjList.h"
#include "TarjanDFS.h"
#include "LinkList.h"
#include "JohnsonCycleAlgo.h"
#include "step2.h"

#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define TRUE 1
#define FALSE 0
static LinkList SGC_1, SGC_2;
//**************************************step 2*****************************************//



CommonEV CommonVertex(LinkList head1, LinkList head2) //follows TraceLinkList to output the common vertex of two circuitss
{
    int Mark[MAX] = {0}, storeEdges[MAX] = {0}, whereVertexInStoreEdge[MAX] ={0}, Index = 0, whereVertex, numVertex = 0;
    LinkList p1 = head1->next, p2 = head2->next, p2Previous = head2;
    CommonEV commonEdgeVertex = (CommonEV)malloc(sizeof(CommonEdgeVertex));
    commonEdgeVertex->listOfCommonV = CreateLinkList();
    commonEdgeVertex->numberOfCommonE = 0;

    while(p1 != head1)
    {
        Mark[p1->data] = 1;
        storeEdges[Index] = p1->data;
        whereVertexInStoreEdge[p1->data] = Index;
        p1 = p1->next;
        Index++;
    }
    storeEdges[Index] = head1->next->data; //store the last edge in the circuit

    while(p2 != head2)
    {
        if( !Mark[p2->data] ) {p2 = p2->next; p2Previous = p2Previous->next; continue;}
        numVertex++; //number of common vertex
        commonEdgeVertex->listOfCommonV = InitLinkList(p2->data, commonEdgeVertex->listOfCommonV);
        if( Mark[p2->next->data] ) //count how many edge
        {
            whereVertex = whereVertexInStoreEdge[p2->data];
            if(storeEdges[++whereVertex] == p2->next->data ) {commonEdgeVertex->numberOfCommonE++;}//find a common edge
        }
        p2 = p2->next;
        p2Previous = p2Previous->next;
    }
    if( Mark[p2->next->data] ) //test the last edge
    {
            whereVertex = whereVertexInStoreEdge[p2Previous->data];
            if(storeEdges[++whereVertex] == p2->next->data ) {commonEdgeVertex->numberOfCommonE++;}//find a common edge
    }
    commonEdgeVertex->listOfCommonV->data = numVertex;
    return commonEdgeVertex;
}

void printCommonEdgeVertex(CommonEV commonEV) //for debug
{
    printf("the number of common edges: %d\n", commonEV->numberOfCommonE);
    printf("the number of common vertexes: %d\n", commonEV->listOfCommonV->data);
    if(commonEV->listOfCommonV->data)
    {
        printf("the list of common vertexes:\n");
        printLinkList(commonEV->listOfCommonV);
    }
}

Cycle *generateCycles(void) // for debug
{
    Cycle *cycles = (Cycle *)malloc(sizeof(Cycle));

    int number, i=0;
    printf("the number of cycle you want:\n");
    scanf("%d", &number);

    cycles->number_of_Cycles = number;
    LinkList head = CreateLinkList();
    while(i<number)
    {
        head = scanfLinkList(head);
        cycles->listOfCycles[i] = head;
        head = NULL;
        head = CreateLinkList();
        i++;
    }
    return cycles;
}

//the step 2 function body, returns K5 or K3,3 for non-planar graph, and return NULL for planar graph
//the function finds only on possibility
kuratowski_subgraphs Step2_CompareComponent (Cycle *cycles)
{

    if(cycles->number_of_Cycles<2) {return NULL;}
    kuratowski_subgraphs KSubgraphs = (kuratowski_subgraphs)malloc(sizeof(K_subgraphs));
    KSubgraphs->IfK33 = 0;
    KSubgraphs->IfK5 = 0;
    KSubgraphs->K33 = NULL;
    KSubgraphs->K5 = NULL;

    for(int compare1 = 0; compare1 < (cycles->number_of_Cycles-1); compare1++)
    {
        SGC_1 = cycles->listOfCycles[compare1];
        for(int compare2 = compare1+1; compare2 < cycles->number_of_Cycles; compare2++)
        {
            SGC_2 = cycles->listOfCycles[compare2];
            CommonEV commonEV = CommonVertex(SGC_1, SGC_2);
            if(commonEV->numberOfCommonE == 0 && commonEV->listOfCommonV->data == 5)
            {
                Graph *K5Subdivision = S3TraseCommonV(commonEV, SGC_1, SGC_2);
                if(K5Subdivision != NULL)
                {
                    KSubgraphs->IfK5 = 1;
                    KSubgraphs->K5 =K5Subdivision;
                    return KSubgraphs;
                }
                else
                {
                    continue;
                }
            }
            else
            {
                //go to step 4
                Graph *MaybeK33 = step4(commonEV);
                KSubgraphs->IfK33 = K3OrNot;
                KSubgraphs->K33 = MaybeK33;
                if(KSubgraphs->K33 != NULL) return KSubgraphs;
            }
        }
        free(SGC_1);
    }
    return NULL;

}



//***************************** step 3 *****************************************

Graph *S3TraseCommonV( CommonEV commonEV, LinkList SGC_1, LinkList SGC_2)
{
    int SGC_1CommonVertex[6], mark[MAX];
    memset(SGC_1CommonVertex, -1, sizeof(SGC_1CommonVertex));
    memset(mark, 0, sizeof(mark));

    LinkList head = commonEV->listOfCommonV->next;
    LinkList TraverseSG1 = SGC_1->next;

    //mark the common vertex
    while(head != commonEV->listOfCommonV)
    {
        mark[head->data] = 1;
        head = head->next;
    }

    //traverse SG(C1) and order the common vertex
    int SGC_1Order = 1;
    while(TraverseSG1 != SGC_1)
    {
        if(mark[TraverseSG1->data])
        {
            SGC_1CommonVertex[SGC_1Order++] = TraverseSG1->data;
        }
        TraverseSG1 = TraverseSG1->next;
    }

    //traverse SG(C2) and check the order
    LinkList TraverseSG2 = SGC_2->next;

    //start to traverse from SGC_1CommonVertex[1]
    int checkOrder[6] = {-1, SGC_1CommonVertex[1], SGC_1CommonVertex[3], SGC_1CommonVertex[5], SGC_1CommonVertex[2], SGC_1CommonVertex[4]};
    int checkOrder2[6] = {-1, SGC_1CommonVertex[1], SGC_1CommonVertex[4], SGC_1CommonVertex[2], SGC_1CommonVertex[5], SGC_1CommonVertex[3]};
    while(TraverseSG2->data!= checkOrder[1])
    {
        TraverseSG2 = TraverseSG2->next;
        if(TraverseSG2 == SGC_2) TraverseSG2 = TraverseSG2->next;
    }

    LinkList start = TraverseSG2;
    int Index = 1;
    while(Index<6)
    {
        if(TraverseSG2 != SGC_2 && mark[TraverseSG2->data] == 1 && TraverseSG2->data != checkOrder[Index])
        {
            break;
        }
        else if(TraverseSG2 != SGC_2 && mark[TraverseSG2->data] == 1 && TraverseSG2->data == checkOrder[Index])
        {
            Index++;
        }
        TraverseSG2 = TraverseSG2->next;
    }

    if(Index < 6)
    {
        Index = 1;
        TraverseSG2 = start;
        while(Index<6)
        {
            if(TraverseSG2 != SGC_2 && mark[TraverseSG2->data] == 1 && TraverseSG2->data != checkOrder2[Index])
            {
                return NULL;
            }
            else if(TraverseSG2 != SGC_2 && mark[TraverseSG2->data] == 1 && TraverseSG2->data == checkOrder2[Index])
            {
                Index++;
            }
            TraverseSG2 = TraverseSG2->next;
        }
    }


    //S_G(C_1+C_2) is a K5 graph, nonplannar G
    Graph *K5Subdivision = SG_C1_U_C2(commonEV, SGC_1, SGC_2);
    return K5Subdivision;
}


Graph *SG_C1_U_C2(CommonEV commonEV, LinkList SGC_1, LinkList SGC_2)
{
    Graph *K5Subdivision = (Graph *)malloc(sizeof(Graph));
    int StoreVertex = 0;
    int numberOfVInSG1 = SGC_1->data;
    int numberOfVInSG2 = SGC_2->data;
    int numberOfCommonV = commonEV->listOfCommonV->data;
    int storedVertexhead[MAX], storedEdges[MAX][MAX];
    memset(storedVertexhead, -1, sizeof(storedVertexhead));
    memset(storedEdges, -1, sizeof(storedEdges));
    int WhereIsVertex;
    EdgeNode *addEdge;

    //initiate the graph
    K5Subdivision->numV = numberOfVInSG1 + numberOfVInSG2 - numberOfCommonV;
    K5Subdivision->numE = 0;
    LinkList p1 = SGC_1->next, p1Next = SGC_1->next->next;
    LinkList p2 = SGC_2->next, p2Next = SGC_2->next->next;

    //traverse SG(C1)
    while(p1Next != SGC_1)
    {
        if(storedVertexhead[p1->data] == -1) //if the vertex in SG(C1) hasn't been stored in the head of graph
        {
            storedVertexhead[p1->data] = StoreVertex; //where will the vertex be in the graph

            //add vertex to the graph
            K5Subdivision->ajList[StoreVertex].data = p1->data;
            K5Subdivision->ajList[StoreVertex].firstE = NULL;

            //add edge to the graph
            addEdge = (EdgeNode *)malloc(sizeof(EdgeNode));
            addEdge->AjV = p1Next->data;
            addEdge->next = K5Subdivision->ajList[StoreVertex].firstE;
            K5Subdivision->ajList[StoreVertex].firstE = addEdge;
            K5Subdivision->numE++;
            storedEdges[p1->data][p1Next->data] = 1;

            addEdge = NULL;
            StoreVertex++;
        }
        else if(storedEdges[p1->data][p1Next->data] == -1)
        {
            WhereIsVertex = storedVertexhead[p1->data];
            //add edge to the graph
            addEdge = (EdgeNode *)malloc(sizeof(EdgeNode));
            addEdge->AjV = p1Next->data;
            addEdge->next = K5Subdivision->ajList[WhereIsVertex].firstE;
            K5Subdivision->ajList[WhereIsVertex].firstE = addEdge;
            K5Subdivision->numE++;
            storedEdges[p1->data][p1Next->data] = 1;

        }
        addEdge = NULL;
        p1 = p1->next;
        p1Next = p1Next->next;
    }

    //the last edge in SG(C1)
    p1Next = p1Next->next;
    if(storedVertexhead[p1->data] == -1) //if the vertex in SG(C1) hasn't been stored in the head of graph
    {
            storedVertexhead[p1->data] = StoreVertex; //where will the vertex be in the graph

            //add vertex to the graph
            K5Subdivision->ajList[StoreVertex].data = p1->data;
            K5Subdivision->ajList[StoreVertex].firstE = NULL;

            //add edge to the graph
            addEdge = (EdgeNode *)malloc(sizeof(EdgeNode));
            addEdge->AjV = p1Next->data;
            addEdge->next = K5Subdivision->ajList[StoreVertex].firstE;
            K5Subdivision->ajList[StoreVertex].firstE = addEdge;
            K5Subdivision->numE++;
            storedEdges[p1->data][p1Next->data] = 1;

            addEdge = NULL;
            StoreVertex++;
    }
    else if(storedEdges[p1->data][p1Next->data] == -1)
    {
        WhereIsVertex = storedVertexhead[p1->data];
        //add edge to the graph
        addEdge = (EdgeNode *)malloc(sizeof(EdgeNode));
        addEdge->AjV = p1Next->data;
        addEdge->next = K5Subdivision->ajList[WhereIsVertex].firstE;
        K5Subdivision->ajList[WhereIsVertex].firstE = addEdge;
        K5Subdivision->numE++;
        //end traverse SG(C1)
        addEdge = NULL;
    }



    //traverse SG(C2)
    while(p2Next != SGC_2)
    {
        if(storedVertexhead[p2->data] == -1) //if the vertex in SG(C1) hasn't been stored in the head of graph
        {
            storedVertexhead[p2->data] = StoreVertex; //where will the vertex be in the graph

            //add vertex to the graph
            K5Subdivision->ajList[StoreVertex].data = p2->data;
            K5Subdivision->ajList[StoreVertex].firstE = NULL;

            //add edge to the graph
            addEdge = (EdgeNode *)malloc(sizeof(EdgeNode));
            addEdge->AjV = p2Next->data;
            addEdge->next = K5Subdivision->ajList[StoreVertex].firstE;
            K5Subdivision->ajList[StoreVertex].firstE = addEdge;
            K5Subdivision->numE++;
            storedEdges[p2->data][p2Next->data] = 1;

            addEdge = NULL;
            StoreVertex++;
        }
        else if(storedEdges[p2->data][p2Next->data] == -1)
        {
            WhereIsVertex = storedVertexhead[p2->data];
            //add edge to the graph
            EdgeNode *addEdge = (EdgeNode *)malloc(sizeof(EdgeNode));
            addEdge->AjV = p2Next->data;
            addEdge->next = K5Subdivision->ajList[WhereIsVertex].firstE;
            K5Subdivision->ajList[WhereIsVertex].firstE = addEdge;
            K5Subdivision->numE++;

            addEdge = NULL;
        }

        p2 = p2->next;
        p2Next = p2Next->next;
    }

    //the last edge in SG(C2)
    p2Next = p2Next->next;
    if(storedVertexhead[p2->data] == -1) //if the vertex in SG(C1) hasn't been stored in the head of graph
    {
            storedVertexhead[p2->data] = StoreVertex; //where will the vertex be in the graph

            //add vertex to the graph
            K5Subdivision->ajList[StoreVertex].data = p2->data;
            K5Subdivision->ajList[StoreVertex].firstE = NULL;

            //add edge to the graph
            addEdge = (EdgeNode *)malloc(sizeof(EdgeNode));
            addEdge->AjV = p2Next->data;
            addEdge->next = K5Subdivision->ajList[StoreVertex].firstE;
            K5Subdivision->ajList[StoreVertex].firstE = addEdge;
            K5Subdivision->numE++;
            storedEdges[p2->data][p2Next->data] = 1;

            addEdge = NULL;
            StoreVertex++;
    }
    else if(storedEdges[p2->data][p2Next->data] == -1)
    {
        WhereIsVertex = storedVertexhead[p2->data];
        //add edge to the graph
        addEdge = (EdgeNode *)malloc(sizeof(EdgeNode));
        addEdge->AjV = p2Next->data;
        addEdge->next = K5Subdivision->ajList[WhereIsVertex].firstE;
        K5Subdivision->ajList[WhereIsVertex].firstE = addEdge;
        K5Subdivision->numE++;
        //end traverse SG(C2)
        addEdge = NULL;
    }

    return K5Subdivision;

}

//********************************************************* Step 4 *******************************************************************
Graph *step4(CommonEV commonEV) //return NULL or K3,3
{
    if(commonEV->numberOfCommonE > 2 && commonEV->listOfCommonV->data > 5)
    {
        Matrix_M * M = step5(SGC_1, SGC_2); //go to step 5
        nonZeroRow_extra after_step6 = Step6(M);
        Step7_Extra* WhichRowHas3Entries = SixNZRow_in_step7(M, after_step6);
        if(WhichRowHas3Entries == NULL) return NULL;
        Graph *MaybeK33 = step8(WhichRowHas3Entries, M, commonEV, SGC_1, SGC_2);
        return MaybeK33;
    }
    return NULL;
}


//******************************************************* Step 5 *************************************************************
Matrix_M *step5(LinkList SGC_1, LinkList SGC_2)
{
    //matrix M stores undirected graph
    matrix_M Mmatrix = (matrix_M)malloc(sizeof(Matrix_M));

    //form adjacency matrix of SG(C1) and SG(C2)
    int storeSGC_1[MAX][MAX] = {0};
    int storeSGC_2[MAX][MAX] = {0};
    LinkList p1 = SGC_1->next;
    LinkList p1Next = SGC_1->next->next;
    LinkList p2 = SGC_2->next;
    LinkList p2Next = SGC_2->next->next;
    int maxVertex = -1;

    while(p1Next != SGC_1)
    {
        storeSGC_1[p1->data][p1Next->data] = 1;

        if(maxVertex < p1->data) {maxVertex = p1->data;}
        p1 = p1->next;
        p1Next = p1Next->next;
    }
    p1Next = p1Next->next;
    storeSGC_1[p1->data][p1Next->data] = 1;

    while(p2Next != SGC_2)
    {
        storeSGC_2[p2->data][p2Next->data] = 1;

        if(maxVertex < p2->data) {maxVertex = p2->data;}
        p2 = p2->next;
        p2Next = p2Next->next;
    }
    p2Next = p2Next->next;
    storeSGC_2[p2->data][p2Next->data] = 1;

    //form M
    memset(Mmatrix->pointToM, 0, sizeof(Mmatrix->pointToM));
    p1 = SGC_1->next;
    p1Next = SGC_1->next->next;
    p2 = SGC_2->next;
    p2Next = SGC_2->next->next;

    //traverse edges in SG(C1)
    while(p1Next != SGC_1)
    {
        //mij = 1 if{vi, vj} in C1-C2
        if(storeSGC_2[p1->data][p1Next->data] == 0)
        {
            Mmatrix->pointToM[p1->data][p1Next->data] = 1;
            Mmatrix->pointToM[p1Next->data][p1->data] = 1;
        }
        //mij = 3 if {vi, vj} in C1 intersect C2
        if(storeSGC_2[p1->data][p1Next->data])
        {
            Mmatrix->pointToM[p1->data][p1Next->data] = 3;
            Mmatrix->pointToM[p1Next->data][p1->data] = 3;
        }
        p1 = p1->next;
        p1Next = p1Next->next;
    }
    p1Next = p1Next->next;
    //mij = 1 if{vi, vj} in C1-C2
    if(!storeSGC_2[p1->data][p1Next->data])
    {
            Mmatrix->pointToM[p1->data][p1Next->data] = 1;
            Mmatrix->pointToM[p1Next->data][p1->data] = 1;
    }
    //mij = 3 if {vi, vj} in C1 intersect C2
    if(storeSGC_2[p1->data][p1Next->data])
    {
            Mmatrix->pointToM[p1->data][p1Next->data] = 3;
            Mmatrix->pointToM[p1Next->data][p1->data] = 3;
    }

    //traverse edges in SG(C2)
    while(p2Next != SGC_2)
    {
        if(!storeSGC_1[p2->data][p2Next->data])
        {
            Mmatrix->pointToM[p2->data][p2Next->data] = 2;
            Mmatrix->pointToM[p2Next->data][p2->data] = 2;
        }

        p2 = p2->next;
        p2Next = p2Next->next;
    }
    p2Next = p2Next->next;
    if(!storeSGC_1[p2->data][p2Next->data])
    {
            Mmatrix->pointToM[p2->data][p2Next->data] = 2;
            Mmatrix->pointToM[p2Next->data][p2->data] = 2;
    }

    Mmatrix->MaxVertex = maxVertex;
    return Mmatrix;
}
void printMmatrix(matrix_M M) //for debugging
{
    printf("M matrix:\n");
    printf("  ");
    for(int i = 0; i<=M->MaxVertex; i++)
    {
        printf("%d ", i);
    }
    printf("\n");

    for(int i = 0; i<=M->MaxVertex; i++)
    {
        printf("%d ", i);
        for(int j = 0; j<=M->MaxVertex; j++)
        {
            printf("%d ", M->pointToM[i][j]);
        }
        printf("\n");
    }
}



//******************************************************************** step 6 ****************************************************************

nonZeroRow_extra Judge_step6(Matrix_M * Mmatrix, nonZeroRow_extra NonZeroRows) // set NoneZeroRows = int* [6] outside the function!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
{

    //for step 7:
    int numberOfNoneZeroRows = 0, counted = 0;
    memset(NonZeroRows->RowZeroOrNot, 0, sizeof(NonZeroRows->RowZeroOrNot));//RowZeroOrNot[which row] = number of non-zero entries in the row

    for(int row = 0; row<=Mmatrix->MaxVertex; row++)
    {
        int i=-1, j=-1, k=-1, mki = -1, valid = 0;
        for(int colume = 0; colume<=Mmatrix->MaxVertex; colume++)
        {
            if(Mmatrix->pointToM[row][colume])
            {
                if(!counted)
                {
                    numberOfNoneZeroRows++;
                    counted = 1;
                }

                NonZeroRows->RowZeroOrNot[row]++;

                //if there are exactly two non-zero entries, mki = mkj or mki != mkj
                if(i == -1) {i = colume; mki = Mmatrix->pointToM[row][colume];}
                else if(i!= -1 && Mmatrix->pointToM[row][colume] == mki) {j = colume; valid = 1; k = row;}
                else if(i != -1 && Mmatrix->pointToM[row][colume] != mki)
                {
                    valid = 0;
                    continue;
                }
                else if(i!= -1 && j != -1) //there are more than two nonzero entries in the row, jump to next row
                {
                    valid = 0;
                    continue;
                }
            }
        }

        counted = 0;
        if(valid)
        {
            numberOfNoneZeroRows--;

            Mmatrix->pointToM[i][j]+= Mmatrix->pointToM[k][i];
            Mmatrix->pointToM[j][i]+= Mmatrix->pointToM[k][i];
            Mmatrix->pointToM[k][i] = Mmatrix->pointToM[i][k] = Mmatrix->pointToM[k][j] = Mmatrix->pointToM[j][k] = 0;

            //i!= k &&  j!= k because self-loop has been excluded by tarjan
            NonZeroRows->RowZeroOrNot[k] = NonZeroRows->RowZeroOrNot[k] -2;

        }
    }
    NonZeroRows->numberOfnonZeroRows = numberOfNoneZeroRows;
    return NonZeroRows;
}

nonZeroRow_extra Step6(Matrix_M * Mmatrix)
{
    nonZeroRow_extra NonZeroRows = (nonZeroRow_extra )malloc(sizeof(NonZeroRow_Extra));
    NonZeroRows = Judge_step6(Mmatrix, NonZeroRows);
    return NonZeroRows;
}

void printNonZeroRows(nonZeroRow_extra NonZeroRows, Matrix_M * Mmatrix)
{
    printf("%d\n", NonZeroRows->numberOfnonZeroRows);
    printf("Row:          ");
    for(int i = 0; i<=Mmatrix->MaxVertex; i++)
    {
        printf("%d ", i);
    }
    printf("\n");
    printf("NonZeroEntries");
    for(int j = 0; j<= Mmatrix->MaxVertex; j++)
    {
        printf("%d ", NonZeroRows->RowZeroOrNot[j]);
    }
    printf("\n");
}

//******************************************************************* step 7 *****************************************************************

Step7_Extra *SixNZRow_in_step7(Matrix_M * Mmatrix, nonZeroRow_extra nonZeroRowList)
{
    step7_Extra WhichRowHas3Entries = (step7_Extra)malloc(sizeof(Step7_Extra));
    int j = 0;
    memset(WhichRowHas3Entries->NonZeroRows, 0, sizeof(WhichRowHas3Entries->NonZeroRows));

    if(nonZeroRowList->numberOfnonZeroRows == 6)
    {
        for(int i=0; i<=Mmatrix->MaxVertex; i++)
        {
            if(nonZeroRowList->RowZeroOrNot[i] != 3 && nonZeroRowList->RowZeroOrNot[i] != 0)
            {
                //go to step 9
                return NULL;
            }
            else if(nonZeroRowList->RowZeroOrNot[i] == 3) {WhichRowHas3Entries->NonZeroRows[j++] = i;}
        }
        //go to step 8
        return WhichRowHas3Entries;
    }
    //step 9
    return NULL;
}

void printStep7_Three_Entries_Row(step7_Extra WhichRowHas3Entries) //for debugging
{
    if(!WhichRowHas3Entries)
    {
        printf("Fail to print the list of six rows with three non-zero entries. Go to step 9.\n");
        return;
    }
    printf("The row with three None Zero entries: \n");
    for(int i = 0; i<=5; i++)
    {
        printf("%d ", WhichRowHas3Entries->NonZeroRows[i]);
    }
    printf("\n");

}


//******************************************************************* step 8 *****************************************************************

Graph *step8(Step7_Extra *WhichRowHas3Entries, Matrix_M * Mmatrix, CommonEV commonEV, LinkList SGC_1, LinkList SGC_2)
{
    if(WhichRowHas3Entries == NULL) return NULL;

    int *whichNonZeroRow = WhichRowHas3Entries->NonZeroRows;
    int row, column;

    int FirstTraverseIndex = 1,BreakOrNot = 0;
    int firstTraverse[7] = {-1}, addOnlyOnce[MAX] = { [0 ... MAX-1] = 1};//addOnlyOnce check which index has been added to the firstTraver[7]

    //traverse {vi1, vi2, ..., vi6}
    //find vi1, vi2 where (vi1, vi2) = 3
    for(int i = 0; i<=4; i++)
    {
        if(BreakOrNot) break;
        row = whichNonZeroRow[i];
        for(int j = i; j<=5; j++)
        {
            if(BreakOrNot){break;}
            column = whichNonZeroRow[j];
            if(Mmatrix->pointToM[row][column] == 3)
            {
                firstTraverse[FirstTraverseIndex] = row;
                firstTraverse[++FirstTraverseIndex] = column;

                addOnlyOnce[row] = addOnlyOnce[column] = 0;
                BreakOrNot = 1;
            }
        }
    }
    while(FirstTraverseIndex < 6)
    {
        for(int i = 0; i<=5; i++)
        {
            int NewColumn = whichNonZeroRow[i];
            if(Mmatrix->pointToM[column][NewColumn] == 1 && addOnlyOnce[NewColumn] == 1)
            {
                firstTraverse[++FirstTraverseIndex] = NewColumn;
                addOnlyOnce[NewColumn] = 0;
                column = NewColumn;
                break;
            }
        }
        if(FirstTraverseIndex == 6) break;

        for(int i = 0; i<=5; i++)
        {
            int NewColumn = whichNonZeroRow[i];
            if(Mmatrix->pointToM[column][NewColumn] == 3 && addOnlyOnce[NewColumn] == 1)
            {
                firstTraverse[++FirstTraverseIndex] = NewColumn;
                addOnlyOnce[NewColumn] = 0;
                column = NewColumn;
                continue;
            }
        }
     }
    //check if can be traversed following {vi1, vi2, vi5, vi6, vi3, vi4}
    int secondTraverse[7] = {-1, firstTraverse[1], firstTraverse[2], firstTraverse[5], firstTraverse[6], firstTraverse[3], firstTraverse[4]};
    int TwoOrThree = 0; //TwoOrThree = 0 <=> the weight of edge should be two, and vice versa
    for( int i = 0; i<= 3; i++)
    {
        int row = secondTraverse[i+2], column = secondTraverse[i+3];
        if(!TwoOrThree)
        {
            if(Mmatrix->pointToM[row][column] != 2)
            {
                //step 9
                return NULL;
            }
        }
        else if(TwoOrThree == 1 && Mmatrix->pointToM[row][column] != 3)
        {
                //step 9
                return NULL;
        }

        if(!TwoOrThree) TwoOrThree = 1;
        else if(TwoOrThree) TwoOrThree = 0;
    }
    if(Mmatrix->pointToM[firstTraverse[1]][firstTraverse[4]] != 2)
    {
        //step 9
        return NULL;
    }

    //output SG(C1 U C3)
    Graph *K3Subdivision = SG_C1_U_C2(commonEV, SGC_1, SGC_2);
    K3OrNot = 1;
    return K3Subdivision;
}


//***************************************************** step 9 ************************************************************************************
//pack the functions up


void Kuratowski(void)
{
    int DirectOrNot;
    printf("What kind of graph do you want to test? Please type 0 or 1 only.\n");
    printf("0: Directed graph.   1: Undirected graph.\n");
    scanf("%d", &DirectOrNot);

  if(DirectOrNot)
  {
    Stack stack1 = CreateStack();
    Graph *g;
    Graph *B;
    Stack stack2;
    GraphList *SCCList;
    int K3OrNot=0, K5OrNot=0;
    cmp component;

    g = (Graph *)malloc(sizeof(Graph)); //malloc graph cannot be placed inside the CreateDirGraph function otherwise the memory is released after the function being excuted
    CreateDirGraph(g); //initiate directed graph, vertex starts from 0
    component = (cmp)malloc(sizeof(Component));
    findSCC(g, stack1, 0, component);

    SCCList = Extract_Vertex_induced_subgraph(component, g);

    //Johnson - the Cycle Generator
    SAS* VertexSorted = BucketSort(g); //sort the vertexes, generate two arrays mapping original vertex to vertexes start from 1, then 2, 3, ... or backward
    Graph *g2 = VertexStartFromOne(g, VertexSorted); //convert the vertexes in graph to vertexes start from 1
    Cycle *cycles = JohnsonFindCycles(g2, stack2, B); //generate cycles by Johnson algorithm

    kuratowski_subgraphs K5OrK33 = Step2_CompareComponent(cycles);
    if(K5OrK33 == NULL) {printf("Planar graph.\n");}
    else if(K5OrK33->IfK33) {printf("The graph contains a subgraph which is a subdivision to K3,3.\nThe subgraph is \n"); printGraph(K5OrK33->K33);}
    else if(K5OrK33->IfK5) {printf("The graph contains a subgraph which is a subdivision to K5.\nThe subgraph is\n"); printGraph(K5OrK33->K5);}
  }
  else
  {
    //input graph-------------
    Stack stack1 = CreateStack();
    Graph *Directed_g;
    Graph *B;
    Stack stack2;
    GraphList *SCCList;
    int K3OrNot=0, K5OrNot=0;
    cmp component;

    Directed_g = (Graph *)malloc(sizeof(Graph)); //malloc graph cannot be placed inside the CreateDirGraph function otherwise the memory is released after the function being excuted
    CreateDirGraph(Directed_g); //initiate directed graph, vertex inputed must start from 0

    //directed to undirected----------------
    Graph *g = DirectToUndirect(Directed_g);

    //K_subgraph for undirected----------------
    component = (cmp)malloc(sizeof(Component));
    findSCC(g, stack1, 0, component);

    //Johnson - the Cycle Generator
    SAS* VertexSorted = BucketSort(g); //sort the vertexes, generate two arrays mapping original vertex to vertexes start from 1, then 2, 3, ... or backward
    Graph *g2 = VertexStartFromOne(g, VertexSorted); //convert the vertexes in graph to vertexes start from 1

    Cycle *cycles = JohnsonFindCycles(g2, stack2, B); //generate cycles by Johnson algorithm

    kuratowski_subgraphs K5OrK33 = Step2_CompareComponent(cycles);

    //Vertex for K_subgraph + vertex induced subgraph----------------
    if(K5OrK33 == NULL) {printf("Planar graph.\n");}
    else if(K5OrK33->IfK33)
    {
        CommonEV Vertex_Of_KSubgraph = Vertex_From_Graph(K5OrK33->K33);
        Graph* K33 = Extract_Vertex_induced_K_subgraph_DirectedGraph(Vertex_Of_KSubgraph, Directed_g);
        printf("The graph contains a subgraph which is a subdivision to K3,3.\nThe subgraph is \n");
        printGraph(K33);
    }
    else if(K5OrK33->IfK5)
    {
        CommonEV Vertex_Of_KSubgraph = Vertex_From_Graph(K5OrK33->K5);
        Graph* K5 = Extract_Vertex_induced_K_subgraph_DirectedGraph(Vertex_Of_KSubgraph, Directed_g);
        printf("The graph contains a subgraph which is a subdivision to K5.\nThe subgraph is \n");
        printGraph(K5);
    }
  }
}
