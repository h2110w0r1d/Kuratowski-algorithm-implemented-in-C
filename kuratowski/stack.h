#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED


typedef int ElemType;

typedef struct Node
{
  ElemType data;
  struct Node *next;
} Node, *Stack, *Location;

Stack CreateStack(void);
int isEmpty(Stack K);
void push(ElemType data, Stack K );
ElemType top(Stack K);
ElemType pop(Stack K);
void makeEmpty(Stack K);
void printStack(Stack K);
void DeleteInStack(Stack top, int deleteMe); //delete the element 'deleteMe' in a stack


#endif // STACK_H_INCLUDED
