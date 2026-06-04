#include <iostream>
#include <climits>

#define Maxsize 1000

using namespace std;

typedef int ElemType;

typedef struct BiNode{
    ElemType data;
    struct BiNode *leftchild, *rightchild;
} BiNode, *BiTree;


int getNodeDegree_1(BiTree t)
{
    if(t == nullptr) return 0;
    if(t->leftchild == nullptr && t->rightchild == nullptr) return 0;
    if(t->leftchild == nullptr || t->rightchild == nullptr)
    {
        return 1 + getNodeDegree_1(t->rightchild == nullptr ? t->leftchild : t->rightchild);
    }
    
    return getNodeDegree_1(t->leftchild) + getNodeDegree_1(t->rightchild);
}

int getNodeDegree_2(BiTree t)
{
    if(t == nullptr) return 0;
    if(t->leftchild != nullptr && t->rightchild != nullptr)
    {
        return 1 + getNodeDegree_2(t->leftchild) + getNodeDegree_2(t->rightchild);
    }
    
    return getNodeDegree_2(t->leftchild) + getNodeDegree_2(t->rightchild);
}

int getNodeDegree_0(BiTree t)
{
    if(t == nullptr) return 0;

    if(t->leftchild == nullptr && t->rightchild == nullptr) return 1;
    
    return getNodeDegree_0(t->leftchild) + getNodeDegree_0(t->rightchild);
        
}

int getDepth(BiTree t)
{
    if(t == nullptr) return 0;

    int leftDepth = 1 + getDepth(t->leftchild);
    int rightDepth = 1 + getDepth(t->rightchild);

    return leftDepth > rightDepth ? leftDepth : rightDepth;
}

int getWidth(BiTree t)
{
    if(t == nullptr) return 0;
    BiNode* queue[Maxsize];
    int front = 0, rear = 0;
    queue[rear++] = t;
    
    int maxWidth = 1;
    while(front < rear)
    {
        int levelWidth = rear - front;
        maxWidth = levelWidth > maxWidth ? levelWidth : maxWidth;
        for(int i = 1; i <= levelWidth; i++)
        {
            BiNode* temp = queue[front++];
            if(temp->leftchild != nullptr) queue[rear++] = temp->leftchild;
            if(temp->rightchild != nullptr) queue[rear++] = temp->rightchild;
        }
    }

    return maxWidth;
}

void deleteLeafNode(BiTree &t)
{
    if(t == nullptr) return;
    if(t->leftchild == nullptr && t->rightchild == nullptr)
    {
        delete t;
        t = nullptr;
    }
    else 
    {
        deleteLeafNode(t->leftchild);
        deleteLeafNode(t->rightchild);
    }
}

int getNodeLevel(BiTree t, BiNode* n, int level = 1)
{
    if(t == nullptr) return 0;
    if(t == n) return level;
    
    int leftLevel = getNodeLevel(t->leftchild, n, level + 1);

    if(leftLevel) return leftLevel;

    return getNodeLevel(t->rightchild, n, level + 1);
}

ElemType getMaxNodeData(BiTree t)
{
    if(t == nullptr) return INT_MIN;
    int maxData = t->data;
    int leftMaxData = getMaxNodeData(t->leftchild);
    int rightMaxData = getMaxNodeData(t->rightchild);

    int tempMax = leftMaxData > rightMaxData ? leftMaxData : rightMaxData;
    return maxData > tempMax ? maxData : tempMax;
}

void changeTree(BiTree &t)
{
    if(t == nullptr) return;
    if(t->rightchild == nullptr && t->leftchild == nullptr) return;
    changeTree(t->leftchild);
    changeTree(t->rightchild);

    BiNode *temp = t->leftchild;
    t->leftchild = t->rightchild;
    t->rightchild = temp;
}

void PreOrder(BiTree t)
{
    if(t == nullptr) return;
    cout << t->data <<" ";
    PreOrder(t->leftchild);
    PreOrder(t->rightchild);
}
