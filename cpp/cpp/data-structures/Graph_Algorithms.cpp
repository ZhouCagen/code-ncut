#include <iostream>
using namespace std;

#define MaxSize 1000
#define nullP -1
#define INF 0x3f3f3f3f

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
    int weight;
} Arc;

int Prim(Graph G, int startPos)
{
    int lowCost[MaxSize];
    int adjVex[MaxSize];
    bool visited[MaxSize];

    if (startPos < 0 || startPos >= G.vexnum || G.directed == true)
        return nullP;

    for (int i = 0; i < G.vexnum; i++)
    {
        visited[i] = false;
        lowCost[i] = G.edge[startPos][i];
        if (i != startPos && G.edge[startPos][i] != INF)
            adjVex[i] = startPos;
        else
            adjVex[i] = nullP;
    }

    visited[startPos] = true;
    lowCost[startPos] = 0;

    int totalWeight = 0;

    for (int i = 1; i < G.vexnum; i++)
    {
        int minWeight = INF;
        int k = nullP;

        for (int j = 0; j < G.vexnum; j++)
        {
            if (visited[j] == false && lowCost[j] < minWeight)
            {
                minWeight = lowCost[j];
                k = j;
            }
        }

        if (k == nullP || minWeight == INF)
            return nullP;

        visited[k] = true;
        totalWeight += minWeight;

        cout << G.vertex[adjVex[k]] << "-" << G.vertex[k] << ":" << minWeight << endl;

        for (int j = 0; j < G.vexnum; j++)
        {
            if (!visited[j] && G.edge[k][j] != INF && G.edge[k][j] < lowCost[j])
            {
                lowCost[j] = G.edge[k][j];
                adjVex[j] = k;
            }
        }
    }

    return totalWeight;
}

typedef struct
{
    int from;
    int to;
    int weight;
} KruskalEdge;

KruskalEdge edgeList[MaxSize * MaxSize];
int parent[MaxSize];

int Find(int x)
{
    if (x != parent[x])
        parent[x] = Find(parent[x]);

    return parent[x];
}

bool Union(int a, int b)
{
    int rootA = Find(a);
    int rootB = Find(b);

    if (rootA == rootB)
        return false;
    parent[rootB] = rootA;
    return true;
}

void QuickSort(int left, int right)
{
    if (left >= right)
        return;

    KruskalEdge pivot = edgeList[left];
    int i = left;
    int j = right;
    while (i < j)
    {
        while (i < j && edgeList[j].weight >= pivot.weight)
            j--;

        edgeList[i] = edgeList[j];

        while (i < j && edgeList[i].weight <= pivot.weight)
            i++;

        edgeList[j] = edgeList[i];
    }

    edgeList[i] = pivot;

    QuickSort(left, i - 1);
    QuickSort(i + 1, right);
}

int Kruskal(Graph G)
{
    if (G.directed)
        return nullP;

    int edgeNum = 0;

    for (int i = 0; i < G.vexnum; i++)
    {
        for (int j = i + 1; j < G.vexnum; j++)
        {
            if (G.edge[i][j] != INF)
            {
                edgeList[edgeNum].from = i;
                edgeList[edgeNum].to = j;
                edgeList[edgeNum].weight = G.edge[i][j];
                edgeNum++;
            }
        }
    }

    QuickSort(0, edgeNum - 1);

    for (int i = 0; i < G.vexnum; i++)
        parent[i] = i;

    int totalWeight = 0;
    int selectedEdgeNum = 0;

    for (int i = 0; i < edgeNum; i++)
    {
        int v = edgeList[i].from;
        int w = edgeList[i].to;

        if (Union(v, w))
        {
            totalWeight += edgeList[i].weight;
            selectedEdgeNum++;
            cout << G.vertex[v] << "-" << G.vertex[w] << ":" << edgeList[i].weight << endl;

            if (selectedEdgeNum == G.vexnum - 1)
                break;
        }
    }

    if (selectedEdgeNum != G.vexnum - 1)
        return nullP;

    return totalWeight;
}

