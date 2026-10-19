#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <iomanip>

using namespace std;

const long long INF = 1e18; // Infinity representation for unreachable hubs

// Structure to store edge details
struct Edge {
    int to;
    long long weight; // Travel cost/time considering traffic, weather, fuel
};

// Dynamic Programming Solver for Multistage Graph (Backward Approach)
void solveMultistageDP(int numStages, const vector<vector<int>>& stageNodes, 
                       const vector<vector<Edge>>& adj, vector<long long>& cost, vector<int>& nextNode) {
    int totalNodes = 0;
    for (const auto& stage : stageNodes) totalNodes += stage.size();

    cost.assign(totalNodes, INF);
    nextNode.assign(totalNodes, -1);

    // Destination hub is in the final stage (Stage N-1)
    int destNode = stageNodes[numStages - 1][0];
    cost[destNode] = 0;

    // Process stages backwards from Stage (N-2) down to Stage 0
    for (int s = numStages - 2; s >= 0; s--) {
        for (int u : stageNodes[s]) {
            for (const auto& edge : adj[u]) {
                int v = edge.to;
                if (cost[v] != INF && edge.weight + cost[v] < cost[u]) {
                    cost[u] = edge.weight + cost[v];
                    nextNode[u] = v;
                }
            }
        }
    }
}

// Visual Representation of Optimal Delivery Route
void printPath(int src, const vector<int>& nextNode) {
    cout << "   [Optimal Delivery Path]: ";
    int curr = src;
    while (curr != -1) {
        cout << "Hub " << curr;
        curr = nextNode[curr];
        if (curr != -1) cout << "  ===>  ";
    }
    cout << "\n";
}

// Function to handle real-time traffic / weather / road closure updates
void updateEdgeWeight(vector<vector<Edge>>& adj, int u, int v, long long newWeight) {
    bool found = false;
    for (auto& edge : adj[u]) {
        if (edge.to == v) {
            edge.weight = newWeight;
            found = true;
            break;
        }
    }
    if (found) {
        cout << "\n[REAL-TIME UPDATE SUCCESSFUL]: Route cost from Hub " << u << " to Hub " << v 
             << " updated to " << newWeight << " units.\n";
    } else {
        cout << "\n[ERROR]: No direct route exists between Hub " << u << " and Hub " << v << ".\n";
    }
}

int main() {
    int numStages;
    cout << "Enter total number of stages (N >= 2): ";
    cin >> numStages;

    vector<vector<int>> stageNodes(numStages);
    int nodeCounter = 0;

    cout << "\n--- Hub Allocation Per Stage ---\n";
    for (int i = 0; i < numStages; i++) {
        int count;
        cout << "Enter number of hubs in Stage " << i + 1 << ": ";
        cin >> count;
        for (int j = 0; j < count; j++) {
            stageNodes[i].push_back(nodeCounter++);
        }
    }

    int totalNodes = nodeCounter;
    vector<vector<Edge>> adj(totalNodes);

    cout << "\nGenerated Hub IDs by Stage:\n";
    for (int i = 0; i < numStages; i++) {
        cout << "  Stage " << i + 1 << ": Hub IDs [ ";
        for (int hub : stageNodes[i]) cout << hub << " ";
        cout << "]\n";
    }

    int numEdges;
    cout << "\nEnter total number of connecting routes/edges: ";
    cin >> numEdges;

    cout << "Enter route details (Format: Source_Hub Dest_Hub Weight/Cost):\n";
    for (int i = 0; i < numEdges; i++) {
        int u, v;
        long long w;
        cin >> u >> v >> w;
        adj[u].push_back({v, w});
    }

    int sourceHub = stageNodes[0][0];          // Origin in Stage 1
    int destHub = stageNodes[numStages - 1][0]; // Final destination in Stage N

    vector<long long> cost;
    vector<int> nextNode;

    // Initial Multistage DP Computation
    solveMultistageDP(numStages, stageNodes, adj, cost, nextNode);

    cout << "\n=================================================================\n";
    cout << "            SWIFTCARGO MULTISTAGE ROUTE OPTIMIZATION             \n";
    cout << "=================================================================\n";
    if (cost[sourceHub] >= INF) {
        cout << "No valid route available from Source Hub " << sourceHub << " to Destination Hub " << destHub << ".\n";
    } else {
        cout << "Total Optimal Delivery Cost / Time: " << cost[sourceHub] << " units\n";
        printPath(sourceHub, nextNode);
    }
    cout << "=================================================================\n";

    // Dynamic Control & Batch Processing Menu
    int choice;
    do {
        cout << "\n--- SWIFTCARGO LOGISTICS CONTROL MENU ---\n";
        cout << "1. Update real-time route cost (Traffic/Weather/Road Closures)\n";
        cout << "2. Batch Process Delivery Packages (Recalculate Optimal Routes)\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice == 1) {
            int u, v;
            long long newW;
            cout << "Enter Source Hub, Dest Hub, and New Cost (u v new_cost): ";
            cin >> u >> v >> newW;
            updateEdgeWeight(adj, u, v, newW);
        } else if (choice == 2) {
            solveMultistageDP(numStages, stageNodes, adj, cost, nextNode);
            cout << "\n=================================================================\n";
            cout << "               BATCH ROUTING RECALCULATION RESULTS               \n";
            cout << "=================================================================\n";
            if (cost[sourceHub] >= INF) {
                cout << "No valid route available!\n";
            } else {
                cout << "Updated Minimum Delivery Cost: " << cost[sourceHub] << " units\n";
                printPath(sourceHub, nextNode);
            }
            cout << "=================================================================\n";
        }
    } while (choice != 3);

    return 0;
}