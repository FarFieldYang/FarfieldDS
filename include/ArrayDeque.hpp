#pragma once

#include <cstddef>
#include <stdexcept>
#include <vector>

template <typename T>
class ArrayDeque {
public:
    ArrayDeque();
    ~ArrayDeque();

    ArrayDeque(const ArrayDeque&) = delete;
    ArrayDeque& operator=(const ArrayDeque&) = delete;

    void push_front(const T& item);
    void push_back(const T& item);

    T pop_front();
    T pop_back();

    T& front();
    T& back();

    const T& front() const;
    const T& back() const;

    T& get(std::size_t index);
    const T& get(std::size_t index) const;

    std::size_t size() const;
    bool empty() const;

    std::vector<T> to_vector() const;

private:
    T* data_;
    std::size_t size_;
    std::size_t capacity_;
    std::size_t front_;

    static constexpr std::size_t INIT_CAPACITY = 8;

    void resize(std::size_t new_capacity);

    std::size_t physical_index(std::size_t logical_index) const;

};

template <typename T>
ArrayDeque<T>::ArrayDeque(): data_(new T[INIT_CAPACITY]), size_(0), capacity_(INIT_CAPACITY), front_(0){}

template <typename T>
ArrayDeque<T>::~ArrayDeque(){
    delete[] data_;
}

template <typename T>
void ArrayDeque<T>::push_front(const T& item){
    if (size_ == capacity_) resize(2 * capacity_);
    front_ = (front_ + capacity_ - 1) % capacity_;
    data_[front_] = item;
    ++size_;
}

template <typename T>
void ArrayDeque<T>::push_back(const T& item){
    if (size_ == capacity_) resize(2 * capacity_);
    data_[physical_index(size_)] = item;
    ++size_;
}

template <typename T>
T ArrayDeque<T>::pop_front(){
    if (empty())
        throw std::out_of_range("ArrayDeque is empty");

    T value = data_[front_];
    front_ = (front_ + 1) % capacity_;
    --size_;
    if(size_ * 4 <= capacity_ && capacity_ > INIT_CAPACITY)
        resize(static_cast<float>(capacity_) / 2);
    return value;
}

template <typename T>
T ArrayDeque<T>::pop_back(){
    if (empty())
        throw std::out_of_range("ArrayDeque is empty");

    T value = data_[physical_index(size_ - 1)];
    --size_;
    if(size_ * 4 <= capacity_ && capacity_ > INIT_CAPACITY)
        resize(static_cast<float>(capacity_) / 2);
    return value;
}

template <typename T>
T& ArrayDeque<T>::front(){
    if (empty())
        throw std::out_of_range("ArrayDeque is empty");

    return data_[front_];
}

template <typename T>
T& ArrayDeque<T>::back(){
    if (empty())
        throw std::out_of_range("ArrayDeque is empty");

    return data_[physical_index(size_ - 1)];
}

template <typename T>
const T& ArrayDeque<T>::front() const{
    if (empty())
        throw std::out_of_range("ArrayDeque is empty");

    return data_[front_];
}
template <typename T>
const T& ArrayDeque<T>::back() const{
    if (empty())
        throw std::out_of_range("ArrayDeque is empty");

    return data_[physical_index(size_ - 1)];
}

template <typename T>
T& ArrayDeque<T>::get(std::size_t index){
    if (index >= size_)
        throw std::out_of_range("ArrayDeque index out of range");

    return data_[physical_index(index)];
}

template <typename T>
const T& ArrayDeque<T>::get(std::size_t index) const{
    if (index >= size_)
        throw std::out_of_range("ArrayDeque index out of range");
        
    return data_[physical_index(index)];
}

template <typename T>
std::size_t ArrayDeque<T>::size() const{
    return size_;
}

template <typename T>
bool ArrayDeque<T>::empty() const{
    return size_ == 0;
}

template <typename T>
std::vector<T> ArrayDeque<T>::to_vector() const{
    std::vector<T> result(size_);
    for (std::size_t i = 0; i < size_; ++i){
        result[i] = get(i);
    }
    return result;
}

template <typename T>
void ArrayDeque<T>::resize(std::size_t new_capacity){
    T* new_data = new T[new_capacity];
    for (std::size_t i = 0; i < size_; ++i){
        new_data[i] = get(i);
    }
    front_ = 0;
    delete[] data_;
    data_ = new_data;
    capacity_ = new_capacity;
}

template <typename T>
std::size_t ArrayDeque<T>::physical_index(std::size_t logical_index) const{
    return (front_  + logical_index) % capacity_;
}