#pragma once

#include <cstddef>
#include <vector>



class UnionFind{
public:
    using Id = std::size_t;
    using Size = std::size_t;

    explicit UnionFind(Id n);
    
    Id find(Id x);
    void unite(Id a, Id b);
    bool connected(Id a, Id b);

    Size size_of(Id x);
private:
    std::vector<Id> parent_;
    std::vector<Size> size_;
};

inline UnionFind::UnionFind(Id n): parent_(n), size_(n,1){
    for(Id i = 0; i < n; ++i) parent_[i] = i;
}

inline UnionFind::Id UnionFind::find(Id x){
    if (parent_[x] != x) parent_[x] = find(parent_[x]);
    return parent_[x];
}

inline void UnionFind::unite(Id a, Id b){
    Id root_a = find(a);
    Id root_b = find(b);

    if(root_a == root_b) return;

    if(size_[root_a] > size_[root_b]) {
        parent_[root_b] = root_a;
        size_[root_a] += size_[root_b];
    }
    else{
        parent_[root_a] = root_b;
        size_[root_b] += size_[root_a];
    }
}

inline bool UnionFind::connected(Id a, Id b){
    return find(a) == find(b);
}

inline UnionFind::Size UnionFind::size_of(Id x) {
    return size_[find(x)];
}