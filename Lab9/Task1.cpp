#include <iostream>
#include <vector>
#include <stack>
using namespace std;

struct Node {
    int key;
    Node *left, *right;
    Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

// ---------- 1. Build BST ----------
Node* insert(Node* root, int key) {
    if (!root) return new Node(key);
    if (key < root->key) root->left = insert(root->left, key);
    else if (key > root->key) root->right = insert(root->right, key);
    return root;
}

void inorder(Node* root) {
    if (!root) return;
    inorder(root->left);
    cout << root->key << " ";
    inorder(root->right);
}

// ---------- 2. Predecessor / Successor ----------
void findPredSucc(Node* root, int key, Node*& pred, Node*& succ) {
    pred = succ = nullptr;
    Node* cur = root;
    while (cur) {
        if (key < cur->key) {
            succ = cur;            // possible successor
            cur = cur->left;
        } else if (key > cur->key) {
            pred = cur;            // possible predecessor
            cur = cur->right;
        } else {
            // key found: use its subtrees if they exist
            if (cur->left) {
                Node* t = cur->left;
                while (t->right) t = t->right;   // max of left subtree
                pred = t;
            }
            if (cur->right) {
                Node* t = cur->right;
                while (t->left) t = t->left;     // min of right subtree
                succ = t;
            }
            return;
        }
    }
}

// ---------- 3 & 4. Range Query (recursive) ----------
// cmp counts every comparison of a node key against lo / hi
void rangeRecursive(Node* n, int lo, int hi, int& cmp) {
    if (!n) return;

    cmp++;                                   // lo < key ?
    if (lo < n->key) rangeRecursive(n->left, lo, hi, cmp);

    cmp++;                                   // lo <= key ?
    if (lo <= n->key) {
        cmp++;                               // key <= hi ?
        if (n->key <= hi) cout << n->key << " ";
    }

    cmp++;                                   // key < hi ?
    if (n->key < hi) rangeRecursive(n->right, lo, hi, cmp);
}

// ---------- 3 & 4. Range Query (iterative) ----------
void rangeIterative(Node* root, int lo, int hi, int& cmp) {
    stack<Node*> st;
    Node* cur = root;
    while (cur || !st.empty()) {
        while (cur) {
            st.push(cur);
            cmp++;                           // lo < key ?
            cur = (lo < cur->key) ? cur->left : nullptr;
        }
        cur = st.top();
        st.pop();

        cmp++;                               // lo <= key ?
        if (lo <= cur->key) {
            cmp++;                           // key <= hi ?
            if (cur->key <= hi) cout << cur->key << " ";
        }

        cmp++;                               // key < hi ?
        cur = (cur->key < hi) ? cur->right : nullptr;
    }
}

// ---------- Main ----------
int main() {
    vector<int> data = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65, 85};
    Node* root = nullptr;
    for (int x : data) root = insert(root, x);

    cout << "Inorder traversal: ";
    inorder(root);
    cout << "\n\n";

    int key = 40;
    Node *pred, *succ;
    findPredSucc(root, key, pred, succ);
    cout << "Key = " << key << "\n";
    cout << "Predecessor: " << (pred ? to_string(pred->key) : "None") << "\n";
    cout << "Successor  : " << (succ ? to_string(succ->key) : "None") << "\n\n";

    int lo = 30, hi = 70;
    cout << "Range query [" << lo << ", " << hi << "]\n";

    int cmpRec = 0;
    cout << "Recursive : ";
    rangeRecursive(root, lo, hi, cmpRec);
    cout << "\n  Comparisons: " << cmpRec << "\n";

    int cmpIter = 0;
    cout << "Iterative : ";
    rangeIterative(root, lo, hi, cmpIter);
    cout << "\n  Comparisons: " << cmpIter << "\n";

    return 0;
}