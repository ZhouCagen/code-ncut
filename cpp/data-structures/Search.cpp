#include <iostream>
using namespace std;

typedef int ElemType;

typedef struct
{
    ElemType* elem;
    int TableLen;
} SSTable;

int Search_Sequence(SSTable st, ElemType key)
{
    st.elem[0] = key;
    int i = st.TableLen;
    while (st.elem[i] != key)
        i--;
    return i;
}

int Search_Binary(SSTable st, ElemType key)
{
    int low = 0, high = st.TableLen - 1, mid = 0;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (key == st.elem[mid])
            return mid;
        else if (key > st.elem[mid])
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }

    return -1;
}

typedef struct BiNode
{
    ElemType data;
    struct BiNode *leftchild, *rightchild;
} BiNode, *BiTree;

BiNode* BST_Search(BiTree T, ElemType key)
{
    while (T != nullptr && key != T->data)
    {
        if (key < T->data)
            T = T->leftchild;
        else
            T = T->rightchild;
    }

    return T;
}

bool BST_Insert(BiTree& T, ElemType key)
{
    if (T == nullptr)
    {
        BiNode* temp = new BiNode();
        temp->data = key;
        temp->leftchild = nullptr;
        temp->rightchild = nullptr;

        T = temp;
        return true;
    }

    if (key == T->data)
        return false;
    else if (key < T->data)
        return BST_Insert(T->leftchild, key);
    else
        return BST_Insert(T->rightchild, key);
}

bool BST_DeleteNode(BiTree& T)
{
    BiNode* temp = T;
    if (temp->leftchild == nullptr && temp->rightchild == nullptr)
    {
        delete temp;
        T = nullptr;
        return true;
    }
    else if (temp->leftchild == nullptr || temp->rightchild == nullptr)
    {
        T = (temp->rightchild == nullptr) ? temp->leftchild : temp->rightchild;
        delete temp;
        return true;
    }
    else
    {
        BiNode* parent = T;
        BiNode* p = T->leftchild;
        while (p->rightchild != nullptr)
        {
            parent = p;
            p = p->rightchild;
        }

        T->data = p->data;

        if (parent == T)
            T->leftchild = p->leftchild;
        else
            parent->rightchild = p->leftchild;

        delete p;

        return true;
    }
}

bool BST_Delete(BiTree& T, ElemType key)
{
    if (T == nullptr)
        return false;
    if (key == T->data)
    {
        return BST_DeleteNode(T);
    }
    else if (key < T->data)
        return BST_Delete(T->leftchild, key);
    else
        return BST_Delete(T->rightchild, key);
}
