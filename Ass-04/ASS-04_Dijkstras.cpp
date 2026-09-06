#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <algorithm>

using namespace std;

const int INF = 1e9; // Represents infinity for unreachable nodes

// Structure to store dynamic edge details
struct Edge {
    int to;
    int weight;
};

// Function to run Dijkstra's Algorithm from the ambulance source position
void dijkstra(int src, int V, const vector<vector<pair<int, int>>>& adj, vector<int>& dist, vector<int>& parent) {
    dist.assign(V, INF);
    parent.assign(V, -1);
    dist[src] = 0;

    // Min-heap priority queue storing pairs of {distance, node}
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        pair<int, int> current = pq.top();
        pq.pop();

        int d = current.first;
        int u = current.second;

        // Skip outdated distance pairs
        if (d > dist[u]) continue;

        for (const auto& neighbor : adj[u]) {
            int v = neighbor.first;
            int weight = neighbor.second;

            // Relaxation step
            if (dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
                parent[v] = u;
                pq.push({dist[v], v});
            }
        }
    }
}

// Function to print visual navigation route from source to destination
void printRoute(int dest, const vector<int>& parent) {
    vector<int> path;
    for (int curr = dest; curr != -1; curr = parent[curr]) {
        path.push_back(curr);
    }
    reverse(path.begin(), path.end());

    cout << "   [Route Path]: ";
    for (size_t i = 0; i < path.size(); i++) {
        cout << "Intersection " << path[i];
        if (i + 1 < path.size()) cout << "  ===>  ";
    }
    cout << "\n";
}

// Function to update dynamic road travel time (traffic update)
void updateRoadTraffic(vector<vector<pair<int, int>>>& adj, int u, int v, int newWeight) {
    bool updated = false;
    for (auto& edge : adj[u]) {
        if (edge.first == v) {
            edge.second = newWeight;
            updated = true;
            break;
        }
    }
    for (auto& edge : adj[v]) {
        if (edge.first == u) {
            edge.second = newWeight;
            break;
        }
    }

    if (updated) {
        cout << "\n[TRAFFIC UPDATE SUCCESSFUL]: Travel time between Intersection " 
             << u << " and " << v << " updated to " << newWeight << " mins.\n";
    } else {
        cout << "\n[ERROR]: No direct road exists between Intersection " << u << " and " << v << ".\n";
    }
}

// Displays all hospitals and highlights the quickest option
void evaluateHospitals(int src, const vector<int>& hospitals, const vector<int>& dist, const vector<int>& parent) {
    cout << "\n=================================================================\n";
    cout << "             EMERGENCY AMBULANCE ROUTING ANALYSIS                \n";
    cout << "=================================================================\n";

    int nearestHospital = -1;
    int minTime = INF;

    for (int h : hospitals) {
        cout << "\nHospital Node " << h << ":\n";
        if (dist[h] == INF) {
            cout << "   Status: Unreachable due to route blockages.\n";
        } else {
            cout << "   Total Travel Time: " << dist[h] << " minutes\n";
            printRoute(h, parent);

            if (dist[h] < minTime) {
                minTime = dist[h];
                nearestHospital = h;
            }
        }
    }

    cout << "\n-----------------------------------------------------------------\n";
    if (nearestHospital != -1) {
        cout << ">>> RECOMMENDED DESTINATION: Hospital " << nearestHospital 
             << " (Quickest Time: " << minTime << " mins) <<<\n";
    } else {
        cout << ">>> WARNING: No accessible hospital available! <<<\n";
    }
    cout << "-----------------------------------------------------------------\n";
}

int main() {
    int V, E;
    cout << "Enter total number of intersections (nodes): ";
    cin >> V;
    cout << "Enter total number of roads (edges): ";
    cin >> E;

    vector<vector<pair<int, int>>> adj(V);

    cout << "\nEnter road details (Format: Source_Intersection Dest_Intersection Travel_Time_Mins):\n";
    for (int i = 0; i < E; i++) {
        int u, v, w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
        adj[v].push_back({u, w}); // Two-way road
    }

    int source;
    cout << "\nEnter Ambulance current location (Source node): ";
    cin >> source;

    int numHospitals;
    cout << "Enter total number of hospitals: ";
    cin >> numHospitals;

    vector<int> hospitals(numHospitals);
    cout << "Enter hospital intersection IDs:\n";
    for (int i = 0; i < numHospitals; i++) {
        cin >> hospitals[i];
    }

    vector<int> dist, parent;
    
    // Initial Route Computation
    dijkstra(source, V, adj, dist, parent);
    evaluateHospitals(source, hospitals, dist, parent);

    // Interactive Loop for Dynamic Traffic Updates
    int choice;
    do {
        cout << "\n--- DYNAMIC TRAFFIC CONTROL MENU ---\n";
        cout << "1. Update dynamic traffic on a road\n";
        cout << "2. Recalculate optimal routes\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int u, v, newWeight;
            cout << "Enter the two connected intersections and new travel time (u v new_time): ";
            cin >> u >> v >> newWeight;
            updateRoadTraffic(adj, u, v, newWeight);
        } else if (choice == 2) {
            dijkstra(source, V, adj, dist, parent);
            evaluateHospitals(source, hospitals, dist, parent);
        }
    } while (choice != 3);

    cout << "\nTraffic system session closed.\n";
    return 0;
}