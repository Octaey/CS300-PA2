#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "MPQ.h"

using namespace std;

// Structure to represent a building
struct Building {
    int left;
    int height;
    int right;
    int id;  // Building identifier
    
    Building(int l = 0, int h = 0, int r = 0, int i = 0) 
        : left(l), height(h), right(r), id(i) {}
};

// Structure to represent an x-coordinate event
struct Event {
    int x;          // x-coordinate
    int buildingId; // which building this event belongs to
    bool isLeft;    // true if left edge, false if right edge
    int height;     // height of the building (for sorting ties)
    
    Event(int xCoord = 0, int id = 0, bool left = true, int h = 0)
        : x(xCoord), buildingId(id), isLeft(left), height(h) {}
    
    // Comparison operator for sorting
    bool operator<(const Event& other) const {
        if (x != other.x) {
            return x < other.x;
        }
        // If at same x-coordinate:
        // Process left edges before right edges
        if (isLeft != other.isLeft) {
            return isLeft > other.isLeft;
        }
        // If both are left edges, process taller buildings first
        if (isLeft) {
            return height > other.height;
        }
        // If both are right edges, process shorter buildings first
        return height < other.height;
    }
};

int main() {
    // Step 1: Read input from file
    ifstream input("input.txt");
    if (!input.is_open()) {
        cerr << "Error: Could not open input.txt" << endl;
        return 1;
    }
    
    int numBuildings;
    input >> numBuildings;
    
    vector<Building> buildings(numBuildings);
    
    for (int i = 0; i < numBuildings; i++) {
        input >> buildings[i].left >> buildings[i].height >> buildings[i].right;
        buildings[i].id = i;
    }
    input.close();
    
    // Step 2: Create events for all left and right edges and sort them
    vector<Event> events;
    events.reserve(numBuildings * 2);
    
    for (int i = 0; i < numBuildings; i++) {
        events.push_back(Event(buildings[i].left, i, true, buildings[i].height));
        events.push_back(Event(buildings[i].right, i, false, buildings[i].height));
    }
    
    sort(events.begin(), events.end());
    
    // Step 3: Sweep through events and compute skyline
    MPQ activeBuildingsHeap(numBuildings);
    int currentMaxHeight = 0;
    
    // Output initial ground level
    cout << 0 << " " << 0 << endl;
    
    for (size_t i = 0; i < events.size(); i++) {
        Event& event = events[i];
        
        if (event.isLeft) {
            // Left edge: building starts, add to MPQ
            activeBuildingsHeap.insert(buildings[event.buildingId].height, 
                                      event.buildingId);
        } else {
            // Right edge: building ends, remove from MPQ
            activeBuildingsHeap.Remove(event.buildingId);
        }
        
        // Check if maximum height has changed
        int newMaxHeight = activeBuildingsHeap.GetMax();
        
        // Only output if height changed and this is the last event at this x-coordinate
        // (or the next event is at a different x-coordinate)
        bool isLastEventAtThisX = (i == events.size() - 1) || 
                                  (events[i + 1].x != event.x);
        
        if (isLastEventAtThisX && newMaxHeight != currentMaxHeight) {
            cout << event.x << " " << newMaxHeight << endl;
            currentMaxHeight = newMaxHeight;
        }
    }
    
    return 0;
}