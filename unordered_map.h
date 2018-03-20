#include <list>
#include <algorithm>
#include <vector>
#include <stdexcept>
#include <iostream>


const size_t HashMapStorageSize = 11;

template<class KeyType,
         class ValueType,
         class Hash = std::hash<KeyType> > 
class HashMap {
  private:
    const size_t StorageResizeMultiplier = 2;
    using Key_Value = std::pair<const KeyType, ValueType>;
    using HashMapList = std::list<Key_Value>;
    using HashMapStorage = std::vector<std::vector<typename HashMapList::iterator>>;

    HashMapList iter;
    HashMapStorage storage;
    Hash hasher;
    size_t sz = 0;

  public:
    using iterator = typename HashMapList::iterator;
    using const_iterator = typename HashMapList::const_iterator;
   
    HashMap(Hash _hasher = Hash())
    : hasher(_hasher) {
        storage = HashMapStorage(HashMapStorageSize);
    }
    template <class InpIt>
    HashMap(const InpIt begin,
            const InpIt end,
            Hash _hasher = Hash()) 
    : hasher(_hasher) {
        storage = HashMapStorage(HashMapStorageSize);
        for (auto it = begin; it != end; ++it) {
            size_t key_hash = hasher(it->first) % storage.size();
            iter.push_back(*it);
            ++sz;
            storage[key_hash].push_back(--iter.end());
        }
    }
    HashMap(const std::initializer_list<Key_Value> & init_list,
            Hash _hasher = Hash()) 
        : hasher(_hasher) {
        storage = HashMapStorage(HashMapStorageSize);
        for (auto it : init_list) {
            size_t key_hash = hasher(it.first) % storage.size();
            iter.push_back(it);
            ++sz;
            storage[key_hash].push_back(--iter.end());
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
    void clear(size_t k = HashMapStorageSize) {
        iter.clear();
        storage.clear();
        sz = 0;
        storage.resize(k);
    }
    void rebuild() {
        HashMapList iter2 = iter;
        clear(storage.size() * StorageResizeMultiplier);
        for (auto i : iter2) {
            insert(i);
        }
    }
    void insert(const Key_Value & p) {
        size_t key_hash = hasher(p.first) % storage.size();
        for (auto elem : storage[key_hash]) {
            if (elem->first == p.first) {
                return;
            }
        }
        iter.push_back(p);
        ++sz;
        storage[key_hash].push_back(--iter.end());
        if (StorageResizeMultiplier * sz > storage.size()){
            rebuild();
        }
    }
    void erase(const KeyType & key) {
        size_t key_hash = hasher(key) % storage.size();
        size_t i = 0;
        while (i < storage[key_hash].size() && !(storage[key_hash][i]->first == key)) {
            ++i;
        }
        if (i != storage[key_hash].size()) {
            iter.erase(storage[key_hash][i]);
            --sz;
            storage[key_hash].erase(storage[key_hash].begin() + i);
        }
    }

    const_iterator begin() const {
        return iter.cbegin();
    }
    iterator begin(){
        return iter.begin();
    }
    const_iterator end() const {
        return iter.cend();
    }
    iterator end(){
        return iter.end();
    }

    iterator find(const KeyType& key) {
        size_t key_hash = hasher(key) % storage.size();
        size_t i = 0;
        while (i < storage[key_hash].size() && !(storage[key_hash][i]->first == key)) {
            ++i;
        }
        if (i == storage[key_hash].size()) {
            return end();
        }
        else {
            return storage[key_hash][i];
        }
    }
    const_iterator find(const KeyType& key) const {
        size_t key_hash = hasher(key) % storage.size();
        size_t i = 0;
        while (i < storage[key_hash].size() && !(storage[key_hash][i]->first == key)) {
            ++i;
        }
        if (i == storage[key_hash].size()) {
            return end();
        } else {
            return storage[key_hash][i];
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
        HashMapList hmiter = hm.iter;
        clear(std::max(StorageResizeMultiplier * sz, HashMapStorageSize));
        hasher = hm.hasher;
        for (auto p : hmiter) {
            insert(p);
        }
        return *this;
    }
};