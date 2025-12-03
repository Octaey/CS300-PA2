#include "MPQ.h"

// constructor
MPQ::MPQ(int maxLabelValue) : currentSize(0), maxLabel(maxLabelValue), capacity(maxLabelValue + 1) {
    Heap = new HeapItem[capacity];  // index 0 is unused, we use 1-indexed heap
    Location = new int[capacity];
    
    // initialize location array to -1 (indicates label not in heap)
    for (int i = 0; i < capacity; i++) {
        Location[i] = -1;
    }
}

// destructor
MPQ::~MPQ() {
    delete[] Heap;
    delete[] Location;
}

// percolate up to maintain max heap property
void MPQ::percolateUp(int hole) {
    HeapItem temp = Heap[hole];
    
    // while not at root and parent is smaller
    while (hole > 1 && Heap[hole / 2].value < temp.value) {
        // move parent down
        Heap[hole] = Heap[hole / 2];
        Location[Heap[hole].label] = hole;
        hole /= 2;
    }
    
    // place the item in its final position
    Heap[hole] = temp;
    Location[temp.label] = hole;
}

// percolate down to maintain max heap property
void MPQ::percolateDown(int hole) {
    HeapItem temp = Heap[hole];
    int child;
    
    while (hole * 2 <= currentSize) {
        child = hole * 2;
        
        // choose the larger child
        if (child != currentSize && Heap[child + 1].value > Heap[child].value) {
            child++;
        }
        
        // if child > temp, move child up
        if (Heap[child].value > temp.value) {
            Heap[hole] = Heap[child];
            Location[Heap[hole].label] = hole;
            hole = child;
        } else {
            break;
        }
    }
    
    // place temp in its final position
    Heap[hole] = temp;
    Location[temp.label] = hole;
}

// insert item with given value and label
void MPQ::insert(int value, int label) {
    if (label < 0 || label > maxLabel) {
        throw std::out_of_range("Label out of range");
    }
    
    if (Location[label] != -1) {
        throw std::runtime_error("Label already exists in MPQ");
    }
    
    currentSize++;
    
    // insert at the end and percolate up
    Heap[currentSize] = HeapItem(value, label);
    Location[label] = currentSize;
    percolateUp(currentSize);
}

// remove and return the value with this label
int MPQ::Remove(int label) {
    if (label < 0 || label > maxLabel) {
        throw std::out_of_range("Label out of range");
    }
    
    if (Location[label] == -1) {
        throw std::runtime_error("Label not found in MPQ");
    }
    
    int position = Location[label];
    int value = Heap[position].value;
    
    // mark label as removed
    Location[label] = -1;
    
    // if removing last element just decrease size
    if (position == currentSize) {
        currentSize--;
        return value;
    }
    
    // move last element to removed position
    Heap[position] = Heap[currentSize];
    Location[Heap[position].label] = position;
    currentSize--;
    
    // fix heap property, try both directions
    if (currentSize > 0) {
        // if moved element is larger than parent percolate up, otherwise percolate down
        if (position > 1 && Heap[position].value > Heap[position / 2].value) {
            percolateUp(position);
        } else {
            percolateDown(position);
        }
    }
    
    return value;
}

// return max value currently stored
int MPQ::GetMax() {
    if (IsEmpty()) {
        return 0;
    }
    return Heap[1].value;
}

// check if MPQ is empty
bool MPQ::IsEmpty() const {
    return currentSize == 0;
}