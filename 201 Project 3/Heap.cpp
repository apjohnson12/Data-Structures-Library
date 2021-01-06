#include "CircularDynamicArray.cpp"

template <class keytype, class valuetype>

// struct for the heap nodes containing generics for a key and a value.
struct HeapNode
{
    keytype key;
    valuetype value;
};
template <class keytype, class valuetype>
class Heap
{
private:
    CircularDynamicArray<HeapNode<keytype, valuetype>> heap;

    // index for parents
    static int _parent(int index) 
    {
        return (index - 1) / 2;
    }

    //index for left children
    static int _leftChild(int index)
    {
        return index * 2 + 1;
    }

    // index for right children
    static int _rightChild(int index)
    {
        return index * 2 + 2;
    }

    // basic swap function
    void _swap(int left, int right) {
        //cout << "swap : " << left << " and " << right << endl;
        HeapNode<keytype, valuetype> temp = heap[left];
        heap[left] = heap[right];
        heap[right] = temp;
    }

    //bottom up approach to a heapify function that modifies the array to match min heap rules
    void _heapifyFromArray(int index) {
        int l = _leftChild(index);
        int r = _rightChild(index);

        int smallest;

        //checks to see if either children are smaller than the parent and if so, swaps them and 
        //calls itself recursively
        if (l < heap.length() && heap[l].key < heap[index].key) {
            smallest = l;
        }
        else {
            smallest = index;
        }

        if (r < heap.length() && heap[r].key < heap[smallest].key) {
            smallest = r;
        }
        if (smallest != index) {
            _swap(index, smallest);
            _heapifyFromArray(smallest);
        }
    }

    //top down approach to a heapify function
    void _heapify(int index)
    {
        
        int parent = _parent(index);
        //cout << "in heapify : " << heap[index].key << " : " << heap[parent].key << endl;
        // if (heap[parent].key > 0) {

        //checks to see if the index is smaller than its parent ad swaps if thats true, then
        //recursively calls itself
        if (heap[index].key < heap[parent].key) {
            _swap(index, parent);
            _heapify(parent);
        }
        //}

        // int left = _leftChild(index);
        // int right = _rightChild(index);
        // int min = index;
        // if (left < heap.length() && heap[left].key < heap[min].key) {
        //     min = left;
        // }
        // if (right < heap.length() && heap[right].key < heap[min].key) {
        //     min = right;
        // }
        // if (min != index) {
        //     _swap(min, index);
        //     _heapify(min);
        // }


    }

    // Function that writes the keys stored in the array, starting at the root
    void _printKey(int index)
    {
        if (index >= heap.length()) {
            return;
        }
        cout << ", " << heap[index].key;
        _printKey(_leftChild(index));
        _printKey(_rightChild(index));
    }
public:

    //General constructor for the heap class
    Heap()
    {
        //Generates a Circular dynamic array object
        heap = CircularDynamicArray<HeapNode<keytype, valuetype>>();
        //makes sure that the CDA is empty
        while (heap.length() > 0) {
            heap.delEnd();
        }
    }

    //Overloaded constructor for heap object that allows user to set keys, values, and size while 
    //creating object
    Heap(keytype keys[], valuetype values[] , int s): Heap() 
    {
        //add the keys and values to the heap object by appending them to the end of the array
        for (int i = 0; i < s; i++) {
            heap.addEnd({keys[i], values[i]});
        }
        //fixes up the CDA so that it follows min heap structure, using a bottom up approach
        for (int i = (s - 1)/2; i>=0; i--) {
            _heapifyFromArray(i);
        }
    }

    //General copy constructor for the heap class
    Heap(const Heap<keytype, valuetype> &original)
    {
        heap = original.heap;
        //heap = CircularDynamicArray<HeapNode<keytype, valuetype>>();
        // while (heap.length() > 0){
        //     heap.delEnd();
        // }
        //heap = CircularDynamicArray<HeapNode<keytype, valuetype> *>(original.heap.length());
        // for (int i = 0; i < original.length(); i++) {
        //     //heap[i] = original.heap[i];
        //     heap.addEnd(original.heap[i]);
        // }
    }

    //General assignment operator for the heap class
    Heap &operator=(const Heap<keytype, valuetype> &original)
    {
        //calling the destructor to make sure new object is empty
        heap.~CircularDynamicArray();
        if (this != &original) {
            heap = original.heap;
        }
        else {
            return *this;
        }
        // delete heap;
        
        // heap = CircularDynamicArray<HeapNode<keytype, valuetype>>(original.heap.length());
        // for (int i = 0; i < original.heap.length(); i++) {
        //     heap[i] = original.heap[i];
        // }
        return *this;
    }

    ~Heap()
    {
        //Destructor is handled by the CDA class
        //heap.~CircularDynamicArray();
    }

    //function for inserting a node into the heap, appends it to the end of the array and then calls heapify
    void insert(keytype key, valuetype value)
    {
        
        heap.addEnd({key, value});
        _heapify(heap.length() - 1);
        // for (int i = heap.length() - 1; i != 0 && heap[i].key < heap[_parent(i)].key; i = _parent(i)) {
        //     if (heap[i].key > heap[_leftChild(_parent(i))].key){
        //         _swap(i, _parent(i));
        //     }
        //     else {
        //         _swap(_leftChild(_parent(i)), _parent(i));
        //     }
            
        // }
    }

    //returns the minimum key in the heap without modifying the heap
    keytype peakKey()
    {
        return heap[0].key;
    }

    //returns the value associated with the minimum key in the heap without
    //modifying the heap
    valuetype peakValue()
    {
        return heap[0].value;
    }

    //Removes the minimum key in the heap and returns the key
    keytype extractMin()
    {
        keytype min = peakKey();

        //sets minimum value to be the end of the array
        if (heap.length() > 1) {
            heap[0] = heap[heap.length() - 1];
        }
        //deletes the end of the array
        heap.delEnd();
        //fixes up the heap after the extraction of the min
        if (heap.length() > 1) {
            _heapifyFromArray(0);
        }
        return min;
    }

    //writes the keys stored in the array, starting at the root
    void printKey()
    {
        for (int i = 0; i < heap.length(); i++) {
            if (i != 0) {
                cout << ", ";
            
            }
            cout << heap[i].key;
        }
        cout << endl;
        // if (heap.length() == 0) {
        //     return;
        // }
        // cout << heap[0].key;
        // _printKey(_leftChild(0));
        // _printKey(_rightChild(0));
    }

    int length()
    {
        return heap.length();
    }
};