#include <bits/stdc++.h>
#include<vector>
#define MAX_VERSION 100
#define NONE -1000
#define INF numeric_limits<int>::infinity()

//TODO RETIRAR O CAMPO PARENTE E VER COMO ATUALIZO OS PAIS 
//FAÇO ISSO COLOCANDO O CAMINHO FEITO NA FUNÇÃO DE UPDATE

using namespace std;

enum NodeField {
    key,
    left, 
    right
};

class Node;

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

class Node {
    //É bom o uso de classes, pois evita problemas de atualizar sem querer o nó 
    //Sem ter usado persistência

    //Como o máximo de modificações dentro de um nó de arvore binária é 1, vamo criar apenas um campo de modificação em vez de uma lista
    
    private:
        int key;
        Node* left;
        Node* right;
        Modification* modification;  

    public:

        Node(int key_, Node* left_, Node* right_){
            key = key_;
            left = left_;
            right = right_;
            modification = nullptr;
        }

        int GetKey(int version){
            if(this->modification == nullptr) return this->key;

            if(this->modification->version <= version) if(this->modification->field == NodeField::key) {return this->modification->key;}

            return this->key;

        }

        Node* GetLeft(int version){
            if(this->modification == nullptr) return this->left;

            if(this->modification->version <= version) if(this->modification->field == NodeField::left) {
                return this->modification->pointer ; }

            return this->left;
        }

        Node* GetRight(int version){
            if(this->modification == nullptr) return this->right;

            if(this->modification->version <= version) if(this->modification->field == NodeField::right) { 
                return this->modification->pointer ;
            }

            return this->right;
        }

        Modification* GetModification(){ return this->modification; }


