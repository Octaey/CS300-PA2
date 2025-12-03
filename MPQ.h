#ifndef MPQ_H
#define MPQ_H

#include <stdexcept>

// Modified Priority Queue Class
// Maintains items with both a value (for comparison) and a label (for identification)
class MPQ {
private:
    struct HeapItem {
        int value;  // The comparable value (building height)
        int label;  // The identifying number (building ID)
        
        HeapItem(int v = 0, int l = -1) : value(v), label(l) {}
    };
    
    HeapItem* Heap;                   // The heap array (1-indexed)
    int* Location;                    // Maps label to position in heap
    int currentSize;                  // Current number of items in heap
    int maxLabel;                     // Maximum label value for Location array size
    int capacity;                     // Capacity of the heap array
    
    // Helper methods for heap operations
    void percolateUp(int hole);
    void percolateDown(int hole);
    
public:
    // Constructor
    MPQ(int maxLabelValue = 1000);
    
    // Destructor
    ~MPQ();
    
    // Insert an item with given value and label
    void insert(int value, int label);
    
    // Remove and return the value with this label
    int Remove(int label);
    
    // Return the maximum value currently stored (without removing)
    int GetMax();
    
    // Check if the MPQ is empty
    bool IsEmpty() const;
    
    // Get current size (useful for debugging)
    int size() const { return currentSize; }
};

#endif // MPQ_H