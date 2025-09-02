#include <iostream>
#include <fstream>
#include <string>

using namespace std;

constexpr bool RED = true;
constexpr bool BLACK = false;

string lowerCase(string word) {
	for (int i = 0; i < word.size(); i++) {
		word[i] = tolower(word[i]);
	}

	return word;
}

class Node {
public:
	string data;
	bool color;
	Node* parent;
	Node* left;
	Node* right;

	Node(string val) {
		data = val;
		color = RED;
		parent = nullptr;
		left = nullptr;
		right = nullptr;
	}

	void toggleColor() {
		if (color == RED) {
			color = BLACK;
		}
		else {
			color = RED;
		}
	}
};

class RedBlackTree {
private:
	Node* root;
	int n;

public:

	int getLength() {
		return this->n;
	}

	RedBlackTree() {
		this->root = nullptr;
		this->n = 0;
	}

	bool insert(string key) {
		key = lowerCase(key);

		Node* newNode = new Node(key);
		Node* parent = nullptr;
		Node* current = root;

		while (current != nullptr) {
			parent = current;
			if (key == current->data) {
				delete newNode;
				return false;
			}
			else if (key < current->data) {
				current = current->left;
			}
			else {
				current = current->right;
			}
		}

		if (parent == nullptr) {
			root = newNode;
		}
		else {
			if (key < parent->data) {
				parent->left = newNode;
			}
			else {
				parent->right = newNode;
			}
			newNode->parent = parent;
		}

		newNode->left = nullptr;
		newNode->right = nullptr;
		fixup(newNode);

		this->n++;
		return true;
	}

	void fixup(Node* node) {
		while (node != root && node->parent->color == RED) {
			Node* uncle = getUncle(node);
			Node* grandparent = node->parent->parent;

			if (uncle != nullptr && uncle->color == RED) {

				node->parent->color = BLACK;
				uncle->color = BLACK;
				grandparent->color = RED;
				node = grandparent;
			}
			else {

				if (node == node->parent->right && node->parent == grandparent->left) {

					node = node->parent;
					leftRotate(node);
				}
				else if (node == node->parent->left && node->parent == grandparent->right) {

					node = node->parent;
					rightRotate(node);
				}

				node->parent->color = BLACK;
				grandparent->color = RED;
				if (node == node->parent->left) {
					rightRotate(grandparent);
				}
				else {
					leftRotate(grandparent);
				}
			}
		}

		root->color = BLACK;
	}

	Node* getUncle(Node* node) {
		if (node == nullptr) {
			return nullptr;
		}
		Node* grandparent = node->parent->parent;

		if (node->parent == grandparent->left) {
			return grandparent->right;
		}
		else {
			return grandparent->left;
		}
	}

	void leftRotate(Node* x) {
		Node* y = x->right;
		x->right = y->left;

		if (y->left != nullptr) {
			y->left->parent = x;
		}

		y->parent = x->parent;
		if (x->parent == nullptr) {
			root = y;
		}
		else if (x == x->parent->left) {
			x->parent->left = y;
		}
		else {
			x->parent->right = y;
		}

		y->left = x;
		x->parent = y;
	}

	void rightRotate(Node* x) {
		Node* y = x->left;
		x->left = y->right;

		if (y->right != nullptr) {
			y->right->parent = x;
		}

		y->parent = x->parent;
		if (x->parent == nullptr) {
			root = y;
		}
		else if (x == x->parent->right) {
			x->parent->right = y;
		}
		else {
			x->parent->left = y;
		}

		y->right = x;
		x->parent = y;
	}

	void inorderTraversal(Node* node) {
		if (node != nullptr) {
			inorderTraversal(node->left);
			cout << node->data << " (" << (node->color == RED ? "RED" : "BLACK") << ") \n";
			inorderTraversal(node->right);
		}
	}

	void printTree() {
		inorderTraversal(root);
		cout << endl;
	}

	int getTreeHeight() {
		return computeHeight(root);
	}

	int getBlackHeight() {
		return computeBlackHeight(root);
	}

	int computeHeight(Node* node) {
		if (node == nullptr) return 0;
		int leftHeight = computeHeight(node->left);
		int rightHeight = computeHeight(node->right);
		return max(leftHeight, rightHeight) + 1;
	}

	int computeBlackHeight(Node* node) {
		int blackHeight = 0;
		while (node != nullptr) {
			if (node->color == BLACK) {
				blackHeight++;
			}
			node = node->left;
		}
		return blackHeight;
	}

	bool search(string key) {
		key = lowerCase(key);
		return searchHelper(root, key);
	}

	bool searchHelper(Node* node, string key) {
		if (node == nullptr) return false;

		if (key == node->data) {
			return true;
		}
		else if (key < node->data) {
			return searchHelper(node->left, key);
		}
		else {
			return searchHelper(node->right, key);
		}
	}
};

class Dictionary {
public:	RedBlackTree tree;
private:
	ofstream outputFile;

	bool load(string fileDest)
	{
		ifstream inputFile(fileDest);

		if (!inputFile) {
			return false;
		}

		string line;
		while (getline(inputFile, line)) {
			this->tree.insert(line);
		}

		inputFile.close();
		return true;
	}

public:

	Dictionary(string filePath) {
		this->tree = RedBlackTree();
		if (!this->load(filePath)) {
			cerr << "Failed to open file.\n";
		}

		this->outputFile = ofstream(filePath, ios::app);
		if (!this->outputFile.is_open()) {
			cerr << "Failed to open file.\n";
		}
	}

	bool saveWord(string word)
	{
		if (!this->tree.insert(word)) {
			return false;
		}

		this->outputFile << '\n' << word << flush;
		return !this->outputFile.fail();
	}

	bool search(string word) {
		return this->tree.search(word);
	}

	void printDictionary() {
		this->tree.printTree();
	}

	int getLength() {
		return this->tree.getLength();
	}

	~Dictionary() {
		this->outputFile.close();
	}
};

int main() {
	char path[] = "C:\\Users\\omar\\Desktop\\Dictionary.txt";

	Dictionary dict(path);

	string word;
	int option;

	while (true)
	{
		cout << "options:\n" << "1- lookup word\n" << "2- insert new word\n" << "choose: ";
		cin >> option;
		switch (option)
		{
		case 1:
			cout << "What is the word: ";
			cin >> word;
			cout << (dict.search(word) ? "Yes" : "No") << '\n';
			break;
		case 2:
			cout << "What is the word: ";
			cin >> word;
			cout << (dict.saveWord(word) ? "" : "ERROR : Word already in the dictionary!") << '\n';
			break;

		default:
			break;
		}

		//validation
		cout << "Size of dictionary: " << dict.getLength() << '\n';
		cout << "Height of dictionary: " << dict.tree.getTreeHeight() << '\n';
		cout << "Black Height of dictionary: " << dict.tree.getBlackHeight() << "\n\n";
	}

	return 0;
}
