#include <iostream>

using namespace std;

#define MaxSize 1000
#define nullP -1

typedef char VertexType;

typedef struct EdgeNode
{
    int iVex;
    int jVex;
    struct EdgeNode* iLink;
    struct EdgeNode* jLink;
} EdgeNode;

typedef struct VertexNode
{
    VertexType data;
    EdgeNode* firstEdge;
} VertexNode;

typedef struct AMLGraph
{
    VertexNode list[MaxSize];
    int vexnum, edgenum;
} AMLGraph;

int LocateVex(AMLGraph G, VertexType v)
{
    for (int i = 0; i < G.vexnum; i++)
    {
        if (G.list[i].data == v)
            return i;
    }

    return nullP;
}

void InsertEdge(AMLGraph& G, VertexType v, VertexType w)
{
    int vPos = LocateVex(G, v);
    int wPos = LocateVex(G, w);

    if (vPos == nullP || wPos == nullP)
        return;

    EdgeNode* temp = new EdgeNode();
    temp->iVex = vPos;
    temp->jVex = wPos;
    temp->iLink = nullptr;
    temp->jLink = nullptr;

    if (G.list[vPos].firstEdge == nullptr)
        G.list[vPos].firstEdge = temp;
    else
    {
        EdgeNode* p = G.list[vPos].firstEdge;
        while (p->iLink != nullptr)
            p = p->iLink;
        p->iLink = temp;
    }

    if (G.list[wPos].firstEdge == nullptr)
        G.list[wPos].firstEdge = temp;
    else
    {
        EdgeNode* p = G.list[wPos].firstEdge;
        while (p->jLink != nullptr)
            p = p->jLink;
        p->jLink = temp;
    }

    G.edgenum++;
}
