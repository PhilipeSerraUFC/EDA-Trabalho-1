#include <bits/stdc++.h>
#define MAX_VERSION 100

enum NodeField {
    key,
    left, 
    right, 
    parent
};


struct Modification {
    /* Modificação contém a versão, o campo que foi alterado
    Caso o campo alterado tenha sido a chave, então o guardamos a modificação em key
    Caso seja um ponteiro, então guardamos a modificação em pointer */

    int version;
    NodeField field;
    int key;
    Node* pointer;

};

struct Node {

    int key;
    Node* left;
    Node* right;
    Node* parent;
    Modification modifications[];  

};


struct BinarySearchTree {

    Node* roots[MAX_VERSION]; //Raizes por versão, roots[version] é a raiz da versão version, roots[0] é a raiz da arvore vazia.
    int last_version;

};

void insert(BinarySearchTree* tree, int key){
    
    



}