int dist_Dijkstra[MaxSize];
int path_Dijkstra[MaxSize];
bool visited[MaxSize];

int Dijkstra(Graph G, int startPos)
{
    if (startPos < 0 || startPos >= G.vexnum)
        return nullP;

    for (int i = 0; i < G.vexnum; i++)
    {
        for (int j = 0; j < G.vexnum; j++)
        {
            if (G.edge[i][j] < 0)
                return nullP;
        }
    }

    for (int i = 0; i < G.vexnum; i++)
    {
        visited[i] = false;
        dist_Dijkstra[i] = G.edge[startPos][i];
        if (G.edge[startPos][i] != INF && i != startPos)
            path_Dijkstra[i] = startPos;
        else
            path_Dijkstra[i] = nullP;
    }

    dist_Dijkstra[startPos] = 0;
    path_Dijkstra[startPos] = nullP;
    visited[startPos] = true;

    for (int i = 1; i < G.vexnum; i++)
    {
        int minWeight = INF;
        int k = nullP;

        for (int j = 0; j < G.vexnum; j++)
        {
            if (!visited[j] && dist_Dijkstra[j] < minWeight)
            {
                minWeight = dist_Dijkstra[j];
                k = j;
            }
        }

        if (k == nullP || minWeight == INF)
            return nullP;

        visited[k] = true;

        for (int j = 0; j < G.vexnum; j++)
        {
            if (!visited[j] && G.edge[k][j] != INF &&
                dist_Dijkstra[k] + G.edge[k][j] < dist_Dijkstra[j])
            {
                dist_Dijkstra[j] = dist_Dijkstra[k] + G.edge[k][j];
                path_Dijkstra[j] = k;
            }
        }
    }

    return 0;
}

int dist_Floyd[MaxSize][MaxSize];
int path_Floyd[MaxSize][MaxSize];
void Floyd(Graph G)
{
    for (int i = 0; i < G.vexnum; i++)
    {
        for (int j = 0; j < G.vexnum; j++)
        {
            dist_Floyd[i][j] = G.edge[i][j];

            if (i == j)
                dist_Floyd[i][j] = 0;

            if (i != j && G.edge[i][j] != INF)
                path_Floyd[i][j] = i;
            else
                path_Floyd[i][j] = nullP;
        }
    }

    for (int k = 0; k < G.vexnum; k++)
    {
        for (int i = 0; i < G.vexnum; i++)
        {
            for (int j = 0; j < G.vexnum; j++)
            {
                if (dist_Floyd[i][k] != INF && dist_Floyd[k][j] != INF &&
                    dist_Floyd[i][j] > dist_Floyd[i][k] + dist_Floyd[k][j])
                {
                    dist_Floyd[i][j] = dist_Floyd[i][k] + dist_Floyd[k][j];
                    path_Floyd[i][j] = path_Floyd[k][j];
                }
            }
        }
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

int topologicalOrder[MaxSize];

bool TopologicalSort(Graph G)
{
    if (!G.directed)
        return false;

    int inDegree[MaxSize];
    Queue Q;
    InitQueue(Q);

    for (int i = 0; i < G.vexnum; i++)
        inDegree[i] = 0;

    for (int i = 0; i < G.vexnum; i++)
    {
        for (int j = 0; j < G.vexnum; j++)
        {
            if (i != j && G.edge[i][j] != INF)
                inDegree[j]++;
        }
    }

    for (int i = 0; i < G.vexnum; i++)
    {
        if (inDegree[i] == 0)
            EnQueue(Q, i);
    }
    int count = 0;

    while (!QueueEmpty(Q))
    {
        int tempPos;
        DeQueue(Q, tempPos);
        topologicalOrder[count++] = tempPos;
        for (int i = 0; i < G.vexnum; i++)
        {
            if (tempPos != i && G.edge[tempPos][i] != INF)
            {
                inDegree[i]--;
                if (inDegree[i] == 0)
                    EnQueue(Q, i);
            }
        }
    }

    if (count != G.vexnum)
        return false;

    return true;
}
