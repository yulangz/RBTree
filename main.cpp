#include <iostream>
#include <vector>
#include "RBTree.h"

int main()
{
    yulan::RBTree<int> tree;
    std::vector<int> a = {10, 40, 30, 60, 90, 70, 20, 50, 80};
    for (auto i:a)
        tree.insert(i);

    tree.printTree();

    for(int i:a)
    {
        tree.remove(i);

        std::cout << "== 删除节点: " << i << std::endl;
//        cout << "== 树的中序遍历: " << endl;
        tree.printTree();
        std::cout << std::endl;
//        cout << endl;
    }
}