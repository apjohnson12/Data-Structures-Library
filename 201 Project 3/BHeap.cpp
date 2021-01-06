#include "CircularDynamicArray.cpp"

using namespace std;

template <class keytype, class valuetype>

//struct for the BHeapNodes in the BHeap
struct BHeapNode {
    keytype key;
    valuetype value;
    int degree;
    int length;
    int min;
    CircularDynamicArray <BHeapNode<keytype, valuetype> *> child;
    // bool operator< (BHeapNode <keytype, valuetype> right) {
    //     return this->degree < right.degree;
    // }
};

// template <class keytype, class valuetype>
// typedef Node BHeapNode<keytype, valuetype>*;
//
// template <class keytype, class valuetype>
// bool operator< (Node<keytype, valuetype> const& left, Node<keytype, valuetype> const& right)  {
//     return left->degree < right->degree;
// }

template <class keytype, class valuetype>

class BHeap
{
    private:

    CircularDynamicArray <BHeapNode<keytype, valuetype> *> _root;
    int _length;
    int _degree;
    int _min;

    //private insert function for adding nodes to the BHeap
    void _insert(BHeapNode<keytype, valuetype> *node) {
        //initially adds the node to the front of the CDA
        _root.addFront(node);
        _min = 0;
        _degree = _root[0]->degree + 1;
        for (int i = 1; i < _root.length(); i++)
        {
            //checks to see if there are two nodes of the same degree and merges them together
            //to fit the rules of the binomial heap
            if (node->degree == _root[i]->degree)
            {
                node = _merge(node, _root[i]);
                _root.delFront();
                _root.addFront(node);
                for (int j = i + 1; j < _root.length(); j++)
                {
                    _root[j - 1] = _root[j];
                }
                _root.delEnd();
                i = 0;
            }
            //updates the min after an inset
            if (_root[i]->key < _root[_min]->key)
            {
                _min = i;
            }
            //updates the root after an insert
            if (_root[i]->degree + 1 > _degree) {
                _degree = _root[i]->degree + 1;
            }
        }
    }

    //merge function that takes two nodes of the same degree and merges them together
    BHeapNode <keytype, valuetype> *_merge(BHeapNode <keytype, valuetype> *left, BHeapNode <keytype, valuetype> *right){
        //checks to see which node key is less, thus resulting in it being merges into the other node
        //adds to the front of the node's array after being merged
        if (left->key < right->key) {
            left->child.addFront(right);
            left->length += right->length;
            if (left->degree < right->degree + 1){
                left->degree = right->degree + 1;
            }
            return left;
        }
        else {
            right->child.addFront(left);
            right->length += left->length;
            if (right->degree < left->degree + 1){
                right->degree = left->degree + 1;
            }
            return right;
        }
    }

    //printkey helper function
    void _printKey(BHeapNode <keytype, valuetype> *node, bool first) {
        if (!first) {
            cout << " ";
        }
        cout << node->key;
        //cout << "@" << node->degree << "#" << node->child.length();
        
        //prints out the tree of the called node
        for (int i = 0; i < node->child.length(); i++) {
            _printKey(node->child[i], false);
        }
    }

    //helper function used by destructor to delete the nodes from memory
    void _delete_child(BHeapNode<keytype, valuetype> *node) {
        for(int i = 0; i < node->child.length(); i++) {
            _delete_child(node->child[i]);
        }
        node = nullptr;
        delete node;
    }

    public:

    //general constructor for creating an empty BHeap object
    BHeap() {
        _root = CircularDynamicArray<BHeapNode<keytype, valuetype> *>();
        _length = 0;
        _degree = 0;
        _min = -1;
    }

    //Overloaded constructor that builds the BHeap using the arrays k and v containing s items of keytype and valuetype
    //implemented using repeated insertion
    BHeap(keytype k[], valuetype v[], int s) : BHeap() {

      for (int i = 0; i < s; i++) {
        insert(k[i], v[i]);
        // BHeapNode <keytype, valuetype> *node = new BHeapNode <keytype, valuetype>;
        // node->key = k;
        // node->length = 1;
        // node->value = v;
        // node->degree = 0;
        // node->child = CircularDynamicArray <BHeapNode<keytype, valuetype> *>();
        // _length++;
      }
    }

