#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue> //for BFS
#include <cmath> //square root for euclid distance
#include <chrono> // library to track clocks, time points, and durations.

using namespace std;
using namespace ::chrono;

//city structure to place data.
struct City {
    int Cityid; //Cities unique ID
    double x; //Coordinates
    double y;
    vector<int> neighbors; //
    double distanceFromBeginning; //tracking from start
    int previousCity; //Tracking from previous city
};
//Euclid distance
double calculateDistance(const City& city1, const City& city2) {
    return sqrt((city1.x - city2.x) * (city1.x - city2.x) + (city1.y - city2.y) * (city1.y - city2.y));
}

int main() {
    string filename = "11PointDFSBFS.tsp"; // Replace with your TSP file's name
    ifstream tspFile(filename);
    // To read the file on xcode I had to create a directory path to where file is located.
    // if not open then gives me error warning.
    if (!tspFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }
    
    string line; //to read each line from input file
    vector<City> cities; //going to store the city structures
    bool readingNodes = false;
    
    while (getline(tspFile, line)) {
        if (line.find("NODE_COORD_SECTION") != string::npos) {
            //while loop to read until end of file is reached. If tsp located teh sring Node_coord_section than its sets to know that nodes are followed and sets reading nodes from false to true.
            readingNodes = true;
            continue;
        }
        //If readingNodes and true and find EOF. Print data.
        if (readingNodes && line.find("EOF") == string::npos) {
            int id;
            double x, y;
            //converts line to string then integer and double precision float in placed in ID,X, and Y in which its equal to 3 values.
            if (sscanf(line.c_str(), "%d %lf %lf", &id, &x, &y) == 3) {
                cities.push_back({id, x, y});
                //pushed ID,X,Y into vector.
            }
        }
    }
    
    tspFile.close();
    //created matrix for city connections. 1 to 2,3,4. 2 to 3, etc.
    const vector<vector<int>> connections = {
        {2, 3, 4},
        {3},
        {4, 5},
        {5, 6, 7},
        {7,8},
        {8},
        {9, 10},
        {9, 10, 11},
        {11},
        {11}
    };
    
    // Print the parsed data
    //testing my code to see if it works on output of file.
    
    cout << "Cities:" << endl;
    for (size_t i = 0; i < cities.size(); i++) {
        const City& city = cities[i];
        cout << "City Number: " << city.Cityid << " X: " << city.x << " Y: " << city.y << " coordinates" << endl;
    }
    
    //BFS Algorithm
    queue<City*> BFS;
    //store points to city objects
    //queue data structure.
    
    // Initialize distances before starting BFS
    for (size_t i = 0; i < cities.size(); i++) {
        cities[i].distanceFromBeginning = numeric_limits<double>::max();
        //sets limits to infinity.
        cities[i].previousCity = -1;
        //keeps track of the path
    }
    
    // Set the starting city's distance to 0 as a tracker.
    cities[0].distanceFromBeginning = 0;
    BFS.push(&cities[0]);
    
    
    int goalCity = 11;
    //goal city to end at
    auto startTime = high_resolution_clock::now();
    //timer and step counter to find how fast and how many steps it takes for program to be completed.
    int transitions = 0; // Count transitions (steps)
    
    while (!BFS.empty()){
        //while queue is not empty push city at front of queue to be processed
        //then removes the item at front of queue.
        City* currentCity = BFS.front();
        BFS.pop();
        //BFS traversal until empty
        transitions++;
        
        // Check if the goal city is reached
        if (currentCity->Cityid == goalCity) {
            //if current city id is equal to goal city id then goal has beenreached
            auto endTime = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(endTime - startTime);
            //calculates duration
            
            cout << "Shortest distance from city 1 to city 11 is: " << currentCity->distanceFromBeginning << endl;
            //current city is now the distance from the beginning calculation
            
            vector<int> path;
            //empty vector to store path
            int currentID = goalCity;
            //Start from City 11
            while (currentID != -1){
                //while not equal to -1.path is inserted current id which is 11.
                path.insert(path.begin(),currentID);
                currentID = cities[currentID - 1]. previousCity;
                //keeps going backwards until reaches -1
            }
            cout << "Shortest path from City 1 to City 11: ";
            for (int i = 0; i < path.size(); i++) {
                cout << path[i];
                if (i < path.size() - 1) {
                    cout << " to ";
                }
            }
            cout << endl;
            //reverse to find the path
            cout << "BFS time taken: " << duration.count() << " microseconds" << endl;
            cout << "BFS transitions (steps): " << transitions << endl;
    
            
            break;
        }
        // Explore neighbors of the current city using Euclidean distance
        for (int neighborIndex = 0; neighborIndex < connections[currentCity->Cityid - 1].size(); neighborIndex++){
            int neighborID = connections[currentCity->Cityid - 1][neighborIndex];
            City& neighbor = cities[neighborID - 1];
            double distance = calculateDistance(*currentCity, neighbor);
            
            if (neighbor.distanceFromBeginning == -1 || neighbor.distanceFromBeginning > currentCity->distanceFromBeginning + distance) {
                neighbor.distanceFromBeginning = currentCity->distanceFromBeginning + distance;
                neighbor.previousCity = currentCity->Cityid;
                BFS.push(&neighbor);
                
            }
        }
    }
    
    return 0;
}
