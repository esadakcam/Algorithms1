/* Mehmed Esad AKÇAM
 *  150190725
 * Compiled with the command: g++ main.cpp
 */
#include <fstream>
#include <time.h>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <climits>
using namespace std;

//-----------------------------------------------------------------------------------------------------------------------------------------
// Vehicle class
class Vehicle
{
public:
    Vehicle(){};
    int vehicle_id;
    string location;
    double distance;

    int speed;
    double time;
    bool operator<(const Vehicle &in) { return this->time < in.time; } // operator overload to simplify heap operations.
    Vehicle(int id, string loc, double dist, int spd, double tme) : vehicle_id(id), location(loc), distance(dist), speed(spd), time(tme) {}
};
//-----------------------------------------------------------------------------------------------------------------------------------------

// Request class
class Request
{
public:
    string location;
    double destination;
    int lucky_number;
    Request(string loc, double dest, int luck) : location(loc), destination(dest), lucky_number(luck) {}
};
//-----------------------------------------------------------------------------------------------------------------------------------------

// Minimumum Priorty Queue with MinHeap Data Structure
class MinHeap
{
    vector<Vehicle> vehicles; // Heap Array
    vector<Request> requests; // This class holds requests to handle
    int max_size = 1642;

public:
    int N;
    vector<Vehicle> history;
    MinHeap(vector<Vehicle> &inVehicles, vector<Request> &inRequests, int n, int size);
    int getLeft(int index);
    int getRight(int index);
    int getParent(int index);
    void minHeapify(int index);
    void extract();
    void insert(Vehicle toadd);
    void swap(int element1, int element2);
    void decrease(int index, Vehicle newVehicle);
};
//-----------------------------------------------------------------------------------------------------------------------------------------
MinHeap::MinHeap(vector<Vehicle> &inVehicles, vector<Request> &inRequests, int n, int size)
{

    for (int i = 0; i < inRequests.size(); i++)
    {
        requests.push_back(inRequests[i]);
    }
    for (int i = 0; i < inVehicles.size(); i++)
    {
        vehicles.push_back(inVehicles[i]);
    }
    // build minimum heap
    for (int i = (vehicles.size() / 2) - 1; i >= 0; i--)
    {
        minHeapify(i);
    }
    this->N = n;
    this->max_size = size;
}
// swap elements in the given indexes
void MinHeap::swap(int element1, int element2)
{
    Vehicle temp = vehicles[element1];
    vehicles[element1] = vehicles[element2];
    vehicles[element2] = temp;
}

int MinHeap::getLeft(int index)
{
    return (2 * index + 1);
}

int MinHeap::getRight(int index)
{
    return (2 * index + 2);
}

int MinHeap::getParent(int index)
{
    return (index - 1) / 2;
}

void MinHeap::minHeapify(int index)
{
    int right = getRight(index);
    int left = getLeft(index);
    int smallest = index;
    // swap parent and smallest child (if child is smaller than the parent)
    if (left < vehicles.size() && vehicles[left] < vehicles[index])
    {
        smallest = left;
    }
    if (right < vehicles.size() && vehicles[right] < vehicles[smallest])
    {
        smallest = right;
    }
    if (smallest != index)
    {
        swap(index, smallest);
        minHeapify(smallest);
    }
}

void MinHeap::insert(Vehicle in)
{
    //    if the heap is full
    if (vehicles.size() + 1 > max_size)
    {
        exit(EXIT_FAILURE); // heap overflow
    }
    vehicles.push_back(in);
    int index = vehicles.size() - 1;

    // satisfy the min heap condition recursively by swaping element in the given index with its parent if the element is smaller than the root
    while (index != 0 && vehicles[index] < vehicles[getParent(index)])
    {
        swap(index, getParent(index));
        index = getParent(index);
    }
    N--; // update n
}
void MinHeap::decrease(int index, Vehicle newVehicle)
{
    vehicles[index] = newVehicle;
    // satisfy the min heap condition
    while (index != 0 && vehicles[index] < vehicles[getParent(index)])
    {
        swap(index, getParent(index));
        index = getParent(index);
    }
    N--; // update n
}

void MinHeap::extract()
{
    // if heap is empty return exit_failure
    if (vehicles.size() == 0)
    {
        exit(EXIT_FAILURE);
    }

    Vehicle retVehicle; // the requested vehicle

    // if there is only one vehicle in the heap call the root
    if (vehicles.size() == 1)
    {
        retVehicle = vehicles[0];
    }
    else
    {
        // if the request has not lucky number call the fastest arriving car which is the root car
        if (requests.front().lucky_number == 0)
        {
            retVehicle = vehicles[0];
            vehicles[0] = vehicles[vehicles.size() - 1];
            vehicles.erase(vehicles.end());
            minHeapify(0);
        }
        // if the request has a lucky number call the car which is in the lucky_number index
        else
        {
            int luckyNum = requests.front().lucky_number;
            retVehicle = vehicles[luckyNum];

            // decrease the car to the root;
            retVehicle.time = INT_MIN;
            decrease(luckyNum, retVehicle);
            vehicles[0] = vehicles[vehicles.size() - 1]; // make the latest element root since root currently containing returned vehicle.

            vehicles.erase(vehicles.end()); // remove the latest car from the heap array since it is currently in the root
            minHeapify(0);                  // satisfy min heap condition
        }
    }
    history.push_back(retVehicle); // append the history
    // update the called vehicle
    retVehicle.time = (requests[0].destination) / retVehicle.speed;
    retVehicle.location = requests[0].location;
    // insert the car back to  heap
    insert(retVehicle);
    requests.erase(requests.begin()); // remove the earliest request
    N--;                              // update n
}
int main(int argc, char const *argv[])
{

    vector<Request> requests; // all requests
    vector<Vehicle> vehicles; // all vehicles
    string line;              // dummy string to read files
    ifstream requestsFile("requests.txt");
    ifstream vehiclesFile("vehicles.txt");
    ofstream historyFile("call_history_" + (string)argv[1] + ".txt"); // output file

    if (requestsFile.is_open())
    {
        getline(requestsFile, line); // read first line
        string location;
        double distance;
        int lucky_number;
        while (requestsFile >> location >> distance >> lucky_number)
        {

            requests.push_back(Request(location, distance, lucky_number)); // append the Requests vector with requests from the file
        }

        requestsFile.close(); // close the requests file
    }

    if (vehiclesFile.is_open())
    {
        getline(vehiclesFile, line); // read first line
        string location;
        double distance;
        int id;
        int speed;
        while (vehiclesFile >> id >> location >> distance >> speed)
        {
            double time = (float)distance / speed;
            vehicles.push_back(Vehicle(id, location, distance, speed, time)); // calculate the estimated time and append the Vehicles vector.
        }
        vehiclesFile.close(); // close the file
    }
    MinHeap heap = MinHeap(vehicles, requests, stoi(argv[1]), vehicles.size()); // create the min poriorty queue

    clock_t start = clock();
    while (heap.N > 0)
    {
        heap.extract(); // send vehicles to requests until N gets 0.
    }

    start = clock() - start;
    float time = (float)start / CLOCKS_PER_SEC;
    time *= 1000; // convert elapsed time into miliseconds
    printf("N = %s took %.0f ms.\n", argv[1], time);
    if (historyFile.is_open())
    {
        for (int i = 0; i < heap.history.size(); i++)
        {
            historyFile << heap.history[i].vehicle_id << "\n"; // write the history into output file
        }

        historyFile.close();
    }
    return 0;
}
