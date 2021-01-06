#include <iostream>

using namespace std;

template <class keytype, class valuetype>
//template <class valuetype>



class RBTree 
{
private:

    struct RBNode
{
    keytype key;
    valuetype value;
    //int rank;
    //RBNode *parent;
    RBNode *left;
    RBNode *right;
    bool red; // true for red. false for black
    int size; // 1 + left->size, + right->size
    //int height; // 1 + max(left->height, right->height)
};

    RBNode *_root;
    keytype keyError;
    valuetype valueError;
    bool const VERBOSE = false;
    void _deleteNodes(RBNode *node)
    {
        //cout << "in delete nodes function" << endl;
        if (node == nullptr) {
            return;
        }
        _deleteNodes(node->left);
        _deleteNodes(node->right);
        delete node;
    }
    valuetype * _search(keytype k, RBNode *node)
    {
        if (node == nullptr) {
            return nullptr;
        }
        if (node->key == k) {
            return &node->value;
        }
        if (k < node->key) {
            return _search(k, node->left);
        }
        else {
            return _search(k, node->right);
        }
    }

    RBNode *_moveRedLeft(RBNode *node){
        //cout << "in moveRedLeft" << endl;
        _colorFlip(node);
        if (node->right && _isRed(node->right->left)){
            node->right = _rotateRight(node->right);
            node = _rotateLeft(node);
            _colorFlip(node);
        }
        return node;
    }

    RBNode *_moveRedRight(RBNode *node){
        //cout << "in moveRedRight" << endl;
        _colorFlip(node);
        if (node->left && _isRed(node->left->left)){
            node = _rotateRight(node);
            _colorFlip(node);
        }
        _update(node);
        return node;
    }

    keytype _min(RBNode *node){
        
        //cout << "in min function" << endl;
        
        while (node->left != nullptr){
            
            node = node->left;
        } 
        if (node == nullptr){
            return keyError;
            
        }
        
        
        return node->key;
    }

    valuetype _get(RBNode *node, keytype key){
        //cout << "in get function" << endl;
        while (node != nullptr){
            if (key == node->key){
                return node->value;
            }
            if (key < node->key){
                node = node->left;
            }
            else {
                node = node->right;
            }
        }
        return valueError;
    }

    RBNode *_deleteMin(RBNode *node){
        //cout << "in deleteMin" << endl;

        if (node->left == nullptr){
            return nullptr;
        }
        if (!_isRed(node->left) && node->left != nullptr && !_isRed(node->left->left) ){//&& node->left->left != nullptr){
            node = _moveRedLeft(node);
        }

        node->left = _deleteMin(node->left);

        return _fixUp(node);
    }

    RBNode *_fixUp(RBNode * node){
        //cout << "in fix up" << endl;
        if (_isRed(node->right)){// && node->right != nullptr){
            node = _rotateLeft(node);
        }
        if (_isRed(node->left) && node->left != nullptr && _isRed(node->left->left)){//&& node->left->left != nullptr){
            node = _rotateRight(node);
        }
        if(_isRed(node->left) && _isRed(node->right)){// && node->left != nullptr && node->right != nullptr){
            _colorFlip(node);
        }
        _update(node);
        return node;
    }

