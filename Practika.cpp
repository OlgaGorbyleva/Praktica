#include<iostream>
#include <stack>

template <typename T>
__interface Iterator {
	virtual bool hasMore() const = 0;
	virtual T getNext() = 0;
};


template <typename T>
__interface IterableCollection {
	virtual Iterator<T>* createIterator() = 0;
};


template <typename T>
struct Node {
	T value;
	Node<T>* parent = nullptr;
	Node<T>* right = nullptr;
	Node<T>* left = nullptr;
	Node(T val) : value(val) {}
	void showNode() const {
		std::cout << value << ", ";
	}
};


template <typename T>
class BinaryTree;

template <typename T>
class BinaryTreeIterator : public Iterator<T> {
public:
	BinaryTreeIterator(BinaryTree<T>* tree, Node<T>* start) {
		this->tree = tree;
		current = start;
	}

	bool hasMore() const override {
		return current != nullptr || !nodeStack.empty();
	}

	T getNext() override {
		if (current == nullptr && !nodeStack.empty()) {
			current = nodeStack.top();
			nodeStack.pop();
		}
		else if (current == nullptr && nodeStack.empty()) {
			throw "tree is empty";
		}

		T result = current->value;
		if (current->right != nullptr) {
			nodeStack.push(current->right);
		}
		current = current->left;
		if (current == nullptr && !nodeStack.empty()) {
			current = nodeStack.top();
			nodeStack.pop();
		}
		return result;
	}

	bool operator!=(const BinaryTreeIterator<T>& other) const {
		return current != other.current;
	}

	T operator*() const {
		return current->value;
	}

	T* operator->() const {
		return &(current->value);
	}

	BinaryTreeIterator<T> operator++() {
		getNext();
		return *this;
	}

private:
	BinaryTree<T>* tree = nullptr;
	std::stack<Node<T>*> nodeStack;
	Node<T>* current = nullptr;
};

template <typename T>
class BinaryTree : public IterableCollection<T> {
	Node<T>* root = nullptr;

public:
	bool isEmpty() const {
		return root == nullptr;
	}

	BinaryTree() {}

	~BinaryTree() {
		clear();
	}

	Node<T>* getRoot() const {
		return root;
	}

	void showTree() const {
		std::cout << "\n";
		showTree(root);
		std::cout << "\n";
	}

private:
	void showTree(Node<T>* element) const {
		if (element != nullptr) {
			showTree(element->left);
			element->showNode();
			showTree(element->right);
		}
	}

public:
	void clear() {
		if (isEmpty())
			return;

		std::stack<Node<T>*> nodeStack;
		nodeStack.push(root);
		while (!nodeStack.empty()) {
			Node<T>* current = nodeStack.top();
			nodeStack.pop();
			if (current->right != nullptr)
				nodeStack.push(current->right);
			if (current->left != nullptr)
				nodeStack.push(current->left);
			delete current;
		}
		root = nullptr;
	}

public:
	Node<T>* findNode(const T& value) const {
		return findNodeHelper(root, value);
	}

private:
	Node<T>* findNodeHelper(Node<T>* current, const T& value) const {
		if (current == nullptr || current->value == value)
			return current;
		if (value < current->value)
			return findNodeHelper(current->left, value);
		return findNodeHelper(current->right, value);
	}

public:
	void addNode(T value) {
		auto n = new Node<T>(value);
		Node<T>* parent = nullptr;
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
	BinaryTreeIterator<T>* createIterator()  override { // iterator pattern method
		return new BinaryTreeIterator<T>(this, root);
	}

	////////////////////////////////////////////////////////////////////
	// range-based-for methods:
	BinaryTreeIterator<T> begin() {
		return *createIterator();
	}

	BinaryTreeIterator<T> end() {
		return BinaryTreeIterator<T>(this, nullptr);
	}

};

int main() {
	setlocale(LC_ALL, "Russian");
	srand(time(0));
	BinaryTree<int> bt;
	for (int i = 0; i < 10; i++)
		bt.addNode(rand() % 100);

	std::cout << "Выводим все элементы дерева с помощью итератора" << std::endl;
	auto iter = bt.createIterator();
	while (iter->hasMore())
		std::cout << iter->getNext() << " ";
	delete iter;
	std::cout << "\n\n";
	std::cout << " Выводим все элементы дерева с помощью range-based for loop" <<std:: endl;
	for (auto elem : bt)
		std::cout << elem << " ";
	std::cout << "\n\n";
	std::cout << " Выводим все элементы дерева с помощью явного итератора" << std::endl;
	for (BinaryTreeIterator<int> iterator = bt.begin(); iterator != bt.end(); ++iterator) {
		int elem = *iterator;
		std::cout << elem << " ";
	}
	std::cout << "\n\n";
	system("pause");
	return 0;
}