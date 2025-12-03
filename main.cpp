#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "MPQ.h"

using namespace std;

// building
struct Building {
    int left;
    int height;
    int right;
    int id;
    
    Building(int l = 0, int h = 0, int r = 0, int i = 0) 
        : left(l), height(h), right(r), id(i) {}
};

// struct to represent x-coord event
struct Event {
    int x;          // x-coord
    int buildingId; // building it belongs to
    bool isLeft;    // true if left edge, false if right edge
    int height;     // height of building
    
    Event(int xCoord = 0, int id = 0, bool left = true, int h = 0)
        : x(xCoord), buildingId(id), isLeft(left), height(h) {}
    
    // comparison operator for sorting
    bool operator<(const Event& other) const {
        if (x != other.x) {
            return x < other.x;
        }
        // if at same x-coordinate, process left edges before right edges
        if (isLeft != other.isLeft) {
            return isLeft > other.isLeft;
        }
        // if both are left edges, process taller buildings first
        if (isLeft) {
            return height > other.height;
        }
        // if both are right edges, process shorter buildings first
        return height < other.height;
    }
};

int main() {
    // read input from file
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
    
    // create events for all left and right edges, sort them
    vector<Event> events;
    events.reserve(numBuildings * 2);
    
    for (int i = 0; i < numBuildings; i++) {
        events.push_back(Event(buildings[i].left, i, true, buildings[i].height));
        events.push_back(Event(buildings[i].right, i, false, buildings[i].height));
    }
    
    sort(events.begin(), events.end());
    
    // sweep through events and compute skyline
    MPQ activeBuildingsHeap(numBuildings);
    int currentMaxHeight = 0;
    
    // output initial ground level
    cout << 0 << " " << 0 << endl;
    
    for (size_t i = 0; i < events.size(); i++) {
        Event& event = events[i];
        
        if (event.isLeft) {
            // left edge -> building starts, add to MPQ
            activeBuildingsHeap.insert(buildings[event.buildingId].height, 
                                      event.buildingId);
        } else {
            // right edge -> building ends, remove from MPQ
            activeBuildingsHeap.Remove(event.buildingId);
        }
        
        // check if max height has changed
        int newMaxHeight = activeBuildingsHeap.GetMax();
        
        // only output if height changed and this is the last event at this x-coord
        // alternatively, if next event is at different x-coord
        bool isLastEventAtThisX = (i == events.size() - 1) || 
                                  (events[i + 1].x != event.x);
        
        if (isLastEventAtThisX && newMaxHeight != currentMaxHeight) {
            cout << event.x << " " << newMaxHeight << endl;
            currentMaxHeight = newMaxHeight;
        }
    }
    
    return 0;
}