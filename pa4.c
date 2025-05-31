#include<stdio.h>
#include "hbt.h"
#include<stdlib.h>
#include<string.h>

Tnode* GetNewNode(int data){
    Tnode* newNode = (Tnode*)malloc(sizeof(Tnode));
    newNode->key = data;
    newNode->balance = 0;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void freeTree(Tnode* root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int max(int num1, int num2) {
    return (num1 > num2) ? num1 : num2;
}

int findHeight(Tnode* root){
    if(root == NULL){
        return -1;
    }

    return max(findHeight(root->left), findHeight(root->right)) + 1;
}

Tnode* CCR(Tnode* old){
    if(old == NULL || old->right == NULL) return old;
    Tnode* new = old->right;
    old->right = new->left;
    new->left = old;
    return new;
}

Tnode* CR(Tnode* old){
    if(old == NULL || old->left == NULL) return old;
    Tnode* new = old->left;
    old->left = new->right;
    new->right = old;
    return new;
}

Tnode* balanceBST(Tnode* rootA, int change, int* endIter){
    if(rootA == NULL) return NULL;
    *endIter = 0;

    if ((rootA->left != NULL &&                                         
        (rootA->left->left != NULL || rootA->left->right != NULL) &&   
        rootA->left->balance == 0)                                     
       ||                                                              
       (rootA->right != NULL &&                                        
        (rootA->right->left != NULL || rootA->right->right != NULL) && 
        rootA->right->balance == 0)) {                                 
        *endIter = 1;
        printf("**commence next insertion**\n");
        return rootA;
    }

    if(change <= rootA->key) {
        printf("Balance of %d goes from %d", rootA->key, rootA->balance);
        (rootA->balance)++;
        printf(" to %d\n", rootA->balance);
    } else if(change > rootA->key) {
        printf("Balance of %d goes from %d", rootA->key, rootA->balance);
        (rootA->balance)--;
        printf(" to %d\n", rootA->balance);
    }

    Tnode* rootFin = rootA;
    
    if(rootA->balance == 2 || rootA->balance == -2){
        Tnode* leftA = rootA->left;
        Tnode* rightA = rootA->right;

        printf("=== Rebalancing Node %d ===\n", rootA->key);
        printf("Node balance: %d\n", rootA->balance);
        if(leftA != NULL) printf("Left child (%d) balance: %d\n", leftA->key, leftA->balance);
        if(rightA != NULL) printf("Right child (%d) balance: %d\n", rightA->key, rightA->balance);
        
        //CR or CCR based on rules in class
        
        //case 1: taller on same side left
        if(rootA->balance == 2 && leftA->balance == 1 && leftA != NULL){
            printf("in case 1\n");
            rootFin = CR(rootA);
            rootFin->balance = 0;
            if(rootFin->right != NULL) rootFin->right->balance = 0;
        }
        //case 3: taller on same side right
        else if(rootA->balance == -2 && rightA->balance == -1 && rightA != NULL){
            printf("in case 3\n");
            rootFin = CCR(rootA);
            rootFin->balance = 0;
            if(rootFin->left != NULL) rootFin->left->balance = 0;
        } 
        //case 2: not same side left
        else if(rootA->balance == 2 && leftA->balance == -1 && leftA != NULL){ 
            printf("in case 2\n");
            rootFin->left = CCR(leftA);
            rootFin = CR(rootFin);
            if(rootFin->balance == 0) {
                if(rootFin->left != NULL) rootFin->left->balance = 0;
                if(rootFin->right != NULL) rootFin->right->balance = 0;
            } else if(rootFin->balance == 1) {
                if(rootFin->left != NULL) rootFin->left->balance = 0;
                if(rootFin->right != NULL) rootFin->right->balance = -1;
            } else {
                if(rootFin->left != NULL) rootFin->left->balance = 1;
                if(rootFin->right != NULL) rootFin->right->balance = 0;
            }
            rootFin->balance = 0;
        }
        //case 4: not same side right
        else if(rootA->balance == -2 && rightA->balance == 1 && rightA != NULL){ 
            printf("in case 4\n");
            Tnode* temp = CR(rightA);
            if(temp == NULL) return rootA;
            
            rootFin->right = temp;
            
            rootFin = CCR(rootFin);
            if(rootFin == NULL) return rootA;
            
            rootFin->balance = 0;
            if(rootFin->left != NULL) {
                if(rootFin->left->balance == 0) {
                    rootFin->left->balance = 0;
                } else {
                    rootFin->left->balance = 1;
                }
            }
            if(rootFin->right != NULL) {
                if(rootFin->right->balance == 0) {
                    rootFin->right->balance = 0;
                } else {
                    rootFin->right->balance = -1;
                }
            }
        }
        printf("=== Rebalancing Complete ===\n");
        printf("rootFin: %d\n", rootFin->key);
        printf("rootFin balance: %d\n", rootFin->balance);
        if (rootFin->left != NULL) printf("rootFinleft: %d\n", rootFin->left->key);
        if (rootFin->right != NULL) printf("rootFinright: %d\n", rootFin->right->key);
    }
    
    return rootFin;
}

Tnode* Insert(Tnode* root, int data, int endIter){

    if(root == NULL) { //empty tree
        root = GetNewNode(data);
        printf("Inserted: %d (new node)\n", data);
        return root;
    }

    if(endIter == 1) {
        return root;
    }

    if(data <= root->key){
        printf("Inserting %d: traversing to ", data);
        printf("left of %d\n", root->key);
        root->left = Insert(root->left, data, 0);
    }
    else if(data > root->key){
        printf("Inserting %d: traversing to ", data);
        printf("right of %d\n", root->key);
        root->right = Insert(root->right, data, 0);
    }

    int newEndIter = 0;
    root = balanceBST(root, data, &newEndIter);
    
    return root;
}

int FindMax(Tnode* root){

    if(root == NULL){
        //printf("Empty Tree");
        return -1;
    }
    else if(root->right == NULL){
        return root->key;
    }
    else{
        return FindMax(root->right);
    }
}

Tnode* delete(Tnode* root, int data){

    int endIter;

    if(root == NULL) return root;
    
    if(data < root->key){
        root->left = delete(root->left, data);
    }
    else if(data > root->key){
        root->right = delete(root->right, data);
    }
    else{ //data = root->key
        //case no child
        if(root->left == NULL && root->right == NULL){
            //printf("Deleted: %d (no children)\n", root->key);
            free(root);
            return NULL;
        }
        //case 1 child
        else if(root->left == NULL){
            //printf("Deleted: %d (right child)\n", root->key);
            Tnode* temp = root->right;
            free(root);
            return temp;
        }
        else if(root->right == NULL){
            //printf("Deleted: %d (left child)\n", root->key);
            Tnode* temp = root->left;
            free(root);
            return temp;
        }
        //case 2 children
        else{ //max predecessor
            //printf("Deleted: %d (two children)\n", root->key);
            int temp = FindMax(root->left);
            //printf("Replacing with predecessor: %d\n", temp);
            root->key = temp;
            root->left = delete(root->left, temp);
        }
    }
    
    return balanceBST(root, data, &endIter);
}

int isBalanced(Tnode* root) {
    if (root == NULL) return 1;
    
    int balance = findHeight(root->left) - findHeight(root->right);
    if (balance > 1 || balance < -1) return 0;
    
    return isBalanced(root->left) && isBalanced(root->right);
}

void InOrder(Tnode* root, int* arr, int* idx){
    if(root == NULL) return;
    InOrder(root->left, arr, idx);
    arr[*idx] = root->key;
    (*idx)++;
    InOrder(root->right, arr, idx);
}

int isBST(Tnode* root, int size){
    int idx = 0;
    int* arr = (int*)malloc(size * sizeof(int));
    if(arr == NULL) return 0;

    InOrder(root, arr, &idx);
    
    //printf("BST Check - In-order traversal: ");
    // for(int i = 0; i < idx; i++) {
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");
    
    for (int i = 0; i < idx - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            //printf("BST violation: %d > %d\n", arr[i], arr[i+1]);
            free(arr);
            return 0;
        }
    }
    free(arr);
    return 1;
}


void writePreOrder(Tnode* root, FILE* file) {
    if (root == NULL) return;
    
    int key = root->key;
    char branches = 0;
    
    if (root->left != NULL) branches |= 2;
    if (root->right != NULL) branches |= 1;
    
    fwrite(&key, sizeof(int), 1, file);
    fwrite(&branches, sizeof(char), 1, file);
    
    writePreOrder(root->left, file);
    writePreOrder(root->right, file);
}

Tnode* readPreOrder(FILE* file) {
    int key;
    char children;  

    if (fread(&key, sizeof(int), 1, file) != 1 ||
        fread(&children, sizeof(char), 1, file) != 1) {
        return NULL;
    }
    else {
        Tnode* root = GetNewNode(key);
         
        if (children == 3) {  // both children
            root->left = readPreOrder(file);
            root->right = readPreOrder(file);
            
            if (root->left == NULL || root->right == NULL) {
                freeTree(root);
                return NULL;
            }
        }
        else if (children == 2) {  // left child only
            root->left = readPreOrder(file);
            if (root->left == NULL) {
                freeTree(root);
                return NULL;
            }
        }
        else if (children == 1) {  // right child only
            root->right = readPreOrder(file);
            if (root->right == NULL) {
                freeTree(root);
                return NULL;
            }
        }
        else if (children != 0) {  // invalid children bits
            freeTree(root);
            return NULL;
        }
        return root;
    }
}

int main(int argc, char *argv[]) {

    Tnode* root = NULL;
    
    if (strcmp(argv[1], "-b") == 0) { //build
        FILE *operations_input_file = fopen(argv[2], "rb");
        if (operations_input_file == NULL) {
            printf("%d\n", -1);
            freeTree(root);
            return EXIT_FAILURE;
        }

        FILE *tree_output_file = fopen(argv[3], "wb");
        if (tree_output_file == NULL) {
            fclose(operations_input_file);
            printf("%d\n", -1);
            freeTree(root);
            return EXIT_FAILURE;
        }

        int key;
        char op;
        
        //printf("=== Reading Operations ===\n");
        while (fread(&key, sizeof(int), 1, operations_input_file) == 1 && fread(&op, sizeof(char), 1, operations_input_file) == 1) {
            //printf("Operation: %c %d\n", op, key);
            if (op == 'i') {
                root = Insert(root, key, 0);
            } else if (op == 'd') {
                root = delete(root, key);
            }
            
            if (root == NULL && op == 'i') {
                //printf("0\n");
                writePreOrder(root, tree_output_file);
                fclose(operations_input_file);
                fclose(tree_output_file);
                freeTree(root);
                return EXIT_FAILURE;
            }
        }

        writePreOrder(root, tree_output_file);
        
        fclose(operations_input_file);
        fclose(tree_output_file);
        printf("%d\n", 1);
        freeTree(root);
        return EXIT_SUCCESS;
    } 


    else if (strcmp(argv[1], "-e") == 0) { //eval
        
        FILE *tree_file = fopen(argv[2], "rb");

        int valid = 1;

        if (tree_file == NULL) { 
            //printf("issue\n");
            printf("%d,0,0\n", -1);
            return EXIT_FAILURE;
        }

        int size = 0;
        int key;
        char children;
        fseek(tree_file, 0, SEEK_SET);

        while (fread(&key, sizeof(int), 1, tree_file) == 1 && fread(&children, sizeof(char), 1, tree_file) == 1) {
            size++;
        }

        if (!feof(tree_file) || size == 0) {
            valid = 0;
        }

        fseek(tree_file, 0, SEEK_SET);
        
        root = NULL;
        if (valid == 1) {
            root = readPreOrder(tree_file);
            if (root == NULL) { 
                valid = 0;
                printf("%d,0,0\n", valid);
            } else {
                printf("%d,%d,%d\n", valid, isBST(root, size), isBalanced(root));
                freeTree(root);
            }
        } 
        else {
            printf("%d,0,0\n", valid);
        }

        fclose(tree_file);
        return (valid == 1) ? EXIT_SUCCESS : EXIT_FAILURE;
    
    }
    
    //printf("hmm");
    freeTree(root);
    return EXIT_FAILURE;
}

