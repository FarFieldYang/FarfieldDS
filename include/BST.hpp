#pragma once

#include <cstddef>
#include <vector>


template <typename T>
class BST {
public:
    BST();
    ~BST();

    bool empty() const;
    std::size_t size() const;

    void insert(const T& value);
    bool contains(const T& value) const;
    void remove(const T& value);

    void clear();

    std::vector<T> inorder() const;

    BST(const BST&) = delete;
    BST& operator=(const BST&) = delete;
private:
    struct Node {
        T value;
        Node* left;
        Node* right;
    };

    Node* root_;
    std::size_t size_;

    void insert(const T& value, Node*& node);
    bool contains(const T& value, const Node* node) const;
    void remove(const T& value, Node*& node);

    void clear(Node* node);

    void inorder(const Node* node, std::vector<T>& result) const;

    const Node* get_min(const Node* node) const;
};

template <typename T>
BST<T>::BST(): root_(nullptr), size_(0){}

template <typename T>
BST<T>::~BST(){
    clear(root_);
}

template <typename T>
bool BST<T>::empty() const{
    return root_ == nullptr;
}

template <typename T>
std::size_t BST<T>::size() const{
    return size_;
}

template <typename T>
void BST<T>::insert(const T& value){
    insert(value, root_);
}

template <typename T>
bool BST<T>::contains(const T& value) const{
    return contains(value, root_);
}

template <typename T>
void BST<T>::remove(const T& value){
    remove(value, root_);
}

template <typename T>
void BST<T>::clear(){
    clear(root_);
    root_ = nullptr;
    size_ = 0;
}

template <typename T>
std::vector<T> BST<T>::inorder() const{
    std::vector<T> result;
    inorder(root_, result);
    return result;
}

template <typename T>
void BST<T>::insert(const T& value, Node*& node){
    if(node == nullptr){
        node = new Node{value, nullptr, nullptr};
        ++size_;
        return;
    }
    if(value < node->value)
        insert(value, node->left);
    else if(value > node->value)
        insert(value, node->right);
}

template <typename T>
bool BST<T>::contains(const T& value, const Node* node) const{
    if(node == nullptr)
        return false;
    if(value < node->value)
        return contains(value, node->left);
    else if(value > node->value)
        return contains(value, node->right);
    else return true;
}

template <typename T>
void BST<T>::remove(const T& value, Node*& node){
    if(node == nullptr)
        return;
    if(value < node->value)
        remove(value, node->left);
    else if(value > node->value)
        remove(value, node->right);
    else{
        if(node->left == nullptr && node->right == nullptr){
            delete node;
            node = nullptr;
            --size_;
        }
        else if(node->left == nullptr){
            Node* tmp = node->right;
            delete node;
            node = tmp;
            --size_;
        }
        else if(node->right == nullptr){
            Node* tmp = node->left;
            delete node;
            node = tmp;
            --size_;
        }
        else{
            T new_value = get_min(node->right)->value;
            node->value = new_value;
            remove(new_value, node->right);
        }
    }
}

template <typename T>
void BST<T>::clear(Node* node){
    if(node == nullptr)
        return;
    clear(node->left);
    clear(node->right);
    delete node;
}

template <typename T>
void BST<T>::inorder(const Node* node, std::vector<T>& result) const{
    if(node == nullptr) return;
    inorder(node->left, result);
    result.push_back(node->value);
    inorder(node->right, result);
}

template <typename T>
const typename BST<T>::Node* BST<T>::get_min(const Node* node) const{
    if (node == nullptr) return nullptr;
    if(node->left == nullptr) return node;
    return get_min(node->left);
}