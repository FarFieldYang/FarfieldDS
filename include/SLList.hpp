#pragma once

#include <cstddef>
#include <stdexcept>

template <typename T>
class SLList{
private:
    struct Node{
        T value;
        Node* next;

        Node(const T& value, Node* next = nullptr): value(value), next(next) {}        
    };

    Node* sentinel_;
    std::size_t size_;
public:
    SLList();
    explicit SLList(const T& value);
    ~SLList();

    void push_front(const T& value);
    void push_back(const T& value);

    T pop_front();
    T pop_back();
    T& front();
    const T& front() const;

    std::size_t size() const;

    SLList(const SLList&) = delete;
    SLList& operator=(const SLList&) = delete;
};

template <typename T>
SLList<T>::SLList(): sentinel_(new Node(T{})), size_(0){}

template <typename T>
SLList<T>::SLList(const T& value): sentinel_(new Node(T{})), size_(1){
    sentinel_->next = new Node(value);
}

template <typename T>
SLList<T>::~SLList(){
    Node* cur = sentinel_;
    while (cur != nullptr) {
        Node* next = cur->next;
        delete cur;
        cur = next;
    }
}

template <typename T>
void SLList<T>::push_front(const T& value){
    sentinel_->next = new Node(value, sentinel_->next);
    ++size_;
}

template <typename T>
void SLList<T>::push_back(const T& value){
    Node* cur = sentinel_;
    while(cur->next != nullptr) cur = cur->next;
    cur->next = new Node(value);
    ++size_;
}
template <typename T>
T SLList<T>::pop_front(){
    if (sentinel_->next == nullptr)
        throw std::out_of_range("SLList::pop_front() called on empty list");

    Node* cur = sentinel_->next;
    sentinel_->next = cur->next;
    T value = cur->value;
    delete cur;
    --size_;
    return value;
}

template <typename T>
T SLList<T>::pop_back(){
    if (sentinel_->next == nullptr)
        throw std::out_of_range("SLList::pop_back() called on empty list");

    Node* cur = sentinel_;
    Node* tmp = cur;
    while(cur->next != nullptr){
        tmp = cur;
        cur = cur->next;
    } 
    T value = cur->value;
    delete cur;
    tmp->next = nullptr;
    --size_;
    return value;
}

template <typename T>
T& SLList<T>::front(){
    if (sentinel_->next == nullptr)
        throw std::out_of_range("SLList::front() called on empty list");

    return sentinel_->next->value;
}

template <typename T>
const T& SLList<T>::front() const{
    if (sentinel_->next == nullptr)
        throw std::out_of_range("SLList::front() called on empty list");

    return sentinel_->next->value;
}

template <typename T>
std::size_t SLList<T>::size() const{
    return size_;
}