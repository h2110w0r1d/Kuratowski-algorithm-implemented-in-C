#ifndef LINKLIST_H_INCLUDED
#define LINKLIST_H_INCLUDED

typedef int ElemType;

typedef struct LinkListNode
{
    ElemType data;
    struct LinkListNode *next;
}LinkListNode, *LinkList;

LinkList CreateLinkList(void); //create a linklist
LinkList InitLinkList(ElemType n, LinkList head); //insert a node at the head of the linklist, given that the linklist exists
int LinkListLength(LinkList head); //return the number of nodes within the link list
LinkList scanfLinkList(LinkList head); //create and input data into a LinkList



#endif // LINKLIST_H_INCLUDED
