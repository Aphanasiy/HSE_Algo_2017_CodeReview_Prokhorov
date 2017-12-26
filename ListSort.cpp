#include <iostream>
#include <iostream>
#include <fstream>
#include <utility>


struct Node {
    int me = 0;
    Node * next = nullptr;
    Node(int _me)
    : me(_me)
    {}
    ~Node(){}
};


class List {
    Node * first = nullptr;
    Node * last = nullptr;
  public:
    size_t sz = 0;
    List() 
    {};

    List(const List& oth) {
        clear();
        Node * nxt = oth.first;
        while (nxt != nullptr) {
            push(nxt->me);
            nxt = nxt->next;
        }
    }

    List& operator = (const List& oth) {
        clear();
        Node * nxt = oth.first;
        while (nxt != nullptr) {
            push(nxt->me);
            nxt = nxt->next;
        }   
        return *this;
    }

    List& operator = (const List&& oth) {
        clear();
        Node * nxt = oth.first;
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

    void push(int x) {
        Node * new_last = new Node(x);
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
        Node * delete_me = first;
        first = first->next;
        delete delete_me;
        --sz;
        if (sz == 0) {
            last = nullptr;
        }
    }

    friend std::ostream & operator <<(std::ostream & out, const List& l)
    {
        Node * nxt = l.first;
        while (nxt != nullptr) {
            out << nxt->me << ' ';
            nxt = nxt->next;
        }
        return out;
    }

    void divide(List& left, List& right) const {
        bool choose = 0;
        Node * nxt = first;
        while (nxt != nullptr) {
            if (choose) {
                left.push(nxt->me);
            } else {
                right.push(nxt->me);
            }
            nxt = nxt->next;
            choose = !choose;
        }
    }

    ~List() {
        clear();
    }
};


List merge(List left, List right) {
    List merged;
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
    return merged;
}


List mergesort(List const& lst) {
    if (lst.size() == 1) {
        return lst;
    }
    List even;
    List odd;
    lst.divide(even, odd);
    even = mergesort(even);
    odd = mergesort(odd);
    return merge(even, odd);
}


using namespace std;


int main() {
    size_t n = 0;
    cin >> n;
    List lst;
    for (size_t i = 0; i < n; ++i) {
        int x = 0;
        cin >> x;
        lst.push(x);
    }
    lst = mergesort(lst);
    cout << lst << endl;
}

