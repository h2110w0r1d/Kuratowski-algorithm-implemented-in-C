#include "stdio.h"
#include "LinkList.h"


LinkList CreateLinkList(void) //create a linklist
{
    LinkList head;
    head = (LinkList)malloc(sizeof(LinkListNode));
    head->next = head;
    return head;
}

LinkList InitLinkList(ElemType n, LinkList head) //insert a node at the head of the linklist, given that the linklist exists (tail has pointed to head)
{
    LinkList temp;
    temp = (LinkList)malloc(sizeof(LinkListNode));
    temp->data = n;
    temp->next = head->next;
    head->next = temp;
    return head;
}

LinkList scanfLinkList(LinkList head) //create and insert data into the tail of a LinkList
{
    head = CreateLinkList();
    LinkList tail = head;
    ElemType n, total;
    LinkList temp;
    printf("how many element do you want to put in?\n");
    scanf("%d", &total);

    head->data = total;
    while(total--)
    {
        scanf("%d", &n);
        temp = (LinkList)malloc(sizeof(LinkListNode));
        temp->data = n;
        temp->next = tail->next;
        tail->next = temp;
        tail = temp;
        temp = NULL;
    }

    return head;
}

int LinkListLength(LinkList head) //return the number of nodes within the link list
{
    int count = 0;
    LinkList tmp;
    tmp = head->next;
    while(tmp != head)
    {
        count++;
        tmp = tmp->next;
    }
    return count;
}

void printLinkList(LinkList head)
{
    printf("\n");
    LinkList p;
    p = head->next;
    if(!p)
    {
      printf("nothing in the LinkList.\n");
      exit(1);
    }
    while(p!= head)
    {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("\n");

}

void TraceLinkList(LinkList head, int *a)  //traverse the LinkList and store the vertex in an array, edge a--b  <=> array[a]=b
{
    a = (int *)malloc(200 * sizeof(int)); //a[200]
    memset(a, -1, 200);
    LinkList p;
    p = head->next;
    while(p ->next != head)
    {
       a[p->data] = p->next->data;
       p = p->next;
    }
    if(p->next = head) //the circular linked list points from to head with head uninitialized
    {
        a[p->data]=head->next->data;
    }
}

int LinkListGet (LinkList head, int index) //input the index of a vertex and return if the vertex is in or not
{
    LinkList traverse;
    traverse = head->next;
    while(traverse != head)
    {
        if(index == traverse->data)
        {
            return 1;
        }
        traverse = traverse->next;
    }
    return 0;
}

//remove all the nodes with data = ... given the data is in the link list
void LinkListDelete(LinkList head, int value)
{
    LinkList p1 = head->next->next;
    LinkList p3 = p1;
    LinkList p2 = head->next;

    while(p1)
    {
        if(p1->data == value)
        {
           p1 = p1->next;
           p2 ->next = p1;

           free(p3);

           p3 = p1;
        }
    }
}





