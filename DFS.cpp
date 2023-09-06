#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <chrono> //libary to track clocks, time points and durations.
#include <stack> //for BFS

using namespace std;
using namespace chrono;

struct City {
    int Cityid;//City Number
    double x; //City coordinates
    double y; //City Coordinates
    double distanceFromBeginning; //
    int previousCity;
};

double calculateDistance(const City& city1, const City& city2) {
    return sqrt((city1.x - city2.x) * (city1.x - city2.x) + (city1.y - city2.y) * (city1.y - city2.y));
}

int main() {
    string filename = "11PointDFSBFS.tsp"; // Replace with your TSP file's name
    ifstream tspFile(filename);

    if (!tspFile.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    string line;
    vector<City> cities; // going to store the city structures
    bool readingNodes = false;

    while (getline(tspFile, line)) {
        if (line.find("NODE_COORD_SECTION") != string::npos) {
            //   //while loop to read until end of file is reached. If tsp located teh sring Node_coord_section than its sets to know that nodes are followed and sets reading nodes from false to true.
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

    const int goalCityID2 = 11;
    //goal city
    const int startCityID = 1;
//matrix for city connections
    const vector<vector<int>> connections = {
        {2, 3, 4},
        {3},
        {4, 5},
        {5, 6, 7},
        {7, 8},
        {8},
        {9, 10},
        {9, 10, 11},
        {11},
        {11}
    };
    
    // Print the parsed data
    //testing my code to see if it works on output of file.
    
    //DFS Algorithm
    stack<City*> DFS;
    //DFS Stack

    for (size_t i = 0; i < cities.size(); i++) {
        cities[i].distanceFromBeginning = numeric_limits<double>::max();
        cities[i].previousCity = -1;
    }

    DFS.push(&cities[startCityID - 1]); // Start from the start city
    cities[startCityID - 1].distanceFromBeginning = 0; // Set distance to 0
    cout << "Entering DFS loop" << endl;

    auto startTime2 = high_resolution_clock::now();
    int transitions2 = 0;
    
    // step counter and start time calculation.

    while (!DFS.empty()) {
        City* currentCity = DFS.top();
        DFS.pop();
        transitions2++;
        //increment steps 

        if (currentCity->Cityid == goalCityID2) {
            auto endTime = high_resolution_clock::now();
            auto duration = duration_cast<microseconds>(endTime - startTime2);

            cout << "DFS shortest distance from city " << startCityID << " to city " << goalCityID2 << " is: " << currentCity->distanceFromBeginning << endl;

            vector<int> path;
            int currentID = goalCityID2;

            while (currentID != -1) {
                path.insert(path.begin(), currentID);
                currentID = cities[currentID - 1].previousCity;
            }

            cout << "DFS Short path: ";
            for (int i = 0; i < path.size(); i++) {
                cout << path[i];
                if (i < path.size() - 1) {
                    cout << " to ";
                }
            }
            cout << endl;

            cout << "DFS Time taken: " << duration.count() << " microseconds" << endl;
            cout << "DFS Transitions (steps): " << transitions2 << endl;

            break;
        }

        for (int neighborIndex = 0; neighborIndex < connections[currentCity->Cityid - 1].size(); neighborIndex++) {
            int neighborID = connections[currentCity->Cityid - 1][neighborIndex];
            City& neighbor = cities[neighborID - 1];
            double distance = calculateDistance(*currentCity, neighbor);

            if (neighbor.distanceFromBeginning == numeric_limits<double>::max() || neighbor.distanceFromBeginning > currentCity->distanceFromBeginning + distance) {
                neighbor.distanceFromBeginning = currentCity->distanceFromBeginning + distance;
                neighbor.previousCity = currentCity->Cityid;
                DFS.push(&neighbor);
            }
        }
    }

    return 0;
}
