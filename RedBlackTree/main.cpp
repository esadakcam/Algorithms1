#include <iostream>
#include <fstream>
#include <vector>
using namespace std;
#define BLACK 0
#define RED 1

/**********************************************************************************************************************************************************************/
class GameNode // Node object of the rbtree
{
    GameNode *parent;
    GameNode *right;
    GameNode *left;

    friend class RBTree; // rbtree can access all of the properties of this class

public:
    string name; // used as key
    string platform;
    int releaseYear;
    string publisher;
    double naSales;
    double euSales;
    double otherSales;
    int color; // red or black

public:
    // constrcutor of the GameNode
    GameNode(string name, string platform, int releaseYear, string publisher, double naSales, double euSales, double otherSales, int color);
    // Method to print node's properties
    void print();
};

/**********************************************************************************************************************************************************************/
class RBTree // rb tree object
{
    GameNode *root; // root of the tree
    GameNode *nill; // a pointer to hold the null leaves of the rbtree
    // rotations
    void rightRotate(GameNode *node);
    void leftRotate(GameNode *node);
    // method for providing the rbtree features
    void insertFixUp(GameNode *node);

public:
    RBTree(const char *file);                      // constructor that takes the name of the csv file
    void insert(GameNode const &node);             // method to insert new node to the tree
    void preOrderPrint(GameNode *node, int index); // method to traverse in preorder and print the tree
    GameNode *getRoot() { return root; }           // getter
    // finding best sellers
    GameNode findEuBestSeller(GameNode *node, GameNode &max);
    GameNode findNaBestSeller(GameNode *node, GameNode &max);
    GameNode findOtherBestSeller(GameNode *node, GameNode &max);
    void printBestSeller(int region);
    void readFile(const char *file); // method for reading the file, calling insert function for each entry and printing the tree after each decade
};

/**********************************************************************************************************************************************************************/
GameNode::GameNode(string name, string platform, int releaseYear, string publisher, double naSales, double euSales, double otherSales, int color)
{
    this->right = NULL;
    this->left = NULL;
    this->parent = NULL;

    this->name = name;
    this->platform = platform;
    this->releaseYear = releaseYear;
    this->publisher = publisher;
    this->naSales = naSales;
    this->euSales = euSales;
    this->otherSales = otherSales;
    this->color = color;
}

void GameNode::print()
{
    string color = (this->color == 0) ? "BLACK" : "RED";
    cout << "( " << color << " ) " << publisher << endl;
}
/**********************************************************************************************************************************************************************/

RBTree::RBTree(const char *file)
{
    nill = new GameNode("", "", -1, "", -1, -1, -1, BLACK); // create the nill pointer as a black node
    root = nill;                                            // root is initially assigned to nill
    readFile(file);
}
void RBTree::preOrderPrint(GameNode *node, int index)
{
    // node-left subtree-right subtree
    if (node != nill)
    {
        for (int i = 0; i < index; i++)
        {
            cout << "-"; // print indentations according to height of the tree
        }
        node->print();
        preOrderPrint(node->left, index + 1);
        preOrderPrint(node->right, index + 1);
    }
}
void RBTree::insert(GameNode const &data)
{
    // create new node accordingly as a red node
    GameNode *toadd = new GameNode(data.name, data.platform, data.releaseYear, data.publisher, data.naSales, data.euSales, data.otherSales, RED);
    toadd->left = nill;
    toadd->right = nill;

    GameNode *y = NULL; // y is for holding the parent of x which is the corrent position of new node
    GameNode *x = root;
    while (x != nill) // search for position of the new node
    {
        y = x;
        if (toadd->publisher < x->publisher) // if new node is less than current one go left
        {
            x = x->left;
        }
        else if (toadd->publisher > x->publisher) // if new node is greater than current one go right
        {
            x = x->right;
        }
        else // tree contains a node with same key with the new node
        {
            // increase corresponding values
            x->euSales += toadd->euSales;
            x->naSales += toadd->naSales;
            x->otherSales += toadd->otherSales;
            delete toadd; // delete unnececary created node so that memmory leak does not occur. Important note this node is created in this function. Any existing node of tree is not deleted
            return;
        }
    }
    toadd->parent = y;
    if (y == NULL) // if added nodes parent is null, make it root of the tree
    {
        root = toadd;
    }
    else if (toadd->publisher < y->publisher) // if the added node is less than its parent make it left child
    {
        y->left = toadd;
    }
    else // if the added node is greater than its parent make it right child
    {
        y->right = toadd;
    }
    if (toadd->parent == NULL) // if new node is root, make it black and return
    {
        toadd->color = BLACK;
        return;
    }
    if (toadd->parent->parent == NULL) // if grandparent is null return
    {
        return;
    }
    insertFixUp(toadd); // maintain rbtree property
}

