#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

template <typename T>
class DLList{
private:
    struct Node{
        T value;
        Node* prev;
        Node* next;

        Node(): value(T{}), prev(this), next(this){}
        Node(const T& value, Node* prev, Node* next ): value(value), prev(prev), next(next) {}        
    };

    Node* sentinel_;
    std::size_t size_;

    Node* get_recursive_helper(Node* cur, std::size_t index);
    const Node* get_recursive_helper(const Node* cur, std::size_t index) const;
public:
    DLList();
    ~DLList();

    void push_front(const T& value);
    void push_back(const T& value);

    T pop_front();
    T pop_back();
    T& front();
    const T& front() const;
    T& back();
    const T& back() const;

    T& get(std::size_t index);
    const T& get(std::size_t index) const;

    T& get_recursive(std::size_t index);
    const T& get_recursive(std::size_t index) const;

    std::vector<T> to_vector() const;

    std::size_t size() const;
    bool empty() const;

    DLList(const DLList&) = delete;
    DLList& operator=(const DLList&) = delete;


};

template <typename T>
DLList<T>::DLList(): sentinel_(new Node()), size_(0){}

template <typename T>
DLList<T>::~DLList(){
    Node* cur = sentinel_->next;
    while (cur != sentinel_) {
        Node* next = cur->next;
        delete cur;
        cur = next;
    }
    delete sentinel_;
}

template <typename T>
void DLList<T>::push_front(const T& value){
    sentinel_->next = new Node(value, sentinel_, sentinel_->next);
    sentinel_->next->next->prev = sentinel_->next;
    ++size_;
}

template <typename T>
void DLList<T>::push_back(const T& value){
    sentinel_->prev->next = new Node(value, sentinel_->prev, sentinel_);
    sentinel_->prev = sentinel_->prev->next;
    ++size_;
}

template <typename T>
T DLList<T>::pop_front(){
    if (sentinel_->next == sentinel_)
        throw std::out_of_range("DLList::pop_front() called on empty list");

    Node* cur = sentinel_->next;
    sentinel_->next = cur->next;
    sentinel_->next->prev = sentinel_;
    T value = cur->value;
    delete cur;
    --size_;
    return value;
}

template <typename T>
T DLList<T>::pop_back(){
    if (sentinel_->prev == sentinel_)
        throw std::out_of_range("DLList::pop_back() called on empty list");

    Node* cur = sentinel_->prev;
    sentinel_->prev = cur->prev;
    sentinel_->prev->next = sentinel_;
    T value = cur->value;
    delete cur;
    --size_;
    return value;
}

template <typename T>
T& DLList<T>::front(){
    if (sentinel_->next == sentinel_)
        throw std::out_of_range("DLList::front() called on empty list");

    return sentinel_->next->value;
}

template <typename T>
const T& DLList<T>::front() const{
    if (sentinel_->next == sentinel_)
        throw std::out_of_range("DLList::front() called on empty list");

    return sentinel_->next->value;
}

template <typename T>
T& DLList<T>::back(){
    if(sentinel_->prev == sentinel_)
        throw std::out_of_range("DLList::back() called on empty list");
    return sentinel_->prev->value;
}

template <typename T>
const T& DLList<T>::back() const{
    if(sentinel_->prev == sentinel_)
        throw std::out_of_range("DLList::back() called on empty list");
    return sentinel_->prev->value;
}

template <typename T>
T& DLList<T>::get(std::size_t index){
    if (index >= size_)   
        throw std::out_of_range("DLList::get() index out of range");
    Node* cur = sentinel_->next;
    for(std::size_t i = 0; i < index; ++i) cur = cur->next;
    return cur->value;
}

template <typename T>
const T& DLList<T>::get(std::size_t index) const{
    if (index >= size_)   
        throw std::out_of_range("DLList::get() index out of range");
    const Node* cur = sentinel_->next;
    for(std::size_t i = 0; i < index; ++i) cur = cur->next;
    return cur->value; 
}

template <typename T>
typename DLList<T>::Node* 
DLList<T>::get_recursive_helper(Node* cur, std::size_t index){
    if(index == 0) return cur;
    else return get_recursive_helper(cur->next, index - 1);
}

template <typename T>
const typename DLList<T>::Node* 
DLList<T>::get_recursive_helper(const Node* cur, std::size_t index) const {
    if(index == 0) return cur;
    else return get_recursive_helper(cur->next, index - 1);
}

template <typename T>
T& DLList<T>::get_recursive(std::size_t index){
    if(index >= size_)
        throw std::out_of_range("DLList::get_recursive() index out of range");
    return get_recursive_helper(sentinel_->next, index)->value;
}

template <typename T>
const T& DLList<T>::get_recursive(std::size_t index) const{
    if(index >= size_)
        throw std::out_of_range("DLList::get_recursive() index out of range");
    return get_recursive_helper(sentinel_->next, index)->value;
}

template <typename T>
std::vector<T> DLList<T>::to_vector() const{
    std::vector<T> result;
    result.reserve(size_);
    const Node* cur = sentinel_->next;
    while(cur != sentinel_){
        result.push_back(cur->value);
        cur = cur->next;
    }
    return result;
}

template <typename T>
std::size_t DLList<T>::size() const{
    return size_;
}

template <typename T>
bool DLList<T>::empty() const{
    return size_ == 0;
}