#pragma once

#include <cstddef>
#include <optional>
#include <vector>
#include <set>

template <typename K, typename V>
class BSTMap {
public:
    BSTMap();
    ~BSTMap();

    BSTMap(const BSTMap&) = delete;
    BSTMap& operator=(const BSTMap&) = delete;

    void put(const K& key, const V& value);
    const V* get(const K& key) const;
    bool containsKey(const K& key) const;

    std::size_t size() const;
    void clear();

    std::optional<V> remove(const K& key);
    std::set<K> keySet() const;
    std::vector<K> inorder() const;

private:
    struct Node {
        K key;
        V value;
        Node* left;
        Node* right;
    };

    Node* root_;
    std::size_t size_;

    void put(const K& key, const V& value, Node*& node);
    const Node* find(const K& key, const Node* node) const;
    void clear(Node* node);
    
    std::optional<V> remove(const K& key, Node*& node);
    void inorder(const Node* node, std::vector<K>& result) const;
    const Node* get_min(const Node* node) const;
};

template <typename K, typename V>
BSTMap<K, V>::BSTMap(): root_(nullptr), size_(0){}

template <typename K, typename V>
BSTMap<K, V>::~BSTMap(){
    clear(root_);
}


template <typename K, typename V>
void BSTMap<K, V>::put(const K& key, const V& value){
    put(key, value, root_);
}

template <typename K, typename V>
const V* BSTMap<K, V>::get(const K& key) const{
    const Node* node = find(key, root_);
    if(node == nullptr)
        return nullptr;
    return &node->value;
}

template <typename K, typename V>
bool BSTMap<K, V>::containsKey(const K& key) const{
    return find(key, root_) != nullptr;
}

template <typename K, typename V>
std::size_t BSTMap<K, V>::size() const{
    return size_;
}

template <typename K, typename V>
void BSTMap<K, V>::clear(){
    clear(root_);
    root_ = nullptr;
    size_ = 0;
}

template <typename K, typename V>
void BSTMap<K, V>::put(const K& key, const V& value, Node*& node){
    if(node == nullptr){
        node = new Node{key, value, nullptr, nullptr};
        ++size_;
        return;
    }
    if(key < node->key)
        put(key, value, node->left);
    else if(node->key < key)
        put(key, value, node->right); 
    else
        node->value = value;
}

template <typename K, typename V>
const typename BSTMap<K, V>::Node* BSTMap<K, V>::find(const K& key, const Node* node) const{
    if(node == nullptr)
        return nullptr;
    if(key < node->key)
        return find(key, node->left);
    else if(node->key < key)
        return find(key, node->right);
    else return node;
}

template <typename K, typename V>
void BSTMap<K, V>::clear(Node* node){
    if(node == nullptr)
        return;
    clear(node->left);
    clear(node->right);
    delete node;
}

//optional
template <typename K, typename V>
std::optional<V> BSTMap<K, V>::remove(const K& key){
    return remove(key, root_);
}

template <typename K, typename V>
std::set<K> BSTMap<K, V>::keySet() const{
    std::vector<K> keys = inorder();
    return std::set<K>(keys.begin(), keys.end());
}

template <typename K, typename V>
std::vector<K> BSTMap<K, V>::inorder() const{
    std::vector<K> result;
    inorder(root_, result);
    return result;
}

template <typename K, typename V>
std::optional<V> BSTMap<K, V>::remove(const K& key, Node*& node){
    if(node == nullptr)
        return std::nullopt;
    if(key < node->key)
        return remove(key, node->left);
    else if(node->key < key)
        return remove(key, node->right);
    else{
        V value = node->value;
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
            K new_key = get_min(node->right)->key;
            V new_value = get_min(node->right)->value;
            node->key = new_key;
            node->value = new_value;
            remove(new_key, node->right);
        }
        return value;
    }
}

template <typename K, typename V>
void BSTMap<K, V>::inorder(const Node* node, std::vector<K>& result) const{
    if(node == nullptr) return;
    inorder(node->left, result);
    result.push_back(node->key);
    inorder(node->right, result);
}


template <typename K, typename V>
const typename BSTMap<K, V>::Node* BSTMap<K, V>::get_min(const Node* node) const{
    if (node == nullptr) return nullptr;
    if(node->left == nullptr) return node;
    return get_min(node->left);
}