#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <climits>

using namespace std;

// Global Declaration
const int V = 50;  // Number of vertices
vector<int> wareHouses {1, 48, 29, 36};  // Warehouse locations
vector<int> GasStation {25, 17, 27, 38}; // Gas station locations
int property[V] = {0};  // Property type for each node
vector<pair<int, int>> adj[V];  // Adjacency list representation of city
vector<vector<int>> dist(V, vector<int>(V, 1e7));  // Distance matrix for all nodes
int Tankcap = 250;  // Tank capacity

// Custom comparator function for sorting based on weight
bool comp(pair<int, int>& a, pair<int, int>& b) {
    return a.second < b.second;
}

// Floyd-Warshall Algorithm for All-Pairs Shortest Path
void floydwarshall() {
    // Initialization of distances for directly connected nodes
    for (int i = 0; i < V; i++) {
        for (auto adjNode : adj[i]) {
            dist[i][adjNode.first] = adjNode.second;
        }
    }

    // Self loop -> dist=0
    for (int i = 0; i < V; i++) {
        dist[i][i] = 0;
    }

    // Relaxation step
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] < 1e7 && dist[k][j] < 1e7) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
}

// Knapsack algorithm (greedy approach)
vector<pair<int, int>> SystematicPackaging(vector<pair<int, int>> List, int capacity) {
    vector<pair<int, int>> finalListOfItems;
    sort(List.begin(), List.end(), comp);
    int i = 0;
    while (i < List.size() && List[i].second <= capacity) {
        capacity -= List[i].second;
        finalListOfItems.push_back(List[i]);
        i++;
    }
    return finalListOfItems;
}

// Selecting warehouse with most items close to it
int wareHouseSelection(vector<pair<int, int>> List) {
    vector<pair<int, int>> data;

    // Calculating the Distances
    for (auto& item : List) {
        int mini = INT_MAX;
        int selectedWarehouse = -1;
        for (int wareHouse : wareHouses) {
            if (dist[wareHouse][item.first] < mini) {
                mini = dist[wareHouse][item.first];
                selectedWarehouse = wareHouse;
            }
        }
        data.push_back({selectedWarehouse, mini});
    }

    // Counting warehouse occurrences
    vector<int> counters(V, 0);
    for (auto& item : data) {
        counters[item.first]++;
    }

    // Selecting warehouse with max count
    int maxCount = 0, resultWarehouse = -1;
    for (int wareHouse : wareHouses) {
        if (counters[wareHouse] > maxCount) {
            maxCount = counters[wareHouse];
            resultWarehouse = wareHouse;
        }
    }
    return resultWarehouse;
}

// Dijkstra's algorithm for shortest path
vector<int> shortestPath(int srcNode, int lastNode) {
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    vector<int> dist(V, INT_MAX), parent(V);

    for (int i = 0; i < V; i++) {
        parent[i] = i;
    }

    dist[srcNode] = 0;
    pq.push({0, srcNode});

    while (!pq.empty()) {
        int dis = pq.top().first;
        int node = pq.top().second;
        pq.pop();

        if (dis > dist[node]) continue;

        for (auto& adjNode : adj[node]) {
            int newDist = dis + adjNode.second;
            if (newDist < dist[adjNode.first]) {
                dist[adjNode.first] = newDist;
                pq.push({newDist, adjNode.first});
                parent[adjNode.first] = node;
            }
        }
    }

    if (dist[lastNode] == INT_MAX) return {-1};

    vector<int> path;
    for (int node = lastNode; node != srcNode; node = parent[node]) {
        path.push_back(node);
        if (node == parent[node]) return {-1};
    }
    path.push_back(srcNode);
    reverse(path.begin(), path.end());
    return path;
}

// Finding the nearest gas station
int NearGasStation(int srcNode) {
    int minDist = INT_MAX, nearestStation = -1;
    for (int station : GasStation) {
        if (dist[srcNode][station] < minDist) {
            minDist = dist[srcNode][station];
            nearestStation = station;
        }
    }
    return nearestStation;
}

// Counting the number of refills
int NumberOfRefill(vector<pair<int, int>> FinalList) {
    int refillCount = 0, fuelLeft = Tankcap;
    int srcNode = wareHouseSelection(FinalList);

    for (auto& item : FinalList) {
        vector<int> path = shortestPath(srcNode, item.first);
        for (size_t j = 0; j < path.size() - 1; j++) {
            int travelDist = dist[path[j]][path[j + 1]];
            if (fuelLeft < travelDist) {
                fuelLeft = Tankcap;
                refillCount++;
            }
            fuelLeft -= travelDist;
        }
        srcNode = item.first;
    }
    return refillCount;
}

// Path printing with distance and refills
void PathPrinting(vector<pair<int, int>> FinalList) {
    int totalDistance = 0;
    int srcNode = wareHouseSelection(FinalList);
    int cap = Tankcap;

    cout << "Starting from warehouse: " << srcNode << endl;
    cout << "Path: ";

    for (auto& item : FinalList) {
        vector<int> path = shortestPath(srcNode, item.first);
        for (size_t j = 0; j < path.size() - 1; j++) {
            int travelDist = dist[path[j]][path[j + 1]];
            if (cap < travelDist) {
                int pump = NearGasStation(path[j]);
                cout << pump << "(*) -> ";
                totalDistance += dist[path[j]][pump] + dist[pump][path[j + 1]];
                cap = Tankcap;
            } else {
                cout << path[j] << " -> ";
                totalDistance += travelDist;
            }
            cap -= travelDist;
        }
        srcNode = item.first;
    }
    cout << srcNode << endl;
    cout << "Total Distance Covered: " << totalDistance << " units" << endl;
    cout << "Refills Required: " << NumberOfRefill(FinalList) << endl;
}

