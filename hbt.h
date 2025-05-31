#ifndef __HBT_TREE__
#define __HBT_TREE__

typedef struct _Tnode {
   int key: 29, balance: 3;
   struct _Tnode *left;
   struct _Tnode *right;
} Tnode;

#define HBT_MAX 268435455
#define HBT_MIN -268435456

#endif
