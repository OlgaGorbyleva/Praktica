#include<iostream>
#include <stack>

template <typename t>
__interface iterator {
    virtual bool hasmore() const = 0;
    virtual t getnext() = 0;
};

template <typename t>
__interface iterablecollection {
    virtual iterator<t>* createiterator() = 0;
};

template <typename t>
struct node {
    t value;
    node<t>* parent = nullptr;
    node<t>* right = nullptr;
    node<t>* left = nullptr;
    node(t val) : value(val) {}
    void shownode() const {
        std::cout << value << ", ";
    }
};

template <typename t>
class binarytree;

template <typename t>
class binarytreeiterator : public iterator<t> {
public:
    binarytreeiterator(binarytree<t>* tree, node<t>* start) {
        this->tree = tree;
        current = start;
    }

    bool hasmore() const override {
        return current != nullptr || !nodestack.empty();
    }

    t getnext() override {
        if (current == nullptr && !nodestack.empty()) {
            current = nodestack.top();
            nodestack.pop();
        }
        else if (current == nullptr && nodestack.empty()) {
            throw "tree is empty";
        }

        t result = current->value;
        if (current->right != nullptr) {
            nodestack.push(current->right);
        }
        current = current->left;
        if (current == nullptr && !nodestack.empty()) {
            current = nodestack.top();
            nodestack.pop();
        }
        return result;
    }

    bool operator!=(const binarytreeiterator<t>& other) const {
        return current != other.current;
    }

    t operator*() const {
        return current->value;
    }

    t* operator->() const {
        return &(current->value);
    }

    binarytreeiterator<t> operator++() {
        getnext();
        return *this;
    }

private:
    binarytree<t>* tree = nullptr;
    std::stack<node<t>*> nodestack;
    node<t>* current = nullptr;
};

template <typename t>
class binarytree : public iterablecollection<t> {
    node<t>* root = nullptr;

public:
    bool isempty() const {
        return root == nullptr;
    }

    binarytree() {}

    ~binarytree() {
        clear();
    }

    node<t>* getroot() const {
        return root;
    }

    void showtree() const {
        std::cout << "\\n";
        showtree(root);
        std::cout << "\\n";
    }

private:
    void showtree(node<t>* element) const {
        if (element != nullptr) {
            showtree(element->left);
            element->shownode();
            showtree(element->right);
        }
    }

public:
    void clear() {
        if (isempty())
            return;

        std::stack<node<t>*> nodestack;
        nodestack.push(root);
        while (!nodestack.empty()) {
            node<t>* current = nodestack.top();
            nodestack.pop();
            if (current->right != nullptr)
                nodestack.push(current->right);
            if (current->left != nullptr)
                nodestack.push(current->left);
            delete current;
        }
        root = nullptr;
    }

    node<t>* findnode(const t& value) const {
        return findnodehelper(root, value);
    }

private:
    node<t>* findnodehelper(node<t>* current, const t& value) const {
        if (current == nullptr || current->value == value)
            return current;
        if (value < current->value)
            return findnodehelper(current->left, value);
        return findnodehelper(current->right, value);
    }

public:
    void addnode(t value) {
        auto n = new node<t>(value);
        node<t>* parent = nullptr;
        auto current = root;

        while (current != nullptr) {
            parent = current;
            if (n->value < current->value)
                current = current->left;
            else
                current = current->right;
        }

        if (parent == nullptr)
            root = n;
        else if (n->value < parent->value)
            parent->left = n;
        else
            parent->right = n;
        n->parent = parent;
    }

    // остальные методы дерева не реализованы по ненадобности

public:
    binarytreeiterator<t>* createiterator() override { // iterator pattern method
        return new binarytreeiterator<t>(this, root);
    }

    // добавление итератора для одномерного массива
public:
    class arrayiterator : public iterator<t> {
    public:
        arrayiterator(t* arr, int size) : array(arr), array_size(size), current_index(0) {}

        bool hasmore() const override {
            return current_index < array_size;
        }

        t getnext() override {
            if (current_index >= array_size)
                throw "array index out of bounds";

            return array[current_index++];
        }

    private:
        t* array;
        int array_size;
        int current_index;
    };

    iterator<t>* createarrayiterator(t* arr, int size) {
        return new arrayiterator(arr, size);
    }

    // range-based-for methods:
    binarytreeiterator<t> begin() {
        return *createiterator();
    }

    binarytreeiterator<t> end() {
        return binarytreeiterator<t>(this, nullptr);
    }

};

int main() {
    setlocale(LC_ALL, "russian");
    srand(time(0));
    binarytree<int> bt;
    int arr[10];
    for (int i = 0; i < 10; i++) {
        int val = rand() % 100;
        bt.addnode(val);
        arr[i] = val;
    }

    std::cout << "Выводим все элементы дерева с помощью итератора" << std::endl;
    auto iter = bt.createiterator();
    while (iter->hasmore())
        std::cout << iter->getnext() << " ";
    delete iter;
    std::cout << "\n\n";
  
    std::cout << "Выводим все элементы дерева с помощью итератора для массива" << std::endl;
    auto arr_iter = bt.createarrayiterator(arr, 10);
    while (arr_iter->hasmore())
        std::cout << arr_iter->getnext() << " ";
    delete arr_iter;
    std::cout << "\n\n";

    std::cout << " Выводим все элементы дерева с помощью range-based for loop" << std::endl;
    for (auto elem : bt)
        std::cout << elem << " ";
    std::cout << "\n\n";
    std::cout << " Выводим все элементы дерева с помощью явного итератора" << std::endl;
    for (binarytreeiterator<int> iterator = bt.begin(); iterator != bt.end(); ++iterator) {
        int elem = *iterator;
        std::cout << elem << " ";
    }
    std::cout << "\\n\\n";
    system("pause");
    return 0;
}