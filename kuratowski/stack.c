#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"stack.h"

Stack CreateStack( void )
{
    Stack K;
    K = (Stack)malloc(sizeof(Node));
    if(NULL == K)
    {
        printf("Fail to create the stack.\n");
        exit(1);
    }
    K->next = NULL;
    return K;
}

int isEmpty(Stack K)
{
    int a = (K->next == NULL);
    return a;
}

void push(ElemType data, Stack K ) //K always points toward the top node, K itself stores nothing
{
    Stack current = NULL;
    current = (Stack)malloc(sizeof(Node));
    if(current == NULL)
    {
      printf("stack invalid.\n");
      exit(1);
    }
    current->next = K->next;
    current->data = data;
    K->next = current;
}

ElemType top(Stack K)
{
    if(isEmpty(K))
    {
        printf("Null Stack.\n");
        exit(1);
    }
    else{
        return K->next->data;
    }
}

ElemType pop(Stack K)
{
    if(isEmpty(K))
    {
        printf("Empty stack.\n");
        exit(1);
    }
    Stack temp = K->next;
    K->next = temp->next;
    int k = temp->data;
    free(temp);
    return k;

}

void makeEmpty(Stack K)
{
    while(!isEmpty(K))
    {
        pop(K);
    }
}

void printStack(Stack K)
{
    if(!isEmpty(K))
    {
      Location temp = K->next;
      printf("\n");
      while(temp)
      {
        printf("%d", temp->data);
        temp = temp->next;
      }
    }
    else
    {
        printf("Null stack.\n");
        exit(1);
    }
    printf("\n");
}

void DeleteInStack(Stack top, int deleteMe) //delete the element 'deleteMe' in a stack
{
    Stack p = top->next;
    Stack previous = top;
    if(p == NULL)
    {
        printf("Nothing in the stack to be deleted.\n");
        return;
    }
    while(p)
    {
        if(p->data == deleteMe)
        {
            Stack DeleteMe = p;
            p = p->next;
            previous->next = p;
            free(DeleteMe);
            DeleteMe = NULL;
            continue;
        }
        p=p->next;
        previous = previous->next;

    }
}






