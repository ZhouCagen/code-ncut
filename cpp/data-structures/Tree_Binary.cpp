#include <iostream>
#include <queue>
using namespace std;

typedef int ElemType;

typedef struct BiTNode{
    ElemType data;
    struct BiTNode *lchild, *rchild;
} BiTNode, *BiTree;

BiTree InitBiTree()
{
    BiTree T = new BiTNode();
    T->data = 0;
    T->lchild = nullptr;
    T->rchild = nullptr;
    return T;
}

BiTree MakeBiTree(ElemType e, BiTree L, BiTree R){
    BiTree T = new BiTNode();
    T->data = e;
    T->lchild = L;
    T->rchild = R;
    return T;
}

BiTree FreeBiTree(BiTree &T)
{
    if(T == nullptr) return nullptr;

    FreeBiTree(T->lchild);
    FreeBiTree(T->rchild);
    delete T;
    T = nullptr;
    return nullptr;
}

bool BiTreeEmpty(BiTree T)
{
    return T == nullptr;
}

int BiTreeDepth(BiTree T)
{
    if(BiTreeEmpty(T)) return 0;
    int leftDepth = BiTreeDepth(T->lchild);
    int rightDepth = BiTreeDepth(T->rchild);

    return max(leftDepth, rightDepth) + 1;
}

BiTNode* Root(BiTree T)
{
    return T;
}

ElemType Value(BiTree T, BiTNode* node)
{
    if(node != nullptr) return node->data;

    return -1;
}

bool Assign(BiTree &T, BiTNode* node, ElemType value)
{
    if(node != nullptr)
    {
        node->data = value;
        return true;
    }
    return false;
}

BiTNode* Parent(BiTree T, BiTNode* node)
{
    if(BiTreeEmpty(T)) return nullptr;
    if(T->lchild == node || T->rchild == node) return T;

    BiTNode* p = Parent(T->lchild, node);
    if(p != nullptr) return p;

    return Parent(T->rchild, node);
}

BiTNode* LeftChild(BiTree T, BiTNode* node)
{
    return node != nullptr ? node->lchild : nullptr;
}

BiTNode* RightChild(BiTree T, BiTNode* node)
{
    return node != nullptr ? node->rchild : nullptr;
}

BiTree ReplaceLeft(BiTree &T, BiTree LT)
{
    if(BiTreeEmpty(T)) return nullptr;
    BiTree l = T->lchild;
    T->lchild = LT;
    return l;
}

BiTree ReplaceRight(BiTree &T, BiTree RT)
{
    if(BiTreeEmpty(T)) return nullptr;
    BiTree r = T->rchild;
    T->rchild = RT;
    return r;
}

void visit(BiTNode* node)
{
    if(node != nullptr) cout << node->data << " ";
}

void PreOrderTrav(BiTree T)
{
    if(!BiTreeEmpty(T))
    {
        visit(T);
        PreOrderTrav(T->lchild);
        PreOrderTrav(T->rchild);
    }
}

void InOrderTrav(BiTree T)
{
    if(!BiTreeEmpty(T))
    {
        InOrderTrav(T->lchild);
        visit(T);
        InOrderTrav(T->rchild);
    }
}

void PostOrderTrav(BiTree T)
{
    if(!BiTreeEmpty(T))
    {
        PostOrderTrav(T->lchild);
        PostOrderTrav(T->rchild);
        visit(T);
    }
}

void LevelOrderTrav(BiTree T)
{
    if(BiTreeEmpty(T)) return;
    queue<BiTNode*> q;
    q.push(T);
    while(!q.empty())
    {
        BiTNode* current = q.front();
        q.pop();
        visit(current);

        if(!BiTreeEmpty(current->lchild)) q.push(current->lchild);
        if(!BiTreeEmpty(current->rchild)) q.push(current->rchild);
    }
}
