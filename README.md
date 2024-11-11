### Delivery Route Optimizer

This program implements a **Delivery Route Optimizer** that calculates optimal delivery routes using a combination of **Greedy Knapsack** (for package selection based on van capacity), **Floyd-Warshall** (for finding shortest paths between all pairs of nodes), and **Dijkstra's Algorithm** (for single-source shortest path from one location to another). It also provides efficient path planning with **refuel station selection** for routes that may require fuel refills.

### Features
1. **All-Pairs Shortest Path Calculation**: Uses the Floyd-Warshall algorithm to calculate the shortest paths between all pairs of nodes.
2. **Package Selection with Knapsack**: Employs a Greedy Knapsack approach to select the maximum number of packages based on the van's weight capacity.
3. **Single-Source Shortest Path Calculation**: Dijkstra’s algorithm is applied to find the shortest route from a warehouse to each delivery location.
4. **Refueling Station Management**: Determines the optimal gas stations for refueling along each delivery path based on the van’s tank capacity.

---

### Components and Code Structure

1. **Global Variables and Graph Representation**
   - `V`: Total number of nodes (50).
   - `wareHouses`: Vector listing warehouse locations.
   - `GasStation`: Vector listing gas station locations.
   - `property`: Array indicating node types (`0` = regular node, `1` = gas station, `2` = warehouse).
   - `adj`: Adjacency list for the graph, where each entry is a pair `{destination_node, distance}`.
   - `dist`: Distance matrix initialized to represent infinite distance between non-adjacent nodes (1e7) and to store shortest path results.
   - `Tankcap`: Maximum capacity of the delivery vehicle’s fuel tank.

2. **Floyd-Warshall Algorithm** (`floydwarshall` function)
   - The `floydwarshall` function initializes distances for all directly connected nodes.
   - Calculates the shortest paths for all pairs of nodes, updating `dist[i][j]` to the minimum path cost found for each node pair using dynamic programming.

3. **Greedy Knapsack Algorithm** (`SystematicPackaging` function)
   - Sorts the items by weight in ascending order and selects the maximum possible items that can fit within the van’s capacity.
   - Returns a final list of items to deliver based on the provided capacity.

4. **Warehouse Selection** (`wareHouseSelection` function)
   - Chooses the warehouse closest to the majority of the delivery locations.
   - Helps optimize starting points for delivery paths, thereby minimizing overall travel distance.

5. **Dijkstra’s Algorithm** (`shortestPath` function)
   - Uses Dijkstra’s algorithm to find the shortest path between a source node and a target node.
   - Implements a priority queue to efficiently determine shortest paths and backtrack to retrieve the complete route.

6. **Refueling Management** (`NearGasStation` and `NumberOfRefill` functions)
   - `NearGasStation`: Finds the closest gas station to the current node.
   - `NumberOfRefill`: Calculates the number of refills required for each trip based on the delivery route and vehicle tank capacity.

7. **Path Printing with Distance and Refills** (`PathPrinting` function)
   - Prints the complete route with nodes traversed from the selected warehouse to each delivery location.
   - Identifies gas stations along the way if refueling is required.
   - Provides total distance covered and the number of refills required.

8. **Main Function**
   - Initializes node properties (warehouse, gas stations, etc.).
   - Builds the graph's adjacency list to represent connections between nodes.
   - Adds reverse edges to make the graph undirected.
   - Calculates shortest paths using Floyd-Warshall.
   - Accepts user input for the number of packages, their destinations, and weights.
   - Accepts input for van capacity, selects packages within capacity, and optimizes the delivery route.

---

### Example Usage
1. **Input Format**:
   - The user enters the number of packages and provides each package's destination and weight.
   - The user inputs the van’s maximum capacity.

2. **Output**:
   - The program displays:
     - **Selected Packages**: List of packages chosen based on the van's capacity.
     - **Delivery Route**: The complete route, showing the starting warehouse, nodes visited, any required refueling stations, and the final destination.
     - **Total Distance**: The distance covered along the optimized route.
     - **Refills Required**: The number of refills necessary based on the distance and tank capacity.

3. **Sample Interaction**:
   ```plaintext
   Enter number of Packages to deliver: 3
   10 20
   15 30
   8 25
   Enter capacity of Van: 100
   ```

   **Output**:
   ```plaintext
   Delivery Route Planning:
   ------------------------
   Final List of Items:
   10 20
   15 30
   8 25
   Starting from warehouse: 1
   Path: 1 -> 4 -> 7 -> 10 -> 7 -> 8 -> 11 -> 12 -> 15
   Total Distance Covered: 190 units
   Refills Required: 0
   ```

---

### Algorithmic Complexity
- **Floyd-Warshall**: \(O(V^3)\) - Calculating shortest paths for all node pairs.
- **Knapsack (Greedy)**: \(O(N \log N)\) - Sorting and selecting packages based on weight.
- **Dijkstra's Algorithm**: \(O((V + E) \log V)\) - Calculating single-source shortest paths.
- **Refuel Calculation**: \(O(V \cdot K)\) where \(K\) is the number of stations to check.

---

### Dependencies
This program uses C++ Standard Template Library (STL) components such as:
- `<vector>` for graph and package representation.
- `<algorithm>` for sorting operations in Greedy Knapsack.
- `<queue>` for Dijkstra’s priority queue.
- `<climits>` for setting infinite distance values.

---

### Installation & Compilation
To compile and run the program:
1. Make sure you have a C++ compiler, e.g., `g++`.
2. Compile using:
   ```bash
   g++ -o delivery_optimizer delivery_optimizer.cpp
   ```
3. Run the program:
   ```bash
   ./delivery_optimizer
   ```

---

### Possible Enhancements
- Implement a more sophisticated knapsack algorithm (dynamic programming) for exact weight distribution.
- Expand graph node properties to handle multiple vehicle types and capacities.
- Add more detailed fuel management, considering fuel consumption per node based on real-world conditions. 

This program provides a robust and flexible approach to delivery optimization, balancing route efficiency, fuel management, and package selection.