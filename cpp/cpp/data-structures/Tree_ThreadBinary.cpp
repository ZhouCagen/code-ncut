#include <iostream>
using namespace std;

typedef int ElemType;

typedef struct BiThreadNode{
    ElemType data;
    struct BiThreadNode *lchild, *rchild;
    int ltag, rtag;
} BiThreadNode, *BiThreadTree;

void InThread(BiThreadTree p, BiThreadNode* &pre)
{
    if(p != nullptr)
    {
        InThread(p->lchild, pre);
        if(p->lchild == nullptr)
        {
            p->ltag = 1;
            p->lchild = pre;
        }

        if(pre != nullptr && pre->rchild == nullptr)
        {
            pre->rtag = 1;
            pre->rchild = p;
        }

        pre = p;
        InThread(p->rchild, pre);
    }
}

void CreateInThread(BiThreadTree T)
{
    BiThreadNode* pre = nullptr;
    if(T != nullptr)
    {
        InThread(T, pre);
        pre->rtag = 1;
        pre->rchild = nullptr;
    }
}

BiThreadNode* FirstNodeIn(BiThreadTree p)
{
    if(p == nullptr)    return nullptr;
    while(p->ltag == 0) p = p->lchild;
    return p;
}

BiThreadNode* NextNodeIn(BiThreadNode *node)
{
    if(node->rtag == 1) return node->rchild;
    else return FirstNodeIn(node->rchild);
}

BiThreadNode* LastNodeIn(BiThreadTree p)
{
    if(p == nullptr) return nullptr;
    while(p->rtag == 0) p = p->rchild;
    return p;
}

BiThreadNode* PreNodeIn(BiThreadNode *node)
{
    if(node->ltag == 1) return node->lchild;
    else return LastNodeIn(node->lchild);
}

void visit(BiThreadNode* node)
{
    if(node != nullptr) cout << node->data << " ";
}

void InOrder(BiThreadTree T)
{
    if(T == nullptr)    return;
    for(BiThreadNode* p = FirstNodeIn(T); p != nullptr; p = NextNodeIn(p))  visit(p);
}

void RevInOrder(BiThreadTree T)
{
    if(T == nullptr)    return;
    for(BiThreadNode* p = LastNodeIn(T); p!= nullptr; p = PreNodeIn(p))  visit(p);
}




void PreThread(BiThreadTree p, BiThreadNode *&pre)
{
    if(p != nullptr)
    {
        if(p->lchild == nullptr)
        {
            p->ltag = 1;
            p->lchild = pre;
        }

        if(pre != nullptr && pre->rchild == nullptr)
        {
            pre->rtag = 1;
            pre->rchild = p;
        }

        pre = p;
        if(p->ltag == 0) PreThread(p->lchild, pre);
        PreThread(p->rchild, pre);
    }
}

void CreatePreThread(BiThreadTree T)
{
    BiThreadNode* pre = nullptr;
    if(T != nullptr)
    {
        PreThread(T, pre);
        pre->rtag = 1;
        pre->rchild = nullptr;
    }
}

BiThreadNode* NextNodePre(BiThreadNode *node)
{
    if(node->ltag == 0) return node->lchild;
    else return node->rchild;
}

void PreOrder(BiThreadTree T)
{
    if(T == nullptr)    return;
    for(BiThreadNode* p = T; p != nullptr; p = NextNodePre(p)) visit(p);
}




void PostThread(BiThreadTree p, BiThreadNode *&pre)
{
    if(p != nullptr)
    {
        PostThread(p->lchild, pre);
        PostThread(p->rchild, pre);
        if(p->lchild == nullptr)
        {
            p->ltag = 1;
            p->lchild = pre;
        }

        if(pre != nullptr && pre->rchild == nullptr)
        {
            pre->rtag = 1;
            pre->rchild = p;
        }

        pre = p;
    }
}

void CreatePostThread(BiThreadTree T)
{
    BiThreadNode *pre = nullptr;
    if(T != nullptr)
    {
        PostThread(T, pre);
        if(pre != nullptr)
        {
            if(pre->rchild == nullptr)
            {
                pre->rtag = 1;
            }
        }
    }
}

BiThreadNode* PreNodePost(BiThreadNode *node)
{
    if(node->rtag == 0) return node->rchild;
    else return node->lchild;
}

void RevPostOrder(BiThreadTree T)
{
    if(T == nullptr)    return;
    for(BiThreadNode* p = T; p!= nullptr; p = PreNodePost(p))  visit(p);
}



typedef struct TriThreadNode{
    ElemType data;
    struct TriThreadNode *lchild, *rchild;
    struct TriThreadNode *parent;
    int ltag, rtag;
} TriThreadNode, *TriThreadTree;

void PostThread(TriThreadTree p, TriThreadNode *&pre)
{
    if(p != nullptr)
    {
        if(p->lchild != nullptr && p->ltag == 0) p->lchild->parent = p;
        if(p->rchild != nullptr && p->rtag == 0) p->rchild->parent = p;

        PostThread(p->lchild, pre);
        PostThread(p->rchild, pre);
        if(p->lchild == nullptr)
        {
            p->ltag = 1;
            p->lchild = pre;
        }

        if(pre != nullptr && pre->rchild == nullptr)
        {
            pre->rtag = 1;
            pre->rchild = p;
        }

        pre = p;
    }
}

void CreatePostThread(TriThreadTree T)
{
    TriThreadNode *pre = nullptr;
    if(T != nullptr)
    {
        T->parent = nullptr;
        PostThread(T, pre);
        if(pre != nullptr)
        {
            if(pre->rchild == nullptr)
            {
                pre->rtag = 1;
            }
        }
    }
}

TriThreadNode* FirstNodePost(TriThreadTree p)
{
    if(p == nullptr) return nullptr;
    while(p->ltag == 0 || p->rtag == 0)
    {
        if(p->ltag == 0) p = p->lchild;
        else p = p->rchild;
    }

    return p;
}

TriThreadNode* NextNodePost(TriThreadNode *node)
{
    if(node->parent == nullptr) return nullptr;
    if(node == node->parent->rchild || node->rtag == 1) return node->rchild;
    return FirstNodePost(node->parent->rchild);
}

void visit(TriThreadNode* node)
{
    if(node != nullptr) cout << node->data << " ";
}


void PostOrder(TriThreadTree T)
{
    if (T == nullptr) return;
    TriThreadNode* first = FirstNodePost(T);
    for (TriThreadNode* p = first; p != nullptr; p = NextNodePost(p))   visit(p);
}