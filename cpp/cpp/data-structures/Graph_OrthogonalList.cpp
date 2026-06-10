#include <iostream>
using namespace std;

#define MaxSize 1000
#define nullP -1

typedef char VertexType;

typedef struct ArcBox{
    int tailVex;
    int headVex;
    struct ArcBox *hlink;
    struct ArcBox *tlink;
} ArcBox;

typedef struct VexNode{
    VertexType data;
    ArcBox *firstIn;
    ArcBox *firstOut;
} VexNode;

typedef struct{
    VexNode list[MaxSize];
    int vexnum, arcnum;
} OLGraph;

typedef struct{
    VertexType v;
    VertexType w;
} Arc;

int LocationVex(OLGraph G, VertexType v)
{
    for(int i = 0; i < G.vexnum; i++)
    {
        if(G.list[i].data == v) return i;
    }

    return nullP;
}

void InsertArc(OLGraph &G, VertexType v, VertexType w)
{
    int tail = LocationVex(G, v);
    int head = LocationVex(G, w);

    if(tail == nullP || head == nullP)  return;
    
    ArcBox *temp = new ArcBox();


    temp->tailVex = tail;
    temp->headVex = head;
    temp->hlink = nullptr;
    temp->tlink = nullptr;

    if(G.list[tail].firstOut == nullptr)    G.list[tail].firstOut = temp;
    else 
    {
        ArcBox *p = G.list[tail].firstOut;
        while(p->tlink != nullptr)  p = p->tlink;
        p->tlink = temp;
    }

    if(G.list[head].firstIn == nullptr)     G.list[head].firstIn = temp; 
    else 
    {
        ArcBox *p = G.list[head].firstIn;
        while(p->hlink != nullptr)  p = p->hlink;
        p->hlink = temp;
    }

    G.arcnum++;
}

void MakeGraph(OLGraph &G, VertexType v[], int vexnum, Arc vr[], int arcnum)
{
    G.vexnum = vexnum;
    G.arcnum = 0;
    
    for(int i = 0; i < G.vexnum; i++)
    {
        G.list[i].data = v[i];
        G.list[i].firstIn = nullptr;
        G.list[i].firstOut = nullptr;
    }

    for(int i = 0; i < arcnum; i++)
    {
        InsertArc(G, vr[i].v, vr[i].w);
    }

}
