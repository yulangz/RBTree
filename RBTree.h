//
// Created by yulan on 2021/3/1.
//

#ifndef RBTREE_RBTREE_H
#define RBTREE_RBTREE_H

#include <iostream>

namespace yulan {
    template<typename T>
    class RBTree {
    private:
        enum NodeColor {
            Red, Black
        };

        // 节点类型
        struct TreeNode {
            T key;
            TreeNode *l_child;
            TreeNode *r_child;
            TreeNode *parent;
            NodeColor color;

            explicit TreeNode(T key_, TreeNode *l_child_ = nullptr,
                              TreeNode *r_child_ = nullptr, TreeNode *parent_ = nullptr)
                    : key(key_),
                      l_child(l_child_),
                      r_child(r_child_),
                      parent(parent_),
                      color(Red) {}
        };

    private:
        TreeNode *root;

    private:
        static inline bool is_red(TreeNode *node) {
            return node && node->color == Red;
        }

        static inline bool is_black(TreeNode *node) {
            return !node || node->color == Black;
        }

        static inline void set_black(TreeNode *node) {
            node->color = Black;
        }

        static inline void set_red(TreeNode *node) {
            node->color = Red;
        }

        /**
         * 把src的颜色染给dest
         * @param dest
         * @param src
         */
        static inline void copy_color(TreeNode *dest, TreeNode *src) {
            dest->color = src->color;
        }

        static inline NodeColor get_color(TreeNode *node) {
            return node->color;
        }

        /**
         * 值复制
         * @param dest
         * @param src
         */
        static inline void copy_element(TreeNode *dest, TreeNode *src) {
            dest->key = src->key;
        }


        /**
         * 中序遍历打印辅助函数
         * @param node
         */
        void inOrderPrint_(TreeNode *node);

        /**
         * 查找key
         * @param key
         * @return 有key的指针，key不存在返回空
         */
        TreeNode *search(T key);

        /**
         * 右旋
         * @param node
         */
        void rRotate(TreeNode *node);

        /**
         * 左旋
         * @param node
         */
        void lRotate(TreeNode *node);

        /**
         * 更新时，双红修正
         * @param node
         */
        void updateFixup(TreeNode *node);

        /**
         * 删除时，双黑修正
         * @param node 替代被删除节点的节点
         * @param parent node的父亲
         */
        void deleteFixup(TreeNode *node, TreeNode *parent);

        /**
         * 先序遍历打印树
         * @param node
         */
        void travelPrintTree(TreeNode *node);

        void destroy(TreeNode *node);

    public:
        RBTree() : root(nullptr) {}

        ~RBTree() {
            destroy(root);
        }

    public:
        bool exist(T key);

        /**
         * 插入key，若key存在则什么也不做
         * @param key
         */
        void insert(T key);

        /**
         * 删除key
         * @param key
         */
        void remove(T key);


        /**
         * 中序遍历打印
         */
        void inOrderPrint();

        /**
         * 打印树
         */
        void printTree() {
            travelPrintTree(root);
        }
    };


    template<typename T>
    void RBTree<T>::inOrderPrint_(RBTree::TreeNode *node) {
        if (!node) return;
        inOrderPrint_(node->l_child);
        std::cout << node->key << " ";
        inOrderPrint_(node->r_child);
    }

    template<typename T>
    void RBTree<T>::inOrderPrint() {
        inOrderPrint_(root);
        std::cout << std::endl;
    }

    template<typename T>
    typename RBTree<T>::TreeNode *RBTree<T>::search(T key) {
        TreeNode *p = root;
        while (p) {
            if (p->key == key)
                return p;
            else if (key < p->key)
                p = p->l_child;
            else
                p = p->r_child;
        }
        return nullptr;
    }

    template<typename T>
    bool RBTree<T>::exist(T key) {
        return search(key) != nullptr;
    }