void RBTree::insertFixUp(GameNode *node)
{
    GameNode *uncle;
    while (node->parent->color == 1)
    {
        if (node->parent == node->parent->parent->right)
        {
            uncle = node->parent->parent->left; // uncle
            if (uncle->color == 1)
            {
                // case 1
                // parent of node and uncle is red
                // uncle and parent becomes black and grandparent becomes red
                uncle->color = 0;
                node->parent->color = 0;
                node->parent->parent->color = 1;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->left)
                {
                    // case 2
                    // parent is right child of the grandparent and node is left child of parent
                    // do right rotation
                    node = node->parent;
                    rightRotate(node);
                }
                // case 3
                //  parent is right child of the grandparent and the node is right child of parent
                // left rotation at grandparent and change the sibling color to red parent to black
                node->parent->color = 0;
                node->parent->parent->color = 1;
                leftRotate(node->parent->parent);
            }
        }
        else
        {
            uncle = node->parent->parent->right; // uncle

            if (uncle->color == 1)
            {
                // mirror of case 1
                // parent of node and uncle is black
                // uncle and parent becomes red and grandparent becomes black
                uncle->color = 0;
                node->parent->color = 0;
                node->parent->parent->color = 1;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->right)
                {
                    // mirror of case 2
                    // parent is left child of the grandparent and node is right child of parent
                    // do left rotation

                    node = node->parent;
                    leftRotate(node);
                }
                // mirror of case 3
                //  parent is left child of the grandparent and the node is left child of parent
                // right rotation at grandparent and change the sibling color to black parent to red
                node->parent->color = 0;
                node->parent->parent->color = 1;
                rightRotate(node->parent->parent);
            }
        }
        if (node == root)
        {
            break;
        }
    }
    root->color = 0;
}
void RBTree::leftRotate(GameNode *rotateNode)
{
    // ordinary BST left rotate
    GameNode *y = rotateNode->right;
    rotateNode->right = y->left;
    if (y->left != nill)
    {
        y->left->parent = rotateNode;
    }
    y->parent = rotateNode->parent;
    if (rotateNode->parent == NULL)
    {
        this->root = y;
    }
    else if (rotateNode == rotateNode->parent->left)
    {
        rotateNode->parent->left = y;
    }
    else
    {
        rotateNode->parent->right = y;
    }
    y->left = rotateNode;
    rotateNode->parent = y;
}
void RBTree::rightRotate(GameNode *rotateNode)
{
    // ordinary BST right rotate
    GameNode *y = rotateNode->left;
    rotateNode->left = y->right;
    if (y->right != nill)
    {
        y->right->parent = rotateNode;
    }
    y->parent = rotateNode->parent;
    if (rotateNode->parent == NULL)
    {
        this->root = y;
    }
    else if (rotateNode == rotateNode->parent->right)
    {
        rotateNode->parent->right = y;
    }
    else
    {
        rotateNode->parent->left = y;
    }
    y->right = rotateNode;
    rotateNode->parent = y;
}

