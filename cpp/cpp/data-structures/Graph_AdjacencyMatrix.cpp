#include <iostream>
using namespace std;

#define MaxSize 1000
#define nullP -1

typedef char VertexType;
typedef int EdgeType;
typedef int ElemType;

typedef struct
{
    VertexType vertex[MaxSize];
    EdgeType edge[MaxSize][MaxSize];
    int vexnum, arcnum;
    bool directed;
} Graph;

typedef struct
{
    VertexType v;
    VertexType w;
} Arc;

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

int LocateVex(Graph G, VertexType v)
{
    for (int i = 0; i < G.vexnum; i++)
    {
        if (G.vertex[i] == v)
        {
            return i;
        }
    }

    return nullP;
}

void MakeGraph(Graph& G, VertexType v[], int vexnum, Arc vr[], int arcnum, bool directed)
{
    G.vexnum = vexnum;
    G.arcnum = 0;
    G.directed = directed;
    for (int i = 0; i < vexnum; i++)
    {
        G.vertex[i] = v[i];
    }

    for (int i = 0; i < vexnum; i++)
    {
        for (int j = 0; j < vexnum; j++)
        {
            G.edge[i][j] = 0;
        }
    }

    for (int k = 0; k < arcnum; k++)
    {
        int i = LocateVex(G, vr[k].v);
        int j = LocateVex(G, vr[k].w);
        if (i != nullP && j != nullP && G.edge[i][j] == 0)
        {
            G.edge[i][j] = 1;
            if (!G.directed)
                G.edge[j][i] = 1;
            G.arcnum++;
        }
    }
}

void FreeGraph(Graph& G)
{
    G.vexnum = 0;
    G.arcnum = 0;
}

VertexType GetVex(Graph G, VertexType v)
{
    int pos = LocateVex(G, v);
    if (pos == nullP)
        return '\0';
    return G.vertex[pos];
}

bool PutVex(Graph& G, VertexType v, VertexType value)
{
    int pos = LocateVex(G, v);
    if (pos == nullP)
        return false;
    G.vertex[pos] = value;
    return true;
}

int FirstAdjvex(Graph G, VertexType v)
{
    int pos = LocateVex(G, v);
    if (pos == nullP)
        return nullP;

    for (int i = 0; i < G.vexnum; i++)
    {
        if (G.edge[pos][i] != 0)
            return i;
    }

    return nullP;
}

int NextAdjvex(Graph G, VertexType v, VertexType w)
{
    int vPos = LocateVex(G, v);
    int wPos = LocateVex(G, w);

    if (vPos == nullP || wPos == nullP)
        return nullP;

    for (int i = wPos + 1; i < G.vexnum; i++)
    {
        if (G.edge[vPos][i] != 0)
            return i;
    }

    return nullP;
}

void DeleteVex(Graph& G, VertexType v)
{
    int pos = LocateVex(G, v);
    if (pos == nullP)
        return;

    int deleteArcnum = 0;

    if (G.directed)
    {
        for (int i = 0; i < G.vexnum; i++)
        {
            if (G.edge[pos][i] != 0)
                deleteArcnum++;
            if (i != pos && G.edge[i][pos] != 0)
                deleteArcnum++;
        }
    }
    else
    {
        for (int i = 0; i < G.vexnum; i++)
        {
            if (G.edge[pos][i] != 0)
                deleteArcnum++;
        }
    }

    G.arcnum -= deleteArcnum;

    for (int i = pos; i < G.vexnum - 1; i++)
    {
        G.vertex[i] = G.vertex[i + 1];
    }

    for (int i = pos; i < G.vexnum - 1; i++)
    {
        for (int j = 0; j < G.vexnum; j++)
        {
            G.edge[i][j] = G.edge[i + 1][j];
        }
    }

    for (int i = 0; i < G.vexnum - 1; i++)
    {
        for (int j = pos; j < G.vexnum - 1; j++)
        {
            G.edge[i][j] = G.edge[i][j + 1];
        }
    }

    G.vexnum--;

    return;
}

bool InsertArc(Graph& G, VertexType v, VertexType w)
{
    int vPos = LocateVex(G, v);
    int wPos = LocateVex(G, w);

    if (vPos == nullP || wPos == nullP)
        return false;
    if (G.edge[vPos][wPos] == 1)
        return false;
    if (G.edge[vPos][wPos] == 0)
    {
        G.edge[vPos][wPos] = 1;
        if (!G.directed)
        {
            G.edge[wPos][vPos] = 1;
        }

        G.arcnum++;
    }

    return true;
}

bool DeleteArc(Graph& G, VertexType v, VertexType w)
{
    int vPos = LocateVex(G, v);
    int wPos = LocateVex(G, w);

    if (vPos == nullP || wPos == nullP)
        return false;
    if (G.edge[vPos][wPos] == 0)
        return false;

    if (G.edge[vPos][wPos] == 1)
    {
        G.edge[vPos][wPos] = 0;
        if (!G.directed)
        {
            G.edge[wPos][vPos] = 0;
        }

        G.arcnum--;
    }

    return true;
}

bool visited[MaxSize];

void InitVisited(Graph G)
{
    for (int i = 0; i < G.vexnum; i++)
        visited[i] = false;
}

void VisitVex(Graph G, int vPos)
{
    cout << G.vertex[vPos] << " ";
}

void DFS(Graph G, int vPos)
{
    VisitVex(G, vPos);
    visited[vPos] = true;
    for (int i = 0; i < G.vexnum; i++)
    {
        if (visited[i] == false && G.edge[vPos][i] == 1)
        {
            DFS(G, i);
        }
    }
}

void DFSTraverse(Graph G)
{
    InitVisited(G);
    for (int i = 0; i < G.vexnum; i++)
    {
        if (!visited[i])
        {
            DFS(G, i);
        }
    }
}

void BFS(Graph G, int vPos)
{
    VisitVex(G, vPos);
    visited[vPos] = true;
    Queue q;
    InitQueue(q);
    EnQueue(q, vPos);
    while (!QueueEmpty(q))
    {
        int tempPos;
        DeQueue(q, tempPos);

        for (int i = 0; i < G.vexnum; i++)
        {
            if (visited[i] == false && G.edge[tempPos][i] == 1)
            {
                VisitVex(G, i);
                visited[i] = true;
                EnQueue(q, i);
            }
        }
    }
}

void BFSTraverse(Graph G)
{
    InitVisited(G);
    for (int i = 0; i < G.vexnum; i++)
    {
        if (!visited[i])
        {
            BFS(G, i);
        }
    }
}

int dist[MaxSize];
int path[MaxSize];

void MinDistance(Graph G, int startPos)
{
    for (int i = 0; i < G.vexnum; i++)
    {
        visited[i] = false;
        dist[i] = 0;
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
        for (int i = 0; i < G.vexnum; i++)
        {
            if (visited[i] == false && G.edge[tempPos][i] == 1)
            {
                visited[i] = true;
                dist[i] = dist[tempPos] + 1;
                path[i] = tempPos;
                EnQueue(Q, i);
            }
        }
    }
}
