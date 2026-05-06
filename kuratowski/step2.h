#ifndef STEP2_H_INCLUDED
#define STEP2_H_INCLUDED
static int K3OrNot=0; //K3OrNot = 1 <=> the graph contains a subgraph whose subdivision is K3,3
//************************************** step 5 *******************************
typedef struct Matrix_M
{
    int pointToM[MAX][MAX];
    int MaxVertex;
} Matrix_M, *matrix_M;

//**************************************** step 6 || step 7 ************************
typedef struct NonZeroRow_Extra
{
    int numberOfnonZeroRows;
    int RowZeroOrNot[MAX]; //RowZeroOrNot[which row] = number of non-zero entries in the row
} NonZeroRow_Extra, *nonZeroRow_extra;

typedef struct Step7_Extra
{
    int NonZeroRows[5]; //stores the sixes rows with three non-zero entries,
}Step7_Extra, *step7_Extra;

//***************************************** output the kuratowski subgraphs *****************
typedef struct K_subgraphs
{
    int IfK33;
    int IfK5;
    Graph *K33;
    Graph *K5;
} K_subgraphs, *kuratowski_subgraphs;

//Step 2***************************************************
CommonEV CommonVertex(LinkList head1, LinkList head2); //follows TraceLinkList to output the common vertex of two circuits
void printCommonEdgeVertex(CommonEV commonEV);
//compare each pair of components
Cycle *generateCycles(void);
kuratowski_subgraphs Step2_CompareComponent (Cycle *cycles);


//step 3*****************************************************
Graph *S3TraseCommonV( CommonEV commonEV, LinkList SGC_1, LinkList SGC_2);
Graph *SG_C1_U_C2(CommonEV commonEV, LinkList SGC_1, LinkList SGC_2);

//step 4****************************************************
Graph *step4(CommonEV commonEV);

//STEP 5 **************************************************
Matrix_M *step5(LinkList SGC_1, LinkList SGC_2);
void printMmatrix(matrix_M M);

//step 6 ***************************************************
nonZeroRow_extra Step6(Matrix_M * Mmatrix);
void printNonZeroRows(nonZeroRow_extra NonZeroRows, Matrix_M * Mmatrix);

//step 7 *****************************************************
Step7_Extra *SixNZRow_in_step7(Matrix_M * Mmatrix, nonZeroRow_extra nonZeroRowList);
void printStep7_Three_Entries_Row(step7_Extra WhichRowHas3Entries);

//step 8********************************************************
Graph *step8(Step7_Extra *WhichRowHas3Entries, Matrix_M * Mmatrix, CommonEV commonEV, LinkList SGC_1, LinkList SGC_2);

//step 9**********************************************************
void Kuratowski(void);


#endif // STEP2_H_INCLUDED