GameNode RBTree::findEuBestSeller(GameNode *node, GameNode &max) // max node should be passed by reference to find maximumum by recursive
{
    if (node != NULL)
    {
        if (node->euSales > max.euSales) // find best seller recursively
        {
            max = *node;
        }
        findEuBestSeller(node->left, max);
        findEuBestSeller(node->right, max);
    }
    return max;
}
GameNode RBTree::findNaBestSeller(GameNode *node, GameNode &max) // max node should be passed by reference to find maximumum by recursive
{
    if (node != NULL)
    {
        if (node->naSales > max.naSales) // find best seller recursively
        {
            max = *node;
        }
        findNaBestSeller(node->left, max);
        findNaBestSeller(node->right, max);
    }
    return max;
}
GameNode RBTree::findOtherBestSeller(GameNode *node, GameNode &max) // max node should be passed by reference to find maximumum by recursive
{
    if (node != NULL)
    {
        if (node->otherSales > max.otherSales) // find best seller recursively
        {
            max = *node;
        }
        findOtherBestSeller(node->left, max);
        findOtherBestSeller(node->right, max);
    }
    return max;
}
void RBTree::printBestSeller(int region) // print wanted region's best seller
{
    GameNode max = *root;
    switch (region)
    {
    case 0:
    {
        GameNode na = findNaBestSeller(root, max);
        cout << "Best seller in North America: " << na.publisher << " - " << na.naSales << " million" << endl;
        break;
    }
    case 1:
    {
        GameNode na = findEuBestSeller(root, max);
        cout << "Best seller in Europe: " << na.publisher << " - " << na.euSales << " million" << endl;
        break;
    }
    case 2:
    {
        GameNode na = findOtherBestSeller(root, max);
        cout << "Best seller rest of the World: " << na.publisher << " - " << na.otherSales << " million" << endl;
        break;
    }
    }
}
void RBTree::readFile(const char *file)
{
    ifstream games(file);
    string read;
    // print end of decade conditions
    bool flag = false;
    bool f91 = false;
    bool f01 = false;
    bool f11 = false;
    if (games.is_open())
    {
        int attributeIndex = 0;
        string attribute = "";
        string name;
        string platform;
        string year;
        string publisher;
        string na;
        string eu;
        string other;
        while (getline(games, read)) // read line by line
        {
            name = "";
            platform = "";
            year = "";
            publisher = "";
            na = "";
            eu = "";
            other = "";
            if (flag)
            {
                char c;
                for (int i = 0; i < read.size(); i++) // read ith entry
                {
                    c = read[i];
                    if (c == ',') // if character c is tab char, an attribute is parsed. assign it to corresponding field.
                    {
                        switch (attributeIndex)
                        {
                        case 0:
                            name = attribute;
                            break;
                        case 1:
                            platform = attribute;
                            break;
                        case 2:
                            year = attribute;
                            break;
                        case 3:
                            publisher = attribute;
                            break;
                        case 4:
                            na = attribute;
                            break;
                        case 5:
                            eu = attribute;
                            break;
                        case 6:
                            other = attribute;
                            break;
                        }
                        attribute = "";   // clear attribute
                        attributeIndex++; // increase attribute index by one
                    }
                    else
                    {
                        attribute += c; // append attribute
                    }
                }
                other = attribute;
                attributeIndex = 0;
                attribute = "";
            }
            if (flag)
            {
                string endYear = "End of the " + year + " Year" + "\n";
                // print end of the decade best seller
                if (stoi(year) % 100 == 91 && !f91)
                {
                    cout << endYear;
                    printBestSeller(0);
                    printBestSeller(1);
                    printBestSeller(2);
                    f91 = true;
                }
                else if (stoi(year) % 100 == 1 && !f01)
                {
                    cout << endYear;
                    printBestSeller(0);
                    printBestSeller(1);
                    printBestSeller(2);
                    f01 = true;
                }
                else if (stoi(year) % 100 == 11 && !f11)
                {
                    cout << endYear;
                    printBestSeller(0);
                    printBestSeller(1);
                    printBestSeller(2);
                    f11 = true;
                }
                insert(GameNode(name, platform, stoi(year), // insert to the tree
                                publisher, stod(na), stod(eu), stod(other), BLACK));
            }
            flag = true;
        }
        games.close();
        cout << "End of the " << year << " Year" << endl;
        printBestSeller(0);
        printBestSeller(1);
        printBestSeller(2);
    }
}

int main(int argc, char const *argv[])
{
    RBTree tree = RBTree(argv[1]);             // create the tree
    int index = 0;                             // preorder starting index which is root's index
    tree.preOrderPrint(tree.getRoot(), index); // print tree in preorder
    return 0;
}
