#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>
using namespace std;

struct Node {
    string word;
    int freq;
    Node *left, *right;
    Node(const string& w) : word(w), freq(1), left(nullptr), right(nullptr) {}
};

class WordBST {
    Node* root = nullptr;
    int totalWords = 0;    // every word in the paragraph
    int uniqueWords = 0;   // number of BST nodes

    Node* insert(Node* n, const string& w) {
        if (!n) {
            uniqueWords++;
            return new Node(w);
        }
        if (w < n->word) n->left = insert(n->left, w);
        else if (w > n->word) n->right = insert(n->right, w);
        else n->freq++;                       // word already exists
        return n;
    }

    void inorder(Node* n) {
        if (!n) return;
        inorder(n->left);
        cout << "  " << n->word << " : " << n->freq << "\n";
        inorder(n->right);
    }

    void collect(Node* n, vector<Node*>& v) {
        if (!n) return;
        collect(n->left, v);
        v.push_back(n);
        collect(n->right, v);
    }

public:
    // Splits paragraph into words, ignores case and punctuation
    void buildFromText(const string& text) {
        string word;
        for (char ch : text) {
            if (isalpha(static_cast<unsigned char>(ch))) {
                word += tolower(static_cast<unsigned char>(ch));
            } else if (!word.empty()) {
                root = insert(root, word);
                totalWords++;
                word.clear();
            }
        }
        if (!word.empty()) {                  // last word (no punctuation after it)
            root = insert(root, word);
            totalWords++;
        }
    }

    void displayAlphabetical() {
        cout << "Alphabetical order:\n";
        inorder(root);
    }

    void displayByFrequency() {
        vector<Node*> v;
        collect(root, v);                     // already alphabetical
        stable_sort(v.begin(), v.end(),
                    [](Node* a, Node* b) { return a->freq > b->freq; });
        cout << "By frequency (high to low):\n";
        for (Node* n : v)
            cout << "  " << n->word << " : " << n->freq << "\n";
    }

    void findMostFrequentWord() {
        vector<Node*> v;
        collect(root, v);
        int maxFreq = 0;
        for (Node* n : v) maxFreq = max(maxFreq, n->freq);
        cout << "Most frequent word(s) (count = " << maxFreq << "):\n";
        for (Node* n : v)
            if (n->freq == maxFreq) cout << "  " << n->word << "\n";
    }

    void displaySummary() {
        cout << "Total unique words: " << uniqueWords << "\n";
        cout << "Total word count  : " << totalWords << "\n";
    }
};

int main() {
    string paragraph =
        "The quick brown fox jumps over the lazy dog. "
        "The fox is quick and smart.";

    WordBST tree;
    tree.buildFromText(paragraph);

    tree.displayAlphabetical();
    cout << "\n";
    tree.displayByFrequency();
    cout << "\n";
    tree.findMostFrequentWord();
    cout << "\n";
    tree.displaySummary();
    return 0;
}