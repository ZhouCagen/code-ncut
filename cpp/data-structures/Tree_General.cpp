#include <iostream>
using namespace std;

typedef int ElemType;

// Parent Representation
typedef struct PTNode{
    ElemType data;
    int parent;
} PTNode;

typedef struct{
    PTNode *nodes;
    int n, r;
    int size;
} PTree;


// Child Representation

typedef struct CTNode{
    int child;
    struct CTNode *next;
} CTNode, *ChildPtr;

typedef struct{
    ElemType data;
    // int parent;
    ChildPtr firstchild;
} CTBox;

typedef struct{
    CTBox *nodes;
    int n, r;
    int size;
} CTree;

// Child-Sibling Representation

typedef struct CSNode{
    int data;
    struct CSNode *firstChild, *nextSibling;
} CSNode, *CSTree;