    RBNode *_remove(keytype k, RBNode *node)
    {
        //cout << "in helper remove" << endl;
        if (node == nullptr) {
            cout << "in first if" << endl;
            return nullptr;
        }
        //cout << "remove " << node->key << " " << k << endl;
        //cout << "after first if statement" << endl;
        //cout << "pre key" << endl;
        keytype key = node->key;
        //cout << "key : " << key << endl;
        if (k < key){
            //cout << "in second if statement" << endl;
            if (node->left != nullptr && !_isRed(node->left) && !_isRed(node->left->left) ){//&& node->left->left != nullptr){
                //cout << "in nested if" << endl;
                node = _moveRedLeft(node);
            }
            //cout << "after nested if statement" << endl;
            if (node->left != nullptr){
                node->left = _remove(k, node->left);
            }
            _update(node->left);
            //cout << "after suspected error" << endl;
        }
        else {
            //cout << "in else " << endl;
            if (_isRed(node->left)){
                //cout << "in else fist if" << endl;
                node = _rotateRight(node);
            }
            

            if (k == key && (node->right == nullptr)){
                //cout << "in else second if" << endl;
                return node->left;
            }

            if (!_isRed(node->right) && node->right != nullptr && !_isRed(node->right->left)){// && node->right->left != nullptr && node->left != nullptr){
                //cout << "in else third if" << endl;
                node = _moveRedRight(node);
            }

            if (k == key){
                //cout << "found key" << endl;
                node->key = _min(node->right);
                node->value = _get(node->right, node->key);
                node->right = _deleteMin(node->right);
            }
            else {
                //cout << "in second else in remove function" << endl;
                node->right = _remove(k, node->right);
            }
            _update(node->right);
            _update(node);
            return _fixUp(node);
        }
       /* if (node->_key == k) {
            if (node->left == nullptr || node->right == nullptr) {
                if (node->_left != nullptr) {
                    RBNode *temp = node->left;
                    temp->parent = node->parent;
                    delete node;
                    _update(temp);
                    return temp;
                }
                else {
                    RBNode *temp = node->right;
                    temp->parent = node->parent;
                    delete node;
                    _update(temp);
                    return temp;
                }
            }
            RBNode *minRight = node->right;
            RBNode *prev = nullptr;
            while (minRight != nullptr) {
                prev = minRight; 
                minRight = minRight->left;
            } 
            prev->parent = node->parent;
            prev->left = node->right;
            delete node;
            _update(prev);
            return prev;
        }
        if (node->key > k) {
            node->right = _remove(k, node->right);
        }
        else {
            node->left = _remove(k, node->left);
        }
        _update(node);
        return node; */
        return node;
    }
    RBNode *_insert(RBNode *root, RBNode *node)
    {
        //cout << "arrived in insert helper" << endl;
        if (root == nullptr) {
            //cout << "root was null" << endl;
            return node;
        }
        //cout << "here " << endl;
        if (node->key == root->key) { // replace earlier value
            root->value = node->value;
            delete node; 
            return root;
        }
        node->red = true;
        
        if (_isRed(root->left) && _isRed(root->right)){
            _colorFlip(root);
        }
        if (node->key < root->key ) {
            root->left = _insert(root->left, node);
        }
        else {
            root->right = _insert(root->right, node);
        }
        _update(root);
        //cout << root->key << " " << root->parent << endl;
        RBNode *old = root;
        if (_isRed(root->right) && !_isRed(root->left)){
            root = _rotateLeft(root);
            if (old == _root){
                _root = root;
            }
            _update(root);
        }
        old = root;
        if (_isRed(root->left) && _isRed(root->left->left) && node->left != nullptr){
            root = _rotateRight(root);
             if (old == _root){
                _root = root;
            }
            _update(root);
        }
       
      
        return root;
    } 

    bool _isRed(RBNode *node){
        //cout << "in isRed" << endl;
        return node != nullptr && node->red;
    }

    void _colorFlip(RBNode *node){
        //cout << "in color flip" << endl;
        //cout << "in color flip : " << node->key <<endl;
        
        //cout << node->key << " " << node->parent << endl;
        /*if (node == _root){
            //cout << "in if statement" << endl;
            return;
        }*/
        /*if (node == _root){
            node->red = false;
        }*/
        
        //node->red = !node->red;
        if (node != nullptr && node->left != nullptr && node->right != nullptr){
            node->red = !node->red;
            node->left->red = !node->left->red;
            node->right->red = !node->right->red;
        }
        
        //cout << "end of color flip" << endl;
    }

    RBNode *_rotateLeft(RBNode *node){
        //cout << "rotate left" << endl;
        /*cout << "RL Before : " << node->key << "(";
        if (node->left != nullptr){
            cout << node->left->key;
        }*/
        //cout << ",";
        //cout << node->right->key << ")" << endl;
        /*if (!node->right || !node->left || !node->right->left){
            return node;
        }*/
        RBNode *temp = node->right;
        node->right = temp->left;
        temp->left = node;
        
        temp->red = temp->left->red;
        temp->left->red = true;
        _update(node);
        _update(temp);
        /*cout << "RL After : " << temp->key << "(";
        cout << temp->left->key << ",";
        if (temp->right != nullptr){
            cout << temp->right->key;
        }
        cout << ")" << endl;*/
     
        return temp;
    }

    RBNode *_rotateRight(RBNode *node){
        //cout << "rotate right" << endl;
        /*if (!node->left || !node->right || !node->left->right){
            return node;
        }*/
        RBNode *temp = node->left;
        node->left = temp->right;
        temp->right = node;
        
        temp->red = temp->right->red;
        temp->right->red = true;
        _update(node);
        _update(temp);
       
        return temp;
    }

    /*int _rank(RBNode *node){
        if (node == nullptr){
            return 1;
        }
        return node->rank;
    }*/

    int _rank(keytype k, RBNode *root, int pos){
        if (root == nullptr) {
            return -1;
        }
        if (root->key == k) {
            return pos+_size(root->left);
        }
        if (k < root->key) {
            return _rank(k, root->left, pos);
        }
        else {
            return _rank(k, root->right, _size(root->left)+pos+1);
        }
    }

    int _size(RBNode *node){
        if (node == nullptr){
            return 0;
        }
        return node->size;
    }

    keytype _select(int pos, RBNode *root){
        //cout << pos << " " << root->key << endl;
        if (_size(root->left) == pos){
            return root->key;
        }
        if (root->left->size >= pos){
            return _select(pos, root->left);
        }

        else {
            return _select((pos-1) - root->left->size, root->right);
        }
    }

