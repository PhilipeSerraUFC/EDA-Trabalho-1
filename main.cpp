#include "parser.hpp"

using namespace std;

const string data_path = "in.txt";

int main(){

    vector<Operation> operations = parser_text(data_path);
    BinarySearchTree* tree = new BinarySearchTree();

    OperateTree(tree, operations);

    return 0;

}