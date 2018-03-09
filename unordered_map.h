#include <list>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <iostream>


const size_t HashMapBaseSize = 11;

template<class KeyType,
         class ValueType,
         class Hash = std::hash<KeyType> > 
class HashMap {
    std::list<std::pair<const KeyType, ValueType>> iter;
    std::vector<std::vector<typename std::list<std::pair<const KeyType, ValueType>>::iterator>> base;
    Hash hasher;
    size_t sz = 0;

  public:

    using iterator = typename std::list<std::pair<const KeyType, ValueType>>::iterator;
    using const_iterator = typename std::list<std::pair<const KeyType, ValueType>>::const_iterator;
   
    HashMap(Hash _hasher = Hash())
    : hasher(_hasher) {
        base = std::vector<std::vector<typename std::list<std::pair<const KeyType, ValueType>>::iterator>>(HashMapBaseSize);
    }
    template <class InpIt>
    HashMap(const InpIt begin,
            const InpIt end,
            Hash _hasher = Hash()) 
    : hasher(_hasher) {
        base = std::vector<std::vector<typename std::list<std::pair<const KeyType, ValueType>>::iterator>>(HashMapBaseSize);
        for (auto it = begin; it != end; ++it) {
            size_t key_hash = hasher(it->first) % base.size();
            iter.push_back(*it);
            ++sz;
            base[key_hash].push_back(--iter.end());
        }
    }
    HashMap(const std::initializer_list<std::pair<const KeyType, ValueType>> & init_list,
            Hash _hasher = Hash()) 
        : hasher(_hasher) {
        base = std::vector<std::vector<typename std::list<std::pair<const KeyType, ValueType>>::iterator>>(HashMapBaseSize);
        for (auto it : init_list) {
            size_t key_hash = hasher(it.first) % base.size();
            iter.push_back(it);
            ++sz;
            base[key_hash].push_back(--iter.end());
        }
    }

    size_t size() const{
        return sz;
    }
    bool empty() const{
        return sz == 0;
    }
    const Hash& hash_function() const {
        return hasher;
    }
    void clear(size_t __k = HashMapBaseSize) {
        iter.clear();
        base.clear();
        sz = 0;
        base.resize(__k);
    }
    void reload_base() {
        if (sz > 2 * base.size()) {
            base.resize(3 * sz);
        }
        for (auto i = iter.begin(); i != iter.end(); ++i) {
            size_t key_hash = hasher(i->first) % base.size();
            base[key_hash].push_back(--iter.end());
        }
    }
    void rebuild() {
        size_t ns = 2 * base.size();
        base.clear();
        base.resize(ns);
        reload_base();
    }
    /*
    void rebuild() {
        std::list<std::pair<const KeyType, ValueType>> iter2 = iter;
        clear(base.size() * 2);
        for (auto i : iter2) {
            insert(i);
        }
    }
    //*/
    void insert(const std::pair<const KeyType, ValueType> & p) {
        //std::cerr << p.first << " + " << p.second << std::endl;
        size_t key_hash = hasher(p.first) % base.size();
        for (auto elem : base[key_hash]) {
            if (elem->first == p.first) {
                return;
            }
        }
        iter.push_back(p);
        ++sz;
        base[key_hash].push_back(--iter.end());
        if (2 * sz > base.size()){
            rebuild();
        }
    }
    void erase(const KeyType & key) {
        size_t key_hash = hasher(key) % base.size();
        size_t i = 0;
        while (i < base[key_hash].size() && !(base[key_hash][i]->first == key)) {
            ++i;
        }
        if (i != base[key_hash].size()) {
            iter.erase(base[key_hash][i]);
            --sz;
            base[key_hash].erase(base[key_hash].begin() + i);
        }
    }

    const_iterator begin() const {
        return iter.cbegin();
    }
    iterator begin() {
        return iter.begin();
    }
    const_iterator end() const {
        return iter.cend();
    }
    iterator end() {
        return iter.end();
    }

    iterator find(const KeyType& key) {
        size_t key_hash = hasher(key) % base.size();
        size_t i = 0;
        while (i < base[key_hash].size() && !(base[key_hash][i]->first == key)) {
            ++i;
        }
        if (i == base[key_hash].size()) {
            return end();
        }
        else {
            return base[key_hash][i];
        }
    }
    const_iterator find(const KeyType& key) const {
        size_t key_hash = hasher(key) % base.size();
        size_t i = 0;
        while (i < base[key_hash].size() && !(base[key_hash][i]->first == key)) {
            ++i;
        }
        if (i == base[key_hash].size()) {
            return end();
        } else {
            return base[key_hash][i];
        }
    }

    ValueType& operator [](const KeyType& key) {
        iterator fnd = find(key);
        if (fnd != end()) {
            return fnd->second;
        } else {
            insert({key, ValueType()});
            return (--iter.end())->second;

        }
    }


    const ValueType& at(const KeyType & key) const{
        const_iterator fnd = find(key);
        if (fnd != end()) {
            return fnd->second;
        } else {
            throw std::out_of_range("There's no such key");
        }
    }

    HashMap& operator= (const HashMap& hm) {
        /*
        hasher = hm.hasher;
        sz = hm.sz;
        iter.clear();
        for (auto p : hm.iter) {
            iter.push_back(p);
        }
        reload_base();
        */
        std::list<std::pair<const KeyType, ValueType>> hmiter = hm.iter;
        clear(std::max(3 * hm.sz, HashMapBaseSize));
        hasher = hm.hasher;
        for (auto p : hmiter) {
            insert(p);
        }
        return *this;
    }
};