    template<typename T>
    void RBTree<T>::lRotate(RBTree::TreeNode *node) {
        TreeNode *rChild = node->r_child;

        node->r_child = rChild->l_child;
        if (rChild->l_child)
            rChild->l_child->parent = node;

        rChild->l_child = node;

        if (node == root)
            root = rChild;
        else {
            if (node->parent->l_child == node)
                node->parent->l_child = rChild;
            else
                node->parent->r_child = rChild;
        }

        rChild->parent = node->parent;
        node->parent = rChild;
    }


    template<typename T>
    void RBTree<T>::rRotate(RBTree::TreeNode *node) {
        TreeNode *lChild = node->l_child;

        node->l_child = lChild->r_child;
        if (lChild->r_child)
            lChild->r_child->parent = node;

        lChild->r_child = node;

        if (node == root)
            root = lChild;
        else {
            if (node->parent->l_child == node)
                node->parent->l_child = lChild;
            else
                node->parent->r_child = lChild;
        }

        lChild->parent = node->parent;
        node->parent = lChild;
    }

    template<typename T>
    void RBTree<T>::updateFixup(RBTree::TreeNode *node) {
        if (node == root) {
            set_black(node);
            return;
        }

        TreeNode *parent;
        TreeNode *gran_parent;
        TreeNode *uncle;

        while ((parent = node->parent) && is_red(parent)) {
            gran_parent = parent->parent;   // parent为红，一定不是root

            // 父亲是祖父的左孩子
            if (parent == gran_parent->l_child) {
                uncle = gran_parent->r_child;

                // 三红
                if (is_red(uncle)) {
                    set_black(parent);
                    set_black(uncle);
                    set_red(gran_parent);

                    // 以祖父为当前节点继续递归
                    node = gran_parent;
                    continue;
                }

                    // 红红黑
                else {

                    // 左右情形，需要先进行一次左旋
                    if (node == parent->r_child) {
                        lRotate(parent);
                        parent = node;// parent会被染成黑色
                    }

                    rRotate(gran_parent);

                    // 染色
                    set_red(gran_parent);
                    set_black(parent);
                    break;
                }
            }

                // 父亲是祖父的右孩子
            else {
                uncle = gran_parent->l_child;

                // 三红
                if (is_red(uncle)) {
                    set_black(parent);
                    set_black(uncle);
                    set_red(gran_parent);

                    // 以祖父为当前节点继续递归
                    node = gran_parent;
                    continue;
                }

                    // 红红黑
                else {

                    // 右左情形，需要先进行一次右旋
                    if (node == parent->l_child) {
                        rRotate(parent);
                        parent = node;// parent会被染成黑色
                    }

                    lRotate(gran_parent);

                    // 染色
                    set_red(gran_parent);
                    set_black(parent);
                    break;
                }
            }
        }

        set_black(root);
    }

    template<typename T>
    void RBTree<T>::insert(T key) {
        // 空树的处理
        if (!root) {
            root = new TreeNode(key);
            set_black(root);
            return;
        }

        // 寻找插入位置
        TreeNode *node = root;
        TreeNode *parent;
        while (node) {
            parent = node;
            if (node->key == key)
                return;
            else if (key < node->key)
                node = node->l_child;
            else
                node = node->r_child;
        }

        if (key < parent->key) {
            parent->l_child = new TreeNode(key, nullptr, nullptr, parent);
            updateFixup(parent->l_child);
        } else {
            parent->r_child = new TreeNode(key, nullptr, nullptr, parent);
            updateFixup(parent->r_child);
        }
    }

    template<typename T>
    void RBTree<T>::remove(T key) {
        TreeNode *node = search(key);
        if (node == nullptr)
            return;

        TreeNode *parent;
        TreeNode *replacer;

        // 有两个孩子
        if (node->l_child != nullptr && node->r_child != nullptr) {

            // 找到直接后继
            TreeNode *successor;
            successor = node->r_child;
            while (successor->l_child)
                successor = successor->l_child;

            // 值复制
            copy_element(node, successor);
            node = successor;
        }

        // 单子节点或无子节点
        if (node->l_child) {
            replacer = node->l_child;
        } else
            replacer = node->r_child;

        // 把父节点续上，隔离出node
        parent = node->parent;
        if (parent) {
            if (node == parent->l_child)
                parent->l_child = replacer;
            else
                parent->r_child = replacer;
        } else {
            root = replacer;
        }

        if (replacer)
            replacer->parent = parent;

        if (is_black(node))
            deleteFixup(replacer, parent);

        delete node;
    }

