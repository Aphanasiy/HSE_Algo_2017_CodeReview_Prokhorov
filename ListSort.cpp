#include <iostream>
#include <iostream>
#include <fstream>
#include <utility>

template <typename D>
struct Node {
    D me;
    Node * next = nullptr;
    Node(D _me)
    : me(_me)
    {}
    ~Node(){}
};

template <typename T>
class List {
    Node<T> * first = nullptr;
    Node<T> * last = nullptr;
  public:
    size_t sz = 0;
    List() 
    {};

    List(const List<T>& oth) {
        clear();
        Node<T> * nxt = oth.first;
        while (nxt != nullptr) {
            push(nxt->me);
            nxt = nxt->next;
        }
    }

    List<T>& operator = (const List<T>& oth) {
        clear();
        Node<T> * nxt = oth.first;
        while (nxt != nullptr) {
            push(nxt->me);
            nxt = nxt->next;
        }   
        return *this;
    }

    List<T>& operator = (const List<T>&& oth) {
        clear();
        Node<T> * nxt = oth.first;
        while (nxt != nullptr) {
            push(nxt->me);
            nxt = nxt->next;
        }   
        return *this;
    }

    int front() const {
        return first->me;
    }
    int back() const {
        return last->me;
    }
    size_t size() const{
        return sz;
    }
    bool empty() const {
        return sz == 0;
    }
    void clear() {
        while (!empty()) {
            pop();
        }
    }

    void push(T x) {
        Node<T> * new_last = new Node<T>(x);
        if (size() == 0) {
            first = new_last;
            last = new_last;
        } else {
            last->next = new_last;
            last = last->next;
        }
        ++sz;
    }
    void pop() {
        Node<T> * delete_me = first;
        first = first->next;
        delete delete_me;
        --sz;
        if (sz == 0) {
            last = nullptr;
        }
    }

    friend std::ostream & operator <<(std::ostream & out, const List& l)
    {
        Node<T> * nxt = l.first;
        while (nxt != nullptr) {
            out << nxt->me << ' ';
            nxt = nxt->next;
        }
        return out;
    }

    template <typename TT>
    friend void divide(List<TT>& me, List<TT>& left, List<TT>& right);
    ~List() {
        clear();
    }
};

template <typename T>
void merge(List<T>& merged, List<T>& left, List<T>& right) {
    while (!left.empty() && !right.empty()) {
        if (right.front() < left.front()) {
            merged.push(right.front());
            right.pop();
        } else {
            merged.push(left.front());
            left.pop();
        }
    }
    while (!left.empty()) {
        merged.push(left.front());
        left.pop();
    }
    while (!right.empty()) {
        merged.push(right.front());
        right.pop();
    }
}


template <typename T>
void divide(List<T>& lst, List<T>& l, List<T>& r) {
    size_t sz = lst.size();
    l.first = lst.first;
    r.last = lst.last;
    Node<T> * me = l.first;
    for (size_t i = 1; i < sz / 2; ++i) {
        me = me->next;
    }
    l.last = me;
    r.first = me->next;
    me->next = nullptr;
    l.sz = sz / 2;
    r.sz = sz - sz / 2;
    lst.first = nullptr;
    lst.last = nullptr;
    lst.sz = 0;
    return;
}


template <typename T>
void mergesort(List<T>& lst) {
    if (lst.size() == 1) {
        return;
    }
    List<T> even;
    List<T> odd;
    divide(lst, even, odd);
    mergesort(even);
    mergesort(odd);
    merge(lst, even, odd);
}


int main() {
    size_t n = 0;
    std::cin >> n;
    List<int> lst;
    for (size_t i = 0; i < n; ++i) {
        int x = 0;
        std::cin >> x;
        lst.push(x);
    }
    mergesort(lst);
    std::cout << lst << std::endl;
}

