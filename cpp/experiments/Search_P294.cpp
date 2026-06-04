#include <iostream>
using namespace std;

typedef int ElemType;

typedef struct BiNode
{
    ElemType data;
    struct BiNode *leftchild, *rightchild;
} BiNode, *BiTree;

bool judgeBST_boundary(BiTree T, BiNode* minNode, BiNode* maxNode)
{
    if (T == nullptr)
        return true;

    if (minNode != nullptr && T->data < minNode->data)
        return false;
    if (maxNode != nullptr && T->data > maxNode->data)
        return false;

    return judgeBST_boundary(T->leftchild, minNode, T) &&
           judgeBST_boundary(T->rightchild, T, maxNode);
}
bool isBST_boundary(BiTree T)
{
    return judgeBST_boundary(T, nullptr, nullptr);
}

bool judgeBST_InOrder(BiTree T, BiNode* pre)
{
    if (T == nullptr)
        return true;

    if (!judgeBST_InOrder(T->leftchild, pre))
        return false;

    if (pre != nullptr && T->data < pre->data)
        return false;
    pre = T;

    return judgeBST_InOrder(T->rightchild, pre);
}

bool isBST_InOrder(BiTree T)
{
    BiNode* pre = nullptr;
    judgeBST_InOrder(T, pre);
}

int BiTree_NodeDepth(BiTree T, BiNode* p, int currentDepth)
{
    if (T == nullptr)
        return 0;
    if (T == p)
        return currentDepth;
    int leftDepth = BiTree_NodeDepth(T->leftchild, p, currentDepth + 1);

    if (leftDepth != 0)
        return leftDepth;

    return BiTree_NodeDepth(T->rightchild, p, currentDepth + 1);
}

int BST_NodeDepth(BiTree T, BiNode* p)
{
    if (T == nullptr)
        return 0;

    int depth = 0;
    while (T != nullptr)
    {
        if (T == p)
            return depth;

        if (p->data < T->data)
            T = T->leftchild;
        else
            T = T->rightchild;

        depth++;
    }

    return 0;
}

bool judgeAVL(BiTree T, BiNode* minNode, BiNode* maxNode, int& height)
{
    if (T == nullptr)
    {
        height = 0;
        return true;
    }
    if (minNode != nullptr && T->data <= minNode->data)
        return false;
    if (maxNode != nullptr && T->data >= maxNode->data)
        return false;

    int leftDepth = 0, rightDepth = 0;
    if (!judgeAVL(T->leftchild, minNode, T, leftDepth))
        return false;
    if (!judgeAVL(T->rightchild, T, maxNode, rightDepth))
        return false;

    if (leftDepth - rightDepth > 1 || rightDepth - leftDepth > 1)
        return false;

    height = leftDepth > rightDepth ? leftDepth + 1 : rightDepth + 1;

    return true;
}

bool isAVL(BiTree T)
{
    int height = 0;
    return judgeAVL(T, nullptr, nullptr, height);
}

bool getBSTMinKey(BiTree T, int& key)
{
    if (T == nullptr)
        return false;
    while (T->leftchild != nullptr)
        T = T->leftchild;

    key = T->data;
    return true;
}

bool getBSTMaxKey(BiTree T, int& key)
{
    if (T == nullptr)
        return false;
    while (T->rightchild != nullptr)
        T = T->rightchild;

    key = T->data;
    return true;
}

void getGreaterKey(BiTree T, int key)
{
    if (T == nullptr)
        return;

    if (T->data >= key)
    {
        getGreaterKey(T->rightchild, key);
        cout << T->data << " ";
        getGreaterKey(T->leftchild, key);
    }
    else
        getGreaterKey(T->leftchild, key);
}

typedef struct BiSortNode
{
    ElemType data;
    int count;
    struct BiSortNode* lchild;
    struct BiSortNode* rchild;
} BiSortNode, *BiSortTree;

BiSortNode* searchKSmallNode(BiSortTree T, int k)
{
    if (T == nullptr)
        return nullptr;
    if (k > T->count)
        return nullptr;

    int leftCount = 0;
    if (T->lchild != nullptr)
        leftCount = T->lchild->count;

    if (k == leftCount + 1)
        return T;
    else if (k < leftCount + 1)
        return searchKSmallNode(T->lchild, k);
    else
        return searchKSmallNode(T->rchild, k - leftCount - 1);
}
