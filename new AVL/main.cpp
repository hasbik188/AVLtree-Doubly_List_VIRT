#include <bits/stdc++.h>

struct Room{
    char letter;
    int number;

    Room(){
        letter = 'A';
        number = 100;
    }

    Room(char let, int num){
        letter = let;
        number = num;
    }
};

struct Node{
    Room* room;
    Node* next;
    Node* prev;

    Node(){
        room = nullptr;
        next = nullptr;
        prev = nullptr;
    }

    Node(char let, int num){
        room = new Room(let, num);
        next = nullptr;
        prev = nullptr;
    }

    ~Node() = default;
};

struct TNode{
    int height;
    Node* list;
    TNode* left;
    TNode* right;
    int balance;

    TNode(){
        list = nullptr;
        left = nullptr;
        right = nullptr;
        height = 1;
        balance = 0;
    }

    TNode(char let, int num){
        list = new Node(let, num);
        left = nullptr;
        right = nullptr;
        height = 1;
        balance = 0;
    }

    ~TNode() = default;
};

char GetRoomLetter(Node* node){
    return node->room->letter;
}

int GetRoomNumber(Node* node){
    return node->room->number;
}

int NodeNumber(Node* node){
    int number = 1;
    if (node->next == nullptr)
        return number;
    Node* current = node->next;
    while(current != nullptr){
        number++;
        current = current->next;
    }
    return number;
}

void DeleteFromList(TNode* node){
    Node* listnode = node->list;
    Node* temp = listnode->next;
    listnode->next = temp->next;
    if (temp->next != nullptr)
        temp->next->prev = listnode;
    delete(temp);
}

void AddToList(Node* &node){
    Node* newone = new Node(GetRoomLetter(node), GetRoomNumber(node));
    if (node != nullptr) {
        node->prev = newone;
        newone->next = node;
    }
    node = newone;
}

int GetRoomNumber(TNode* p){
    return p->list->room->number;
}

char GetRoomLetter(TNode* p){
    return p->list->room->letter;
}

TNode* FindNode(TNode* root, char let, int num){
    if (root == nullptr) {
        std::cout << "Дерево пустое.\n";
        return nullptr;
    }
    TNode* current = root;
    while((current != nullptr) && (GetRoomNumber(current) != num || GetRoomLetter(current) != let)){
        if (GetRoomLetter(current) < let) current = current->left;
        else {
            if (GetRoomLetter(current) > let) current = current->right;
            else if (GetRoomLetter(current) == let) {
                if (GetRoomNumber(current) < num) current = current->left;
                else if (GetRoomNumber(current) > num) current = current->right;
            }
        }
    }
    if (current == nullptr) {
        std::cout << "Элемент не найден.\n";
        return nullptr;
    }
    return current;
}

void InsertInTree(TNode*& root, char let, int num, bool& h) {
    if ((root == nullptr)||(root->list == nullptr)) {
        root = new TNode(let, num);
        root->height = 1;
        root->left = nullptr;
        root->right = nullptr;
        h = true;
    } else if ((GetRoomLetter(root) > let) || (GetRoomLetter(root) == let && GetRoomNumber(root) > num)) {
        InsertInTree(root->left, let, num, h);
        if (h == true) {
            if (root->balance == 1) {
                root->balance = 0;
                h = false;
            } else if (root->balance == 0) {
                root->balance = -1;
            } else if (root->balance == -1) {
                TNode *node = root->left;
                if (node->balance == -1) {
                    root->left = node->right;
                    node->right = root;
                    root->balance = 0;
                    root = node;
                } else {
                    TNode *node1 = node->right;
                    node->right = node1->left;
                    node1->left = node;
                    root->left = node1->right;
                    node1->right = root;
                    if (node1->balance == -1)
                        root->balance = 1;
                    else
                        root->balance = 0;
                    if (node1->balance == 1)
                        node1->balance = -1;
                    else
                        node->balance = 0;
                    root = node1;
                }
                root->balance = 0;
                h = false;
            }
        }
    }else if ((GetRoomLetter(root) < let) || (GetRoomLetter(root) == let && GetRoomNumber(root) < num)) {
        InsertInTree(root->right, let, num, h);
        if (h == true) {
            if (root->balance == -1) {
                root->balance = 0;
                h = false;
            } else if (root->balance == 0){
                root->balance = 1;
            }else if (root->balance == 1) {
                TNode *node = root->right;
                if (node->balance == 1) {
                    root->right = node->left;
                    node->left = root;
                    root->balance = 0;
                    root = node;
                } else {
                    TNode *node1 = node->left;
                    node->left = node1->right;
                    node1->right = node;
                    root->right = node1->left;
                    node1->left = root;
                    if (node1->balance == 1) root->balance = -1;
                    else root->balance = 0;
                    if (node1->balance == -1) node->balance = 1;
                    else node->balance = 0;
                    root = node1;
                }
                root->balance = 0;
                h = false;
            }
        }
    }
    else AddToList(root->list);
}