    void _update(RBNode *node)
    {
        //cout << "in update" << endl;
        
        if (node == nullptr){
            return;
        }
        node->size = 1;
        //node->rank = (node->red)?0:1;
        //node->rank += max(_rank(node->left), _rank(node->right));
        node->size = _size(node->left) + _size(node->right) + 1;
        //node->height = 1;
        //if (node->left != nullptr) {
            //node->size += node->left->size;
            //node->left->parent = node;
            /*if (node->left->height + 1 > node->height) {
                node->height = node->left->height + 1;
            }*/
        //}
        //if (node->right != nullptr) {
            //node->size += node->right->size;
            //node->right->parent = node;
            /*if (node->right->height + 1 > node->height) {
                node->height = node->left->height + 1;
            }*/
        //}

    }

    void _preorder(RBNode *node, int h){
        if (node == nullptr){
            return;
        }
        if (VERBOSE){
            for (int i = 0; i < h; i++){
                cout << "-";
            }
        }
        cout << node->key;
        if (VERBOSE){
            cout << ":" << node->value;
            cout << " " << node->size;
            //cout << " " << node->rank;
            cout << " " << ((node->red)?"red":"black");
            cout << endl;
        }
        else {
            cout << " ";
        }
        if (node->left != nullptr && VERBOSE){
            cout << "L";
        }
        _preorder(node->left, h+1);
        if (node->right != nullptr && VERBOSE){
            cout << "R";
        }
        _preorder(node->right, h+1);
    }

    void _inorder(RBNode *node){
        if (node == nullptr){
            return;
        }
        _inorder(node->left);
        cout << node->key;
        if (VERBOSE){
            cout << " " << node->value;
            cout << " " << node->red;
            cout << endl;
        }
        else {
            cout << " ";
        }
        _inorder(node->right);
    }

    void _postorder(RBNode *node){
        if (node == nullptr){
            return;
        }
        _postorder(node->left);
        _postorder(node->right);
        cout << node->key;
        if (VERBOSE){
            cout << " " << node->value;
            cout << " " << node->red;
            cout << endl;
        }
        else {
            cout << " ";
        }
        
    }

    RBNode *_copy(RBNode *root){
        //cout << root->key << endl;
        RBNode *newNode = new RBNode;
        newNode->key = root->key;
        newNode->value = root->value;
        newNode->red = root->red;
        newNode->size = root->size;
        newNode->left = nullptr;
        newNode->right = nullptr;
        if (root->left != nullptr){
            newNode->left = _copy(root->left);
        }
        if (root->right != nullptr){
            newNode->right = _copy(root->right);
        }
        return newNode;
    }

public:

    RBTree()
    {
        //cout << "default constructor" << endl;
        _root = nullptr;
    }

    RBTree(keytype k[], valuetype V[], int s)
    {
        //cout << "in the constructor" << endl;
        _root = nullptr;
        //cout << "return from the default constructor" << endl;
        for (int i = 0; i < s; i++) {
            insert(k[i], V[i]);
        }
    }

    ~RBTree()
    {
        _deleteNodes(_root);
    }

    RBTree(const RBTree<keytype, valuetype> &original){
        //cout << "copy constructor" << endl;
        //root.preorder();
        _root = _copy(original._root);
    }

    RBTree& operator=(const RBTree<keytype, valuetype> &original){
        _deleteNodes(_root);
        if (this != &original){
            _root = _copy(original._root);
        }

        return *this;
        
    }

    valuetype * search(keytype k)
    {
        return _search(k, _root);
    }

    void insert(keytype k, valuetype v)
    {
        //cout << "in the insert function" << endl;
        
        RBNode *newNode = new RBNode;
        newNode->left = nullptr;
        newNode->right = nullptr;
        //newNode->parent = nullptr;
        newNode->key = k;
        newNode->value = v;
        newNode->size = 1;
        //newNode->rank = 1;
        //newNode->height = 1;
        newNode->red = false;
        //cout << "new node created" << endl;
        _root = _insert(_root, newNode);
        _root->red = false;
        _update(_root);
        //cout << "returned from helper" << endl;
    }

    int remove(keytype k)
    {
        int oldSize = size();
        _root = _remove(k, _root);
        //cout << "removed" << endl;
        _root->red = false;
        //_root->parent = nullptr;
        _update(_root);
        return size() < oldSize;
        
    }

    int rank(keytype k)
    {
        return _rank(k, _root, 0) + 1;
    }

    keytype select(int pos)
    {
       return _select(pos-1, _root); 
    }

    void split(keytype k, RBTree<keytype,valuetype>& T1, RBTree<keytype,valuetype>& T2)
    {
        return;
    }

    int size()
    {
        if (_root == nullptr) {
            return 0;
        }
        return _root->size;
    }

    void preorder()
    {
        _preorder(_root, 0);
        if (!VERBOSE){
            cout << endl;
        }  
    }

    void inorder()
    {
        _inorder(_root);
        if (!VERBOSE){
            cout << endl;
        } 
    }

    void postorder()
    {
        _postorder(_root);
        if (!VERBOSE){
            cout << endl;
        } 
    }
};