    //General copy constructor 
    BHeap(const BHeap <keytype, valuetype> &original) {
        _root = original._root;
        _length = original._length;
        _degree = original._degree;
        _min = original._min;
        // for (int i = 0; i < original._root.length(); i++) {
        //     _root.addFront(_copy(original._root[i]));
        // }
    }

    //general copy assignment operator
    BHeap operator=(const BHeap<keytype, valuetype> &original) {
        if (this != &original) {
            _root = original._root;
            _length = original._length;
            _degree = original._degree;
            _min = original._min;
        }
        else {
            return *this;
        }

        return *this;
    }

    //general destructor that deletes the BHeap from memory
    ~BHeap() {
      for(int i = 0; i < _root.length(); i++) {
        _delete_child(_root[i]);
      }
    }

    //function that writes the keys stored in the heap, printing the smallest binomial tree first. Prints the size of the tree first
    // and then uses a modified preorder traversal of the tree
    void printKey() {
        int order[_degree];
        //bool first = true;
        for (int i = 0; i < _degree; i++) {
          order[i] = -1;
        }
        for (int i = 0; i < _root.length(); i++) {
          order[_root[i]->degree] = i;
        }

        for (int i = 0; i < _degree; i++) {
            if (order[i] < 0) {
              continue;
            }
            // if (!first) {
            //   cout << ", ";
            // }
            //first = false;
            // cout << order[i];

            //goes through and calls helper function to print out tree for each node
            cout << "B" << _root[order[i]]->degree << endl;
            _printKey(_root[order[i]], true);
            cout << endl << endl;
        }
        
    }

    //function that creates a new node with key k and value v into the BHeap and returns the key
    void insert(keytype k, valuetype v) {
        BHeapNode <keytype, valuetype> *node = new BHeapNode <keytype, valuetype>;
        node->key = k;
        node->length = 1;
        node->value = v;
        node->degree = 0;
        node->child = CircularDynamicArray <BHeapNode<keytype, valuetype> *>();
        _length++;
        _insert(node);
    }

    //Returns the minimum key in the BHeap without modifying the BHeap
    keytype peakKey() {
        return _root[_min]->key;
    }

    //returns the value associated with the minimum key in the BHeap without modifying the BHeap
    valuetype peakValue() {
        return _root[_min]->value;
    }

    //Removes the minimum key in the BHeap and returns the key
    keytype extractMin() {
        keytype extract = _root[_min]->key;
        BHeapNode<keytype, valuetype> *node = _root[_min];

        //sets the minimum key to be the end of the array and then calls a delete end
        for (int i = _min + 1; i < _root.length(); i++) {
            _root[i - 1] = _root[i];
        }
        _root.delEnd();
        _length--;
        //inserts immediate child of the deleted node into the BHeap
        for (int i = 0; i < node->child.length(); i++) {
            _insert(node->child[i]);
        }
        return extract;
    }

    //Merges another BHeap object into the current BHeap
    void merge(BHeap<keytype, valuetype> &h2) {
       // cout << h2._root.length() << endl;
        //for(int i = 0; i < h2._root.length(); i++) {
        while(h2._root.length() > 0) {
            //cout << i << ":" << h2._root[i]->degree << ":";
            //h2._printKey(h2._root[i], true);
            //cout << endl;
//            _insert(h2._root[i]);

            //inserting each root node of h2 into the current BHeap
            _insert(h2._root[0]);
//            _length += h2._root[i]->length;
            _length += h2._root[0]->length;
            h2._length -= h2._root[0]->length;
            
            //deleted the BHeap object that is being merged into the current BHeap
            h2._root.delFront();
            //printKey();
        }
        h2._degree = 0;
        h2._min = -1;
        //cout << _root.length() << endl;
    }

    int length() {
       return _length;
    }
};
