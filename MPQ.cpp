#include "MPQ.h"

// Constructor
MPQ::MPQ(int maxLabelValue) : maxLabel(maxLabelValue), currentSize(0) {
    Heap.resize(maxLabelValue + 1);  // Index 0 is unused, we use 1-indexed heap
    Location.resize(maxLabelValue + 1, -1);  // -1 indicates label not in heap
}

// Destructor
MPQ::~MPQ() {
    // Vectors are automatically cleaned up
}

// Percolate up to maintain max heap property
void MPQ::percolateUp(int hole) {
    HeapItem temp = Heap[hole];
    
    // While not at root and parent is smaller
    while (hole > 1 && Heap[hole / 2].value < temp.value) {
        // Move parent down
        Heap[hole] = Heap[hole / 2];
        Location[Heap[hole].label] = hole;
        hole /= 2;
    }
    
    // Place the item in its final position
    Heap[hole] = temp;
    Location[temp.label] = hole;
}

// Percolate down to maintain max heap property
void MPQ::percolateDown(int hole) {
    HeapItem temp = Heap[hole];
    int child;
    
    while (hole * 2 <= currentSize) {
        child = hole * 2;
        
        // Choose the larger child
        if (child != currentSize && Heap[child + 1].value > Heap[child].value) {
            child++;
        }
        
        // If child is larger than temp, move child up
        if (Heap[child].value > temp.value) {
            Heap[hole] = Heap[child];
            Location[Heap[hole].label] = hole;
            hole = child;
        } else {
            break;
        }
    }
    
    // Place temp in its final position
    Heap[hole] = temp;
    Location[temp.label] = hole;
}

// Insert an item with given value and label
void MPQ::insert(int value, int label) {
    if (label < 0 || label > maxLabel) {
        throw std::out_of_range("Label out of range");
    }
    
    if (Location[label] != -1) {
        throw std::runtime_error("Label already exists in MPQ");
    }
    
    currentSize++;
    
    // Insert at the end and percolate up
    Heap[currentSize] = HeapItem(value, label);
    Location[label] = currentSize;
    percolateUp(currentSize);
}

// Remove and return the value with this label
int MPQ::Remove(int label) {
    if (label < 0 || label > maxLabel) {
        throw std::out_of_range("Label out of range");
    }
    
    if (Location[label] == -1) {
        throw std::runtime_error("Label not found in MPQ");
    }
    
    int position = Location[label];
    int value = Heap[position].value;
    
    // Mark label as removed
    Location[label] = -1;
    
    // If removing the last element, just decrease size
    if (position == currentSize) {
        currentSize--;
        return value;
    }
    
    // Move last element to the removed position
    Heap[position] = Heap[currentSize];
    Location[Heap[position].label] = position;
    currentSize--;
    
    // Fix heap property - try both directions
    if (currentSize > 0) {
        // If moved element is larger than parent, percolate up
        if (position > 1 && Heap[position].value > Heap[position / 2].value) {
            percolateUp(position);
        } else {
            // Otherwise percolate down
            percolateDown(position);
        }
    }
    
    return value;
}

// Return the maximum value currently stored
int MPQ::GetMax() {
    if (IsEmpty()) {
        return 0;  // Return 0 for empty heap (ground level)
    }
    return Heap[1].value;
}

// Check if the MPQ is empty
bool MPQ::IsEmpty() const {
    return currentSize == 0;
}