void BalanceL(TNode*& root, bool& h){
    if (root->balance == -1)
        root->balance = 0;
    else if (root->balance == 0) {
        root->balance = 1;
        h = false;
    }
    else {
        TNode* node = root->right;
        if (node->balance >= 0){
            root->right = node->left;
            node->left = root;
            if (node->balance == 0){
                root->balance = 1;
                node->balance = -1;
                h = false;
            }
            else {
                root->balance = 0;
                node->balance = 0;
            }
            root = node;
        }
        else {
            TNode* node1 = node->left;
            node->left = node1->right;
            node1->right = node;
            root->right = node1->left;
            node1->left = root;
            if (node1->balance == 1) root->balance = -1;
            else root->balance = 0;
            if (node1->balance == -1) node->balance = 1;
            else node->balance = 0;
            root = node1;
            node1->balance = 0;
        }
    }
}

void BalanceR(TNode*& root, bool& h){
    if (root->balance == 1)
        root->balance = 0;
    else if (root->balance == 0) {
        root->balance = -1;
        h = false;
    }
    else {
        TNode* node = root->left;
        if (node->balance <= 0){
            root->left = node->right;
            node->right = root;
            if (node->balance == 0){
                root->balance = -1;
                node->balance = 1;
                h = false;
            }
            else {
                root->balance = 0;
                node->balance = 0;
            }
            root = node;
        }
        else {
            TNode* node1 = node->right;
            node->right = node1->left;
            node1->left = node;
            root->left = node1->right;
            node1->right = root;
            if (node1->balance == -1) root->balance = 1;
            else root->balance = 0;
            if (node1->balance == 1) node->balance = -1;
            else node->balance = 0;
            root = node1;
            node1->balance = 0;
        }
    }
}

TNode* DeleteFromTr(TNode*& root,TNode*& node, bool& h){
    if (root->right != nullptr){
        DeleteFromTr(root->right,node, h);
        if (h)
            BalanceR(root, h);
    }
    else{
        node->list = root->list;
        root = root->left;
        h = true;
    }
    return root;
}

TNode* DeleteFromTree(TNode*& root, char let, int num, bool& h){
    if (root == nullptr) std::cout << "Tree is empty";
    else if ((GetRoomLetter(root) > let)||(GetRoomLetter(root) == let &&
                                           GetRoomNumber(root) > num)){
        root->left = DeleteFromTree(root->left, let, num, h);
        if (h == true) BalanceL(root, h);
    }
    else if ((GetRoomLetter(root) < let)||(GetRoomLetter(root) == let &&
                                           GetRoomNumber(root) < num)){
        root->right = DeleteFromTree(root->right, let, num, h);
        if (h == true) BalanceR(root, h);
    }
    else if (NodeNumber(root->list) > 1){
        DeleteFromList(root);
        return root;
    }
    else {
        TNode* node = root;
        if (node->right == nullptr) {
            root = node->left;
            h = true;
        }
        else if (node->left == nullptr){
            root = node->right;
            h = true;
        }
        else {
            node->left = DeleteFromTr(node->left, node, h);
            if (h)
                BalanceL(root, h);
        }
    }
    return root;
}

void DeleteTree(TNode*& node){
    if (node != nullptr) {
        if (node->left)
            DeleteTree(node->left);
        if (node->right)
            DeleteTree(node->right);
        delete(node->list);
        node->list = nullptr;
        node->left = nullptr;
        node->right = nullptr;
        delete(node);
    }
    node = nullptr;
}

void PrintOnSide(int h, TNode* node){
    if (node != nullptr){
        PrintOnSide(h + 8, node->right);
        for (int i = 0; i < h; ++i) std::cout << " ";
        std::cout << GetRoomLetter(node) << GetRoomNumber(node) << " " << NodeNumber(node->list) << std::endl;
        PrintOnSide(h + 8, node->left);
    }
}

void ToStack(TNode* a, std::stack<std::pair<char, int>>* &astack){
    if (a != nullptr){
        ToStack(a->left, astack);
        astack->push(std::make_pair(GetRoomLetter(a), GetRoomNumber(a)));
        ToStack(a->right, astack);
    }

}

bool AreEqual(TNode* a, TNode* b){
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr ^ b == nullptr) return false;
    auto* astack = new std::stack<std::pair<char, int>>;
    auto* bstack = new std::stack<std::pair<char, int>>;
    ToStack(a, astack);
    ToStack(b, bstack);
    while (!astack->empty() && !bstack->empty()){
        if ((astack->top().first != bstack->top().first) || ((astack->top().second != bstack->top().second))) return false;
        astack->pop();
        bstack->pop();
    }
    return AreEqual(a->left, b->left)&&(AreEqual(a->right, b->right));
}

int main() {
    TNode* tree = new TNode();
//  TNode* tree1 = new TNode();
    bool h = true;
    for(int i = 0; i < 20; i++)
    {
        InsertInTree(tree, 'A', i, h);
    }
    for(int i = 0; i < 20; i++)
    {
        InsertInTree(tree, 'A', 60-i, h);
    }
    PrintOnSide(0, tree);
    std::cout << "    " << std::endl;
    DeleteTree(tree);
    PrintOnSide(0,tree);
//

//
//    PrintOnSide(0, tree);
//
//    //tree = DeleteFromTree(tree, 'A', 100, h);
//    std::cout << std::endl;
//    PrintOnSide(0, tree);
}