        void SetModification(int version, NodeField field, int key, Node* pointer){
            if(this->modification == nullptr) this->modification = new Modification(version, field, key, pointer);
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

Node* UpdateNode(Node* node, BinarySearchTree* tree, NodeField field, int key, Node* pointer, vector<Node*>& path){
    /*atualiza a arvore e Retorna o nó atualizado*/
    if(node == nullptr) return nullptr;

    int version = tree->last_version;


    
    if (node->GetModification() == nullptr){
        
        if(field == NodeField::key) node->SetModification(version, field, key, nullptr);
        else node->SetModification(version, field, NONE, pointer);

        if(tree->roots[version] == nullptr) tree->roots[version] = tree->roots[version-1];
        
        return node;
    }

    Node* new_node;

    int node_key = node->GetKey(version);
    Node* node_left = node->GetLeft(version);
    Node* node_right = node->GetRight(version);

    if(field == NodeField::key) new_node = new Node(key, node_left, node_right);
    if(field == NodeField::left) new_node = new Node(node_key, pointer, node_right);
    if(field == NodeField::right) new_node = new Node(node_key, node_left, pointer);

    if(node == tree->roots[version-1] || node == tree->roots[version]){
        tree->roots[version] = new_node;
        return new_node;
    }

    Node* node_parent = path.back();
    path.pop_back();

    if(node == node_parent->GetLeft(version))
        UpdateNode(node_parent, tree, NodeField::left, NONE, new_node, path);
    
    if(node == node_parent->GetRight(version))
        UpdateNode(node_parent, tree, NodeField::right, NONE, new_node, path);

    return new_node;

}

void Insert(BinarySearchTree* tree, int key){
    
    Node* root = tree->roots[tree->last_version];
    tree->last_version += 1;
    
    if(root == nullptr){
        tree->roots[tree->last_version] = new Node(key, nullptr, nullptr);
        return;
    }

    int version = tree->last_version;

    Node* new_node;

    //Procurando qual nó é o pai do novo nó
    Node* current_node = root;
    NodeField son_field;
    
    vector<Node*> path = {};

    do{

        if(key >= current_node->GetKey(version)) {

            if(current_node->GetRight(version) == nullptr){
                son_field = NodeField::right;
                break;
            } 

            son_field = NodeField::right;
            path.push_back(current_node);
            current_node = current_node->GetRight(version);

        }

        else{

            if(current_node->GetLeft(version) == nullptr){
                son_field = NodeField::left;
                break;
            } 

            son_field = NodeField::left;
            path.push_back(current_node);
            current_node = current_node->GetLeft(version);

        }
        
    }while(true);

    //Criação do novo nó
    new_node = new Node(key, nullptr, nullptr);



    //E atualização do pai
    UpdateNode(current_node, tree, son_field, current_node->GetKey(version), new_node, path);
    

}

vector<Node*> FindPathToSmallest(Node* root, int version){

    if(root == nullptr) return {};

    Node* current_node = root;
    vector<Node*> path = {current_node};

    while(current_node->GetLeft(version) != nullptr){
        current_node = current_node->GetLeft(version);
        path.push_back(current_node);
    }

    return path;

}

vector<Node*> FindPath(BinarySearchTree* tree, int key, int version){
    Node* current_node = tree->roots[version];

    if(current_node == nullptr) return {}; 


    vector<Node*> path = {}; 

    while(current_node != nullptr){
        if(key > current_node->GetKey(version)){
            path.push_back(current_node);
            current_node = current_node->GetRight(version);
        }

        if(key < current_node->GetKey(version)){
            path.push_back(current_node);
            current_node = current_node->GetLeft(version);
        }        

        if(current_node->GetKey(version) == key){
            path.push_back(current_node);
            break;
        }               

    }

    if(current_node == nullptr){
        return {};
    }

    return path;
}

void Remove(BinarySearchTree* tree, int key){
    
    Node* root = tree->roots[tree->last_version];

    //Se está vazia
    if(root == nullptr) return;

    int version = tree->last_version;
    //Procurando onde está o nó
    Node* current_node = root;
    NodeField son_field;
    vector<Node*> path;
    bool found_node = false;

    do{
        if(key > current_node->GetKey(version)) {

            if(current_node->GetRight(version) == nullptr) return; //Nó não está na arvore

            path.push_back(current_node);
            current_node = current_node->GetRight(version);
            son_field = NodeField::right;

        }

        if( key < current_node->GetKey(version)) {

            if(current_node->GetLeft(version) == nullptr) return; //Nó não está na arvore

            path.push_back(current_node);
            current_node = current_node->GetLeft(version);
            son_field = NodeField::left;

        }

        if(key == current_node->GetKey(version)) {
            found_node = true;
            break;
        }
        
    }while(true);

    if(!found_node) return; //Não encontrou nenhum nó com valor de chave, não vamos atualizar a versão da árvore
    tree->last_version += 1;
    version += 1;

    if(current_node->GetLeft(version) == nullptr && current_node->GetRight(version) == nullptr){
        //Se é folha

        if(path.size() == 0) {
            //Se é raiz
            tree->roots[tree->last_version] = nullptr;
            return;
        }

        Node* node_parent = path.back();
        path.pop_back();
        UpdateNode(node_parent, tree, son_field, NONE, nullptr, path);
    }

    else if(current_node->GetLeft(version) == nullptr){
        //Só tem o filho direito

        if(path.size() == 0){
            //Se raiz
            tree->roots[version] = current_node->GetRight(version);
            return;
        }

        Node* node_parent = path.back();
        path.pop_back();
        UpdateNode(node_parent, tree, son_field, NONE, current_node->GetRight(version), path);

    }else if(current_node->GetRight(version) == nullptr){
        //Só tem o filho esquerdo
        if(path.size() == 0){
            //Se raiz
            tree->roots[version] = current_node->GetLeft(version);
            return;
        }

        Node* node_parent = path.back();
        path.pop_back();
        UpdateNode(node_parent, tree, son_field, NONE, current_node->GetLeft(version), path);

    }else{
        //Tem dois filhos
        //Por limitação no quantidade de campos de Modification, é 
        //Necessário tirar dois nós (o a ser removido e o da menor folha de sua subarvore direita)
        //E colocar um novo em sua posição com a mesma chave que o menor folha de sua subarvore direita
        vector<Node*> smallest_path = FindPathToSmallest(current_node->GetRight(version), tree->last_version);

        if(smallest_path.size() == 1){

            Node* new_current_node = UpdateNode(current_node, tree, NodeField::right, NONE, smallest_path[0]->GetRight(version), path);
            UpdateNode(new_current_node, tree, NodeField::key, smallest_path[0]->GetKey(version), nullptr, path);

            return;
        }

        Node* smallest = smallest_path.back();
        smallest_path.pop_back();
        Node* second_smallest = smallest_path.back();



        UpdateNode(current_node, tree, NodeField::key, smallest->GetKey(version), nullptr, path);     
        
        vector<Node*> full_path = FindPath(tree, second_smallest->GetKey(version), version);
        full_path.pop_back();

        UpdateNode(second_smallest, tree, NodeField::left, NONE, smallest->GetRight(version), full_path);  
    }
}

int Sucessor(BinarySearchTree* tree, int key, int version){

    if(version > tree->last_version) version = tree->last_version;

    Node * root = tree->roots[version];

    Node* current_node = root;
    int sucessor = INF;

    do {
        //Aqui estou considerando que o sucessor deve ser maior que a chave dada
        if(key >= current_node->GetKey(version)) current_node = current_node->GetRight(version);

        if(key < current_node->GetKey(version)){
            sucessor = current_node->GetKey(version);
            current_node = current_node->GetLeft(version);
        }

    } while(current_node != nullptr);

    return sucessor;
}

void DFS_REC(Node* node, int version, int deepness, vector<pair<int,int>>& dfs_vector) {
        /*Função recursiva auxiliar que será usada para utilizar a DFS sem necessidade de dar 
            profundidade=0 como um dos parametros na função */

        if(node == nullptr) return;
        DFS_REC(node->GetLeft(version), version, deepness + 1, dfs_vector);
        dfs_vector.push_back(pair<int,int>(node->GetKey(version), deepness));
        DFS_REC(node->GetRight(version), version, deepness + 1, dfs_vector);
    
    }

vector<pair<int,int>> DFS(BinarySearchTree* tree, int version){
        /*Função que retorna uma array com o par (Chave, profundidade de um nó) */
        
        if(version > tree->last_version) version = tree->last_version;
        
        Node* root = tree->roots[version];
        vector<pair<int,int>> dfs_vector = {};

        DFS_REC(root, version, 0, dfs_vector);

        return dfs_vector;
}




