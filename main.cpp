#include <algorithm>
#include <chrono>
#include <climits>
#include <iomanip>
#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <vector>

using namespace std;


struct Node {
    int key;
    int height;
    Node* left;
    Node* right;

    explicit Node(int value)
        : key(value), height(1), left(nullptr), right(nullptr) {}
};

int h(Node* node) {
    return node == nullptr ? 0 : node->height;
}

int bf(Node* node) {
    return node == nullptr ? 0 : h(node->left) - h(node->right);
}

void update(Node* node) {
    if (node != nullptr) {
        node->height = 1 + max(h(node->left), h(node->right));
    }
}

Node* rotateRight(Node* y) {
    Node* x = y->left;
    Node* middle = x->right;

    x->right = y;
    y->left = middle;

    update(y);
    update(x);
    return x;
}

Node* rotateLeft(Node* x) {
    Node* y = x->right;
    Node* middle = y->left;

    y->left = x;
    x->right = middle;

    update(x);
    update(y);
    return y;
}

Node* balance(Node* node) {
    if (node == nullptr) return nullptr;

    update(node);
    int factor = bf(node);

    if (factor > 1) {
        if (bf(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }

    if (factor < -1) {
        if (bf(node->right) > 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }

    return node;
}

Node* insert(Node* root, int key) {
    if (root == nullptr) return new Node(key);

    if (key < root->key) {
        root->left = insert(root->left, key);
    } else if (key > root->key) {
        root->right = insert(root->right, key);
    } else {
        return root;
    }

    return balance(root);
}

bool contains(Node* root, int key) {
    while (root != nullptr) {
        if (key == root->key) return true;
        root = (key < root->key) ? root->left : root->right;
    }
    return false;
}

void inorder(Node* root) {
    if (root == nullptr) return;
    inorder(root->left);
    cout << root->key << ' ';
    inorder(root->right);
}

void reverseInorder(Node* root) {
    if (root == nullptr) return;
    reverseInorder(root->right);
    cout << root->key << ' ';
    reverseInorder(root->left);
}

void preorder(Node* root) {
    if (root == nullptr) return;
    cout << root->key << ' ';
    preorder(root->left);
    preorder(root->right);
}

void printLevelOrder(Node* root) {
    if (root == nullptr) {
        cout << "(empty)";
        return;
    }

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* current = q.front();
        q.pop();

        cout << current->key << ' ';
        if (current->left != nullptr) q.push(current->left);
        if (current->right != nullptr) q.push(current->right);
    }
}

void printNodeDetails(Node* root) {
    if (root == nullptr) return;
    printNodeDetails(root->left);
    cout << root->key << "(BF=" << bf(root)
         << ", height=" << root->height << ") ";
    printNodeDetails(root->right);
}

void printTreeSideways(Node* root, int depth = 0) {
    if (root == nullptr) return;
    printTreeSideways(root->right, depth + 1);
    cout << string(depth * 4, ' ') << root->key
         << " [h=" << root->height << ", BF=" << bf(root) << "]\n";
    printTreeSideways(root->left, depth + 1);
}

void clear(Node* root) {
    if (root == nullptr) return;
    clear(root->left);
    clear(root->right);
    delete root;
}

Node* buildAVL(const vector<int>& values) {
    Node* root = nullptr;
    for (int value : values) root = insert(root, value);
    return root;
}

void printPreorderAndRoot(Node* root) {
    cout << "Preorder: ";
    preorder(root);
    cout << "\nRoot: " << (root ? root->key : -1)
         << ", root height: " << h(root)
         << ", root BF: " << bf(root) << '\n';
}


void bstRevisionAndAVLNodeHeights() {
    Node* root = buildAVL({30, 20, 40});

    cout << "Tree (sideways):\n";
    printTreeSideways(root);
    cout << "Inorder: ";
    inorder(root);
    cout << "\nPreorder: ";
    preorder(root);
    cout << "\nNode details in inorder: ";
    printNodeDetails(root);
    cout << "\nNo rotation is needed: the root has BF 0, and both children are leaves.\n";

    clear(root);
}

void singleRotations() {
    Node* caseA = buildAVL({30, 20, 10});
    cout << "Case A: 30, 20, 10 -> LL, so rotate right.\n";
    printPreorderAndRoot(caseA);

    Node* caseB = buildAVL({10, 20, 30});
    cout << "Case B: 10, 20, 30 -> RR, so rotate left.\n";
    printPreorderAndRoot(caseB);

    clear(caseA);
    clear(caseB);
}


void doubleRotations() {

    Node* caseA = buildAVL({30, 10, 20});
    cout << "Case A: 30, 10, 20 -> LR: rotate left at 10, then right at 30.\n";
    printPreorderAndRoot(caseA);

    Node* caseB = buildAVL({10, 30, 20});
    cout << "Case B: 10, 30, 20 -> RL: rotate right at 30, then left at 10.\n";
    printPreorderAndRoot(caseB);

    clear(caseA);
    clear(caseB);
}


void implementAVLInsertionStepByStep() {
    vector<int> values = {10, 20, 30, 40, 50, 25};
    Node* root = nullptr;

    for (int value : values) {
        root = insert(root, value);
        cout << "After inserting " << value << ": ";
        printPreorderAndRoot(root);
    }

    cout << "Final tree (sideways):\n";
    printTreeSideways(root);
    cout << "Rebalancing occurs when an ancestor's BF becomes +2 or -2.\n";
    clear(root);
}


void searchTraversalAndQueueRevision() {
    Node* root = buildAVL({50, 30, 70, 20, 40, 60, 80});

    cout << "Search 60: " << (contains(root, 60) ? "found" : "not found") << '\n';
    cout << "Search 99: " << (contains(root, 99) ? "found" : "not found") << '\n';
    cout << "Level order: ";
    printLevelOrder(root);
    cout << '\n';

    clear(root);
}


void diagnoseBalanceFactors() {
    Node* root = buildAVL({40, 20, 60, 10, 30, 50, 70, 5});

    cout << "key(BF,height) in inorder:\n";
    printNodeDetails(root);
    cout << "\nAll balance factors should be -1, 0, or +1.\n";
    clear(root);
}


void predictAndVerifyRotationType() {

    struct Test {
        vector<int> values;
        string expectedCase;
    };

    vector<Test> tests = {
        {{50, 30, 20}, "LL"},
        {{20, 30, 50}, "RR"},
        {{50, 20, 30}, "LR"},
        {{20, 50, 30}, "RL"}
    };
    char label = 'A';
    for (const Test& test : tests) {
        Node* root = buildAVL(test.values);
        cout << "Case " << label++ << " (" << test.expectedCase << "): ";
        for (int value : test.values) cout << value << ' ';
        cout << "-> final root = " << root->key << ", preorder = ";
        preorder(root);
        cout << '\n';
        clear(root);
    }
}


struct Validation {
    bool valid;
    int actualHeight;
};

Validation validateAVL(Node* node, long long low, long long high) {
    if (node == nullptr) return {true, 0};

    if (node->key <= low || node->key >= high) {
        return {false, 0};
    }

    Validation left = validateAVL(node->left, low, node->key);
    Validation right = validateAVL(node->right, node->key, high);

    int actualHeight = 1 + max(left.actualHeight, right.actualHeight);
    int actualBF = left.actualHeight - right.actualHeight;

    bool balanced = (actualBF >= -1 && actualBF <= 1);
    return {left.valid && right.valid && balanced, actualHeight};
}

void avlValidationAndBSTRevision() {
    Node* avlRoot = buildAVL({40, 20, 60, 10, 30, 50, 70});
    Validation avlResult = validateAVL(avlRoot, LLONG_MIN, LLONG_MAX);
    cout << "AVL-built tree valid: " << (avlResult.valid ? "yes" : "no")
         << ", independently calculated height: " << avlResult.actualHeight << '\n';

    Node* manualRoot = new Node(10);
    manualRoot->right = new Node(20);
    manualRoot->right->right = new Node(30);
    manualRoot->right->right->right = new Node(40);

    Validation manualResult = validateAVL(manualRoot, LLONG_MIN, LLONG_MAX);
    cout << "Manual unbalanced BST valid AVL: "
         << (manualResult.valid ? "yes" : "no")
         << ", independently calculated height: " << manualResult.actualHeight << '\n';

    clear(avlRoot);
    clear(manualRoot);
}


struct BSTNode {
    int key;
    BSTNode* left;
    BSTNode* right;

    explicit BSTNode(int value) : key(value), left(nullptr), right(nullptr) {}
};

void bstInsertIterative(BSTNode*& root, int key) {
    if (root == nullptr) {
        root = new BSTNode(key);
        return;
    }

    BSTNode* current = root;
    while (true) {
        if (key < current->key) {
            if (current->left == nullptr) {
                current->left = new BSTNode(key);
                return;
            }
            current = current->left;
        } else if (key > current->key) {
            if (current->right == nullptr) {
                current->right = new BSTNode(key);
                return;
            }
            current = current->right;
        } else {
            return;
        }
    }
}

bool bstContains(BSTNode* root, int key) {
    while (root != nullptr) {
        if (key == root->key) return true;
        root = (key < root->key) ? root->left : root->right;
    }
    return false;
}

int bstHeightIterative(BSTNode* root) {
    if (root == nullptr) return 0;

    queue<pair<BSTNode*, int>> q;
    q.push({root, 1});
    int result = 0;

    while (!q.empty()) {
        auto current = q.front();
        q.pop();
        result = max(result, current.second);

        if (current.first->left != nullptr)
            q.push({current.first->left, current.second + 1});
        if (current.first->right != nullptr)
            q.push({current.first->right, current.second + 1});
    }
    return result;
}

void bstClearIterative(BSTNode*& root) {
    if (root == nullptr) return;

    stack<BSTNode*> st;
    st.push(root);

    while (!st.empty()) {
        BSTNode* node = st.top();
        st.pop();

        if (node->left != nullptr) st.push(node->left);
        if (node->right != nullptr) st.push(node->right);
        delete node;
    }
    root = nullptr;
}

void bigOExperiment() {
    cout << left << setw(10) << "N"
         << setw(15) << "BST height"
         << setw(15) << "AVL height"
         << setw(20) << "BST us/search"
         << "AVL us/search\n";

    const int repetitions = 2000;
    volatile long long checksum = 0;

    for (int n : {1000, 5000, 10000}) {
        BSTNode* bstRoot = nullptr;
        Node* avlRoot = nullptr;

        for (int key = 1; key <= n; ++key) {
            bstInsertIterative(bstRoot, key);
            avlRoot = insert(avlRoot, key);
        }

        auto bstStart = chrono::steady_clock::now();
        for (int i = 0; i < repetitions; ++i) {
            checksum += bstContains(bstRoot, n) ? 1 : 0;
        }
        auto bstEnd = chrono::steady_clock::now();

        auto avlStart = chrono::steady_clock::now();
        for (int i = 0; i < repetitions; ++i) {
            checksum += contains(avlRoot, n) ? 1 : 0;
        }
        auto avlEnd = chrono::steady_clock::now();

        double bstMicroseconds =
            chrono::duration<double, micro>(bstEnd - bstStart).count() / repetitions;
        double avlMicroseconds =
            chrono::duration<double, micro>(avlEnd - avlStart).count() / repetitions;

        cout << left << setw(10) << n
             << setw(15) << bstHeightIterative(bstRoot)
             << setw(15) << h(avlRoot)
             << setw(20) << fixed << setprecision(4) << bstMicroseconds
             << avlMicroseconds << '\n';

        bstClearIterative(bstRoot);
        clear(avlRoot);
    }

    cout << "Checksum: " << checksum << '\n';
    cout << "Sorted insertion makes this ordinary BST a chain (O(n) height).\n"
         << "AVL height remains O(log n); search is O(log n) in the worst case.\n";
}

void studentIDLookupSystem() {
    Node* root = buildAVL({104, 101, 109, 102, 106, 111, 103});

    cout << "Search ID 106: " << (contains(root, 106) ? "found" : "not found") << '\n';
    cout << "Search ID 120: " << (contains(root, 120) ? "found" : "not found") << '\n';
    cout << "Sorted student IDs: ";
    inorder(root);
    cout << "\nBalanced lookup avoids a long chain when IDs arrive in sorted order.\n";

    clear(root);
}


void productCodeDirectory() {
    Node* root = buildAVL({500, 200, 800, 100, 300, 700, 900});

    cout << "Sorted product codes: ";
    inorder(root);
    cout << "\nCode 700: " << (contains(root, 700) ? "exists" : "does not exist") << '\n';
    cout << "Code 650: " << (contains(root, 650) ? "exists" : "does not exist") << '\n';

    root = insert(root, 650);
    cout << "After adding 650: ";
    inorder(root);
    cout << "\nNew tree height: " << h(root) << '\n';

    clear(root);
}


bool findSuccessor(Node* root, int key, int& successor) {
    Node* candidate = nullptr;
    Node* current = root;

    while (current != nullptr) {
        if (current->key > key) {
            candidate = current;
            current = current->left;
        } else {
            current = current->right;
        }
    }

    if (candidate == nullptr) return false;
    successor = candidate->key;
    return true;
}

void appointmentNumberOrganizer() {
    Node* root = buildAVL({30, 10, 50, 5, 20, 40, 60, 15});

    cout << "Appointments in increasing order: ";
    inorder(root);

    int nextAppointment;
    if (findSuccessor(root, 20, nextAppointment)) {
        cout << "\nNext appointment greater than 20: " << nextAppointment << '\n';
    } else {
        cout << "\nThere is no appointment greater than 20.\n";
    }

    clear(root);
}


void leaderboardScores() {
    Node* root = buildAVL({70, 20, 90, 50, 80, 10, 60});

    cout << "Ascending scores: ";
    inorder(root);
    cout << "\nDescending scores: ";
    reverseInorder(root);
    cout << "\nRepeated searches take O(log n) worst-case time in an AVL tree.\n";

    clear(root);
}

void printRange(Node* root, int low, int high) {
    if (root == nullptr) return;

    // A left subtree can only contain values below root->key.
    if (root->key > low) {
        printRange(root->left, low, high);
    }

    if (root->key >= low && root->key <= high) {
        cout << root->key << ' ';
    }

    if (root->key < high) {
        printRange(root->right, low, high);
    }
}

void libraryBookNumberRangeSearch() {
    Node* root = buildAVL({45, 20, 70, 10, 30, 60, 80, 25, 35});

    cout << "Book numbers from 25 through 65 inclusive: ";
    printRange(root, 25, 65);
    cout << "\nRange search uses BST ordering to skip irrelevant subtrees.\n";

    clear(root);
}


int main() {
    bstRevisionAndAVLNodeHeights();
    singleRotations();
    doubleRotations();
    implementAVLInsertionStepByStep();
    searchTraversalAndQueueRevision();
    diagnoseBalanceFactors();
    predictAndVerifyRotationType();
    avlValidationAndBSTRevision();
    bigOExperiment();
    studentIDLookupSystem();
    productCodeDirectory();
    appointmentNumberOrganizer();
    leaderboardScores();
    libraryBookNumberRangeSearch();

    return 0;
}


