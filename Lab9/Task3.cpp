#include <iostream>
#include <string>
#include <vector>
using namespace std;

class TravelRoute {
public:
    float travel_cost;
    TravelRoute(float c) {
        travel_cost = c;
    }
};

class StarSystem {
public:
    int system_id;
    string name;
    int num_planets;
    float x, y, z;
    bool is_habitable;

    vector<StarSystem*> children;   // child star systems
    vector<TravelRoute> routes;     // routes[i] = cost to reach children[i]

    StarSystem(int id, string n, int planets, float X, float Y, float Z, bool habitable) {
        system_id = id;
        name = n;
        num_planets = planets;
        x = X;
        y = Y;
        z = Z;
        is_habitable = habitable;
    }
};

class Galaxy {
public:
    int M;
    StarSystem* root;

    // constructor 1: only M
    Galaxy(int m) {
        M = m;
        root = NULL;
    }

    // constructor 2: M and root
    Galaxy(int m, StarSystem* r) {
        M = m;
        root = r;
    }

    // connect a child to a parent with a travel cost
    void add_child(StarSystem* parent, StarSystem* child, float cost) {
        if (parent->children.size() == M) {
            cout << "Parent already has M children!" << endl;
            return;
        }
        parent->children.push_back(child);
        parent->routes.push_back(TravelRoute(cost));
    }

    // helper: count total nodes in a subtree
    int count_nodes(StarSystem* x) const {
        if (x == NULL) return 0;
        int total = 1;                       // this node
        for (int i = 0; i < x->children.size(); i++) {
            total = total + count_nodes(x->children[i]);
        }
        return total;
    }

    // b) balanced check
    bool is_balanced(StarSystem* x) const {
        if (x == NULL) return true;
        if (x->children.size() == 0) return true;   // leaf is balanced

        int smallest = count_nodes(x->children[0]);
        int largest = smallest;

        for (int i = 1; i < x->children.size(); i++) {
            int size = count_nodes(x->children[i]);
            if (size < smallest) smallest = size;
            if (size > largest) largest = size;
        }

        if (largest - smallest > 1) return false;

        // every child must also be balanced
        for (int i = 0; i < x->children.size(); i++) {
            if (!is_balanced(x->children[i])) return false;
        }
        return true;
    }

    // c) count leaves
    int count_leaves(StarSystem* x) const {
        if (x == NULL) return 0;
        if (x->children.size() == 0) return 1;      // leaf

        int leaves = 0;
        for (int i = 0; i < x->children.size(); i++) {
            leaves = leaves + count_leaves(x->children[i]);
        }
        return leaves;
    }
};

int main() {
    StarSystem* sol   = new StarSystem(1, "Sol",   8, 0, 0, 0, true);
    StarSystem* alpha = new StarSystem(2, "Alpha", 3, 4, 1, 0, true);
    StarSystem* beta  = new StarSystem(3, "Beta",  5, 2, 7, 1, false);
    StarSystem* gamma = new StarSystem(4, "Gamma", 1, 9, 3, 2, false);
    StarSystem* delta = new StarSystem(5, "Delta", 2, 5, 5, 5, true);

    Galaxy g(3, sol);                 // M = 3, root = Sol

    g.add_child(sol, alpha, 4.5);
    g.add_child(sol, beta, 3.2);
    g.add_child(sol, gamma, 7.0);
    g.add_child(alpha, delta, 2.1);

    cout << "Number of leaves: " << g.count_leaves(g.root) << endl;

    if (g.is_balanced(g.root))
        cout << "Tree is balanced" << endl;
    else
        cout << "Tree is NOT balanced" << endl;

    return 0;
}