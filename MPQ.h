#ifndef MPQ_H
#define MPQ_H

#include <stdexcept>

// modified prio queue class
class MPQ {
private:
    struct HeapItem {
        int value;  // building height
        int label;  // building ID
        
        HeapItem(int v = 0, int l = -1) : value(v), label(l) {}
    };
    
    HeapItem* Heap;     // heap array (1-indexed)
    int* Location;      // maps label to position in heap
    int currentSize;    // current num of items in heap
    int maxLabel;       // max label value for location array size
    int capacity;       // capacity of heap array
    
    // helper methods for heap operations
    void percolateUp(int hole);
    void percolateDown(int hole);
    
public:
    // constructor
    MPQ(int maxLabelValue = 1000);
    
    // destructor
    ~MPQ();
    
    // insert item with given value and label
    void insert(int value, int label);
    
    // remove and return value with this label
    int Remove(int label);
    
    // return max value currently stored (without removing)
    int GetMax();
    
    // check if MPQ is empty
    bool IsEmpty() const;
    
    // get current size (good for debugging)
    int size() const { return currentSize; }
};

#endif // MPQ_H