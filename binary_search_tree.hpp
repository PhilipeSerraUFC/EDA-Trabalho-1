#include <bits/stdc++.h>
#define MAX_VERSION 100
#define NONE -1000

//TODO RETIRAR O CAMPO PARENTE E VER COMO ATUALIZO OS PAIS 
//FAÇO ISSO COLOCANDO O CAMINHO FEITO NA FUNÇÃO DE UPDATE

enum NodeField {
    key,
    left, 
    right
};


struct Modification {
    /* Modificação contém a versão, o campo que foi alterado
    Caso o campo alterado tenha sido a chave, então o guardamos a modificação em key
    Caso seja um ponteiro, então guardamos a modificação em pointer */

    int version;
    NodeField field;
    int key;
    Node* pointer;

    Modification(int version_, NodeField field_, int key_, Node* pointer_){
        version = version_;
        field = field_;
        key = key_;
        pointer = pointer_;
    }

};

struct Node {
    //Como o máximo de modificações dentro de um nó de arvore binária é 1, vamo criar apenas um campo de modificação em vez de uma lista
    int key;
    Node* left;
    Node* right;
    Modification* modification;  

    Node(int key_, Node* left_, Node* right_){
        key = key_;
        left = left_;
        right = right_;
        modification = nullptr;
    }

};


struct BinarySearchTree {

    Node* roots[MAX_VERSION]; //Raizes por versão, roots[version] é a raiz da versão version, roots[0] é a raiz da arvore vazia.
    int last_version;

    BinarySearchTree(){
        for(int i = 0; i < MAX_VERSION; i++) roots[i] = nullptr;
        last_version = 0;
    }

};

void update_node(Node* node, BinarySearchTree* tree, int version, NodeField field, int key, Node* pointer){

    if(node == nullptr) return;

    if (node->modification == nullptr){
        if(field == key) node->modification = new Modification(version, field, key, nullptr);
        else node->modification = new Modification(version, field, NONE, pointer);

        tree->roots[version] = tree->roots[version-1];
    
        return;
    }

    Node* new_node;

    if(field == NodeField::key) new_node = new Node(key, node->left, node->right, node->parent);
    if(field == NodeField::left) new_node = new Node(node->key, pointer, node->right, node->parent);
    if(field == NodeField::right) new_node = new Node(node->key, node->left, pointer, node->parent);
    if(field == NodeField::parent) new_node = new Node(node->key, node->left, node->right, pointer);

    if(node->parent == nullptr){
        tree->roots[version] = new_node;
        return;
    }

    if(node == node->parent->left)
        update_node(node->parent, tree, version, NodeField::left, NONE, new_node);
    
    if(node == node->parent->right)
        update_node(node->parent, tree, version, NodeField::right, NONE, new_node);


}

void insert(BinarySearchTree* tree, int key){
    
    Node* root = tree->roots[tree->last_version];

    if(root == nullptr){
        root = new Node(key, nullptr, nullptr, nullptr);
    }

    tree->last_version += 1;

    Node* new_node;

    //Procurando qual nó é o pai do novo nó
    Node* current_node = root;
    NodeField son_field;

    while(current_node->right != nullptr || current_node->left != nullptr){
        if(key >= current_node->key) {

            if(current_node->right == nullptr){
                son_field = NodeField::right;
                break;
            } 
            current_node = current_node->right;

        }

        else{

            if(current_node->left == nullptr){
                son_field = NodeField::left;
                break;
            } 
            current_node = current_node->left;

        }
        
    }

    //Criação do novo nó
    new_node = new Node(key, nullptr, nullptr, current_node);

    //E atualização do pai
    update_node(current_node, tree, tree->last_version, son_field, current_node->key, new_node);

}

void remove(BinarySearchTree* tree, int key){
    
    Node* root = tree->roots[tree->last_version];

    if(root == nullptr) return;

    tree->last_version += 1;

    //Procurando onde está o nó
    Node* current_node = root;
    NodeField son_field;

    while(current_node->right != nullptr || current_node->left != nullptr){
        if(key > current_node->key) {

            if(current_node->right == nullptr) return; //Nó não está na arvore

            current_node = current_node->right;
            son_field == NodeField::right;

        }

        if( key < current_node->key) {

            if(current_node->left == nullptr) return; //Nó não está na arvore
            current_node = current_node->left;
            son_field == NodeField::left;

        }

        if(key == current_node->key) {
            break;
        }
        
    }

    if(current_node->left == nullptr && current_node->right == nullptr){
        //Se é folha
        if(current_node->parent == nullptr){
            //Se é folha e raiz
            tree->roots[tree->last_version] = nullptr;
            return;
        }

        update_node(current_node->parent, tree, tree->last_version, son_field, NONE, nullptr);
    }

    else if(current_node->left == nullptr){
        //Só tem o filho direito
        update_node(current_node->parent, tree, tree->last_version, son_field, NONE, current_node->right);

    }else if(current_node->right == nullptr){
        //Só tem o filho esquerdo
        update_node(current_node->parent, tree, tree->last_version, son_field, NONE, current_node->left);

    }else{
        //Tem dois filhos
        //Por limitação no quantidade de campos de Modification, é 
        //Necessário tirar dois nós (o a ser removido e o da menor folha de sua subarvore direita)
        //E colocar um novo em sua posição com a mesma chave que o menor folha de sua subarvore direita
        Node* smallest_node = find_smallest_node_in_tree(current_node->right, tree->last_version);
        Node* new_node = new Node(smallest_node->key, current_node->left, current_node->right, current_node->parent);
        new_node->left = current_node->left;
        new_node->right = current_node->right;
        
        update_node(current_node->parent, tree, tree->last_version, son_field, NONE, new_node);
        
    }
}


Node* find_smallest_node_in_tree(Node* root, int version){
    return nullptr;
}

Node* copy_node(Node* node){
    return nullptr;
}