int main() {
    // Initialize properties
    // 2 = Warehouse, 1 = Gas Station, 0 = Regular node
    property[1] = property[48] = property[29] = property[36] = 2;  // Warehouses
    property[25] = property[17] = property[27] = property[38] = 1; // Gas Stations

    // Initialize adjacency list for a 50-node graph
    // Each pair represents {destination_node, distance}
    //adj[i]={{x,y},{a,b}} --->>> i is connected to x with distance y and a with distance b
    adj[0] = {{1, 10}, {2, 15}, {3, 20}};
    adj[1] = {{0, 10}, {2, 35}, {4, 25}};
    adj[2] = {{0, 15}, {1, 35}, {3, 30}, {5, 20}};
    adj[3] = {{0, 20}, {2, 30}, {6, 25}};
    adj[4] = {{1, 25}, {5, 15}, {7, 30}};
    adj[5] = {{2, 20}, {4, 15}, {6, 35}, {8, 25}};
    adj[6] = {{3, 25}, {5, 35}, {9, 20}};
    adj[7] = {{4, 30}, {8, 15}, {10, 25}};
    adj[8] = {{5, 25}, {7, 15}, {9, 30}, {11, 20}};
    adj[9] = {{6, 20}, {8, 30}, {12, 25}};
    adj[10] = {{7, 25}, {11, 35}, {13, 20}};
    adj[11] = {{8, 20}, {10, 35}, {12, 30}, {14, 25}};
    adj[12] = {{9, 25}, {11, 30}, {15, 20}};
    adj[13] = {{10, 20}, {14, 15}, {16, 30}};
    adj[14] = {{11, 25}, {13, 15}, {15, 35}, {17, 25}};
    adj[15] = {{12, 20}, {14, 35}, {18, 20}};
    adj[16] = {{13, 30}, {17, 15}, {19, 25}};
    adj[17] = {{14, 25}, {16, 15}, {18, 30}, {20, 20}};
    adj[18] = {{15, 20}, {17, 30}, {21, 25}};
    adj[19] = {{16, 25}, {20, 35}, {22, 20}};
    adj[20] = {{17, 20}, {19, 35}, {21, 30}, {23, 25}};
    adj[21] = {{18, 25}, {20, 30}, {24, 20}};
    adj[22] = {{19, 20}, {23, 15}, {25, 30}};
    adj[23] = {{20, 25}, {22, 15}, {24, 35}, {26, 25}};
    adj[24] = {{21, 20}, {23, 35}, {27, 20}};
    adj[25] = {{22, 30}, {26, 15}, {28, 25}};
    adj[26] = {{23, 25}, {25, 15}, {27, 30}, {29, 20}};
    adj[27] = {{24, 20}, {26, 30}, {30, 25}};
    adj[28] = {{25, 25}, {29, 35}, {31, 20}};
    adj[29] = {{26, 20}, {28, 35}, {30, 30}, {32, 25}};
    adj[30] = {{27, 25}, {29, 30}, {33, 20}};
    adj[31] = {{28, 20}, {32, 15}, {34, 30}};
    adj[32] = {{29, 25}, {31, 15}, {33, 35}, {35, 25}};
    adj[33] = {{30, 20}, {32, 35}, {36, 20}};
    adj[34] = {{31, 30}, {35, 15}, {37, 25}};
    adj[35] = {{32, 25}, {34, 15}, {36, 30}, {38, 20}};
    adj[36] = {{33, 20}, {35, 30}, {39, 25}};
    adj[37] = {{34, 25}, {38, 35}, {40, 20}};
    adj[38] = {{35, 20}, {37, 35}, {39, 30}, {41, 25}};
    adj[39] = {{36, 25}, {38, 30}, {42, 20}};
    adj[40] = {{37, 20}, {41, 15}, {43, 30}};
    adj[41] = {{38, 25}, {40, 15}, {42, 35}, {44, 25}};
    adj[42] = {{39, 20}, {41, 35}, {45, 20}};
    adj[43] = {{40, 30}, {44, 15}, {46, 25}};
    adj[44] = {{41, 25}, {43, 15}, {45, 30}, {47, 20}};
    adj[45] = {{42, 20}, {44, 30}, {48, 25}};
    adj[46] = {{43, 25}, {47, 35}, {49, 20}};
    adj[47] = {{44, 20}, {46, 35}, {48, 30}};
    adj[48] = {{45, 25}, {47, 30}};
    adj[49] = {{46, 20}};

    // Make the graph undirected by adding reverse edges
    vector<pair<int, int>> tempAdj[V];
    for (int i = 0; i < V; i++) {
        for (auto& edge : adj[i]) {
            tempAdj[edge.first].push_back({i, edge.second});
        }
    }

    // Combine the forward and reverse edges
    for (int i = 0; i < V; i++) {
        for (auto& edge : tempAdj[i]) {
            adj[i].push_back(edge);
        }
    }

    // Calculate all-pairs shortest paths
    floydwarshall();

    // Example delivery list: {destination, package_weight}
    int n;cout << "Enter number of Packages to deliver: "; cin >> n;
    vector<pair<int, int>> itemsToDeliver;
    for(int i=0;i<n;i++) {
        int a,b;cin>>a>>b;
        itemsToDeliver.push_back({a,b});
    }

    cout << "Delivery Route Planning:" << endl;
    cout << "------------------------" << endl;
    int capacity;
    cout << "Enter capacity of Van: ";
    cin >> capacity;
    vector<pair<int,int>>finalList=SystematicPackaging(itemsToDeliver,capacity);
    cout << "Final List of Items:" << endl;
    for(auto it : finalList) {
        cout << it.first << " " << it.second << endl;
    }
    PathPrinting(finalList);
    return 0;
}
