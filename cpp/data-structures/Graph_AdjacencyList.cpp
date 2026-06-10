#include <iostream>
using namespace std;

#define MaxSize 1000
#define nullP -1

typedef char VertexType;
typedef int ElemType;
typedef struct ArcNode
{
    int adjvex;
    struct ArcNode* next;
} ArcNode;

typedef struct VertexNode
{
    VertexType data;
    ArcNode* firstArc;
} VertexNode;

typedef struct
{
    VertexNode list[MaxSize];
    int vexnum, arcnum;
    bool directed;
} ALGraph;

int LocateVex(ALGraph G, VertexType v)
{
    for (int i = 0; i < G.vexnum; i++)
    {
        if (G.list[i].data == v)
            return i;
    }

    return nullP;
}

void InitGraph(ALGraph& G, bool directed)
{
    G.vexnum = 0;
    G.arcnum = 0;
    G.directed = directed;
    for (int i = 0; i < G.vexnum; i++)
    {
        G.list[i].firstArc = nullptr;
    }
}

bool IsExistArc(ALGraph& G, int tail, int head)
{
    ArcNode* p = G.list[tail].firstArc;
    while (p != nullptr)
    {
        if (p->adjvex == head)
            return true;
        p = p->next;
    }

    return false;
}

bool AddArc(ALGraph& G, int tail, int head)
{
    if (IsExistArc(G, tail, head))
        return false;
    ArcNode* temp = new ArcNode();
    temp->adjvex = head;
    temp->next = nullptr;

    if (G.list[tail].firstArc == nullptr)
        G.list[tail].firstArc = temp;
    else
    {
        ArcNode* p = G.list[tail].firstArc;
        while (p->next != nullptr)
            p = p->next;
        p->next = temp;
    }

    return true;
}

void InsertArc(ALGraph& G, VertexType v, VertexType w)
{
    int vPos = LocateVex(G, v);
    int wPos = LocateVex(G, w);

    if (vPos == nullP || wPos == nullP)
        return;

    if (!AddArc(G, vPos, wPos))
        return;

    if (!G.directed)
        AddArc(G, wPos, vPos);

    G.arcnum++;
}

bool visited[MaxSize];

void InitVisited(ALGraph G)
{
    for (int i = 0; i < G.vexnum; i++)
        visited[i] = false;
}

void VisitVex(ALGraph G, int vPos)
{
    cout << G.list[vPos].data << " ";
}

void DFS(ALGraph G, int vPos)
{
    VisitVex(G, vPos);
    visited[vPos] = false;

    ArcNode* p = G.list[vPos].firstArc;
    while (p != nullptr)
    {
        if (!visited[p->adjvex])
            DFS(G, p->adjvex);

        p = p->next;
    }
}

void DFSTraverse(ALGraph G)
{
    InitVisited(G);
    for (int i = 0; i < G.vexnum; i++)
    {
        if (!visited[i])
            DFS(G, i);
    }
}

typedef struct
{
    ElemType data[MaxSize];
    int front, rear;
} Queue;

void InitQueue(Queue& Q)
{
    Q.rear = Q.front = 0;
}

bool QueueEmpty(Queue Q)
{
    if (Q.rear == Q.front)
        return true;
    else
        return false;
}

bool EnQueue(Queue& Q, ElemType x)
{
    if ((Q.rear + 1) % MaxSize == Q.front)
        return false;

    Q.data[Q.rear] = x;
    Q.rear = (Q.rear + 1) % MaxSize;

    return true;
}

bool DeQueue(Queue& Q, ElemType& x)
{
    if (QueueEmpty(Q))
        return false;
    x = Q.data[Q.front];
    Q.front = (Q.front + 1) % MaxSize;

    return true;
}

void BFS(ALGraph G, int vPos)
{
    VisitVex(G, vPos);
    visited[vPos] = true;
    Queue Q;
    InitQueue(Q);
    EnQueue(Q, vPos);
    while (!QueueEmpty(Q))
    {
        int tempPos;
        DeQueue(Q, tempPos);

        ArcNode* p = G.list[tempPos].firstArc;
        while (p != nullptr)
        {
            if (!visited[p->adjvex])
            {

                VisitVex(G, p->adjvex);
                visited[p->adjvex] = true;
                EnQueue(Q, p->adjvex);
            }

            p = p->next;
        }
    }
}

void BFSTraverse(ALGraph G)
{
    InitVisited(G);
    for (int i = 0; i < G.vexnum; i++)
    {
        if (!visited[i])
            BFS(G, i);
    }
}

int dict[MaxSize];
int path[MaxSize];

void GetMinDistance(ALGraph G, int startPos)
{
    for (int i = 0; i < G.vexnum; i++)
    {
        visited[i] = false;
        dict[i] = 0;
        path[i] = nullP;
    }

    Queue Q;
    InitQueue(Q);
    EnQueue(Q, startPos);
    visited[startPos] = true;

    while (!QueueEmpty(Q))
    {
        int tempPos;
        DeQueue(Q, tempPos);
        ArcNode* p = G.list[tempPos].firstArc;
        while (p != nullptr)
        {
            if (!visited[p->adjvex])
            {
                visited[p->adjvex] = true;
                dict[p->adjvex] = dict[tempPos] + 1;
                path[p->adjvex] = tempPos;
                EnQueue(Q, p->adjvex);
            }
        }
    }
}