    template<typename T>
    void RBTree<T>::deleteFixup(RBTree::TreeNode *node, RBTree::TreeNode *parent) {

        while ((node != root) && is_black(node)) {
            if (parent->l_child == node) {
                TreeNode *brother = parent->r_child;  // 被删除的节点是黑节点才会进入此函数，黑节点一定有兄弟

                // 情形1，兄弟是红色
                if (is_red(brother)) {
                    set_red(parent);
                    set_black(brother);
                    lRotate(parent);
                    brother = parent->r_child;
                }
                // 旋转前brother是红色，其子节点一定是黑色，而旋转后node的兄弟是brother的左儿子
                // 故经过选择之后的brother一定是黑色


                // 情形2，brother是黑色，且其两个子节点均为黑色
                if (is_black(brother->l_child) && is_black(brother->r_child)) {
                    set_red(brother);
                    node = parent;
                    if (node)
                        parent = node->parent;
                } else {

                    // 情形3，brother是黑色，且brother的儿子左红右黑
                    if (is_red(brother->l_child) && is_black(brother->r_child)) {
                        set_black(brother->l_child);
                        set_red(brother);
                        rRotate(brother);
                        brother = parent->r_child;
                    }
                    // 转换后brother的右儿子一定是红色

                    // 情形4，brother是黑色，且其右儿子是红色
                    copy_color(brother, parent);
                    set_black(parent);  // 交换parent与brother颜色
                    set_black(brother->r_child);
                    lRotate(parent);
                    break;
                }

            } else {

                // 对称
                TreeNode *brother = parent->l_child;  // 被删除的节点是黑节点才会进入此函数，黑节点一定有兄弟

                // 情形1，
                if (is_red(brother)) {
                    set_red(parent);
                    set_black(brother);
                    rRotate(parent);
                    brother = parent->l_child;
                }

                // 情形2，
                if (is_black(brother->l_child) && is_black(brother->r_child)) {
                    set_red(brother);
                    node = parent;
                    if (node)
                        parent = node->parent;
                } else {

                    // 情形3，
                    if (is_red(brother->r_child) && is_black(brother->l_child)) {
                        set_black(brother->r_child);
                        set_red(brother);
                        lRotate(brother);
                        brother = parent->l_child;
                    }


                    // 情形4，
                    copy_color(brother, parent);
                    set_black(parent);
                    set_black(brother->l_child);
                    rRotate(parent);
                    break;
                }
            }
        }

        if (node)
            set_black(node);
    }

    template<typename T>
    void RBTree<T>::travelPrintTree(RBTree::TreeNode *node) {
        if (node) {
            if (node == root)    // tree是根节点
                std::cout << node->key << "(B) is root";
            else                // tree是分支节点
                std::cout << node->key << (is_red(node) ? "(R)" : "(B)");

            std::cout << "\t左孩子:" << ((node->l_child) ? node->l_child->key : 0)
                      << (is_red(node->l_child) ? "(R)" : "(B)");
            std::cout << "\t右孩子:" << ((node->r_child) ? node->r_child->key : 0)
                      << (is_red(node->r_child) ? "(R)" : "(B)");
            std::cout << std::endl;
            travelPrintTree(node->l_child);
            travelPrintTree(node->r_child);
        }
    }

    template<typename T>
    void RBTree<T>::destroy(RBTree::TreeNode *node) {
        if (node == nullptr)
            return;
        destroy(node->l_child);
        destroy(node->r_child);
        delete node;
    }
}


#endif //RBTREE_RBTREE_H
