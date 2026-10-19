#include <iostream>
#include <vector>
#include <queue>
#include <climits>
#include <iomanip>

using namespace std;

const int INF = 1e9; // Infinity for invalid or visited routes

// Structure to represent a node in the state-space tree
struct Node {
    vector<int> path;                  // Traversed path of cities
    vector<vector<int>> reducedMatrix; // Current state of cost matrix
    int cost;                          // Lower bound cost for this sub-path
    int vertex;                        // Current city ID
    int level;                         // Depth in the state space tree

    // Comparator for priority queue (min-heap based on least lower bound cost)
    bool operator>(const Node& other) const {
        return cost > other.cost;
    }
};

// Function to perform row and column reduction on the cost matrix
int reduceMatrix(vector<vector<int>>& matrix, int N) {
    int reductionCost = 0;

    // Row Reduction
    for (int i = 0; i < N; i++) {
        int rowMin = INF;
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] < rowMin) {
                rowMin = matrix[i][j];
            }
        }
        if (rowMin != INF && rowMin > 0) {
            reductionCost += rowMin;
            for (int j = 0; j < N; j++) {
                if (matrix[i][j] != INF) {
                    matrix[i][j] -= rowMin;
                }
            }
        }
    }

    // Column Reduction
    for (int j = 0; j < N; j++) {
        int colMin = INF;
        for (int i = 0; i < N; i++) {
            if (matrix[i][j] < colMin) {
                colMin = matrix[i][j];
            }
        }
        if (colMin != INF && colMin > 0) {
            reductionCost += colMin;
            for (int i = 0; i < N; i++) {
                if (matrix[i][j] != INF) {
                    matrix[i][j] -= colMin;
                }
            }
        }
    }

    return reductionCost;
}

// Function to create a new tree node
Node createNode(const vector<vector<int>>& parentMatrix, const vector<int>& parentPath, 
                int level, int u, int v, int N) {
    Node node;
    node.path = parentPath;
    node.path.push_back(v);
    node.level = level;
    node.vertex = v;
    node.reducedMatrix = parentMatrix;

    // Set row u and column v to INF to prevent further transitions from u or to v
    for (int k = 0; k < N; k++) {
        node.reducedMatrix[u][k] = INF;
        node.reducedMatrix[k][v] = INF;
    }

    // Prevent direct loop back to starting city prematurely
    node.reducedMatrix[v][0] = INF;

    // Calculate dynamic cost: Parent Cost + Edge (u, v) Weight + Matrix Reduction Cost
    int edgeWeight = (parentMatrix[u][v] == INF) ? 0 : parentMatrix[u][v];
    int reductionCost = reduceMatrix(node.reducedMatrix, N);
    node.cost = edgeWeight + reductionCost;

    return node;
}

// Solves TSP using LC Branch and Bound
void solveTSP(int N, vector<vector<int>>& costMatrix) {
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    // Prepare Root Node
    Node root;
    root.path.push_back(0); // Start at City 0
    root.level = 0;
    root.vertex = 0;
    root.reducedMatrix = costMatrix;

    // Initial root reduction
    int initialReduction = reduceMatrix(root.reducedMatrix, N);
    root.cost = initialReduction;

    pq.push(root);

    while (!pq.empty()) {
        Node minNode = pq.top();
        pq.pop();

        int u = minNode.vertex;

        // If all cities are visited, complete tour by returning to starting city 0
        if (minNode.level == N - 1) {
            minNode.path.push_back(0);

            cout << "\n=================================================================\n";
            cout << "            SWIFTSHIP OPTIMAL DELIVERY ROUTE                     \n";
            cout << "=================================================================\n";
            cout << "   [Minimum Travel Cost / Time]: " << minNode.cost << " units\n\n";
            cout << "   [Optimal Delivery Path]: ";
            for (size_t i = 0; i < minNode.path.size(); i++) {
                cout << "City " << minNode.path[i];
                if (i + 1 < minNode.path.size()) cout << "  ===>  ";
            }
            cout << "\n=================================================================\n";
            return;
        }

        // Branching: Explore all unvisited cities from node u
        for (int v = 0; v < N; v++) {
            if (minNode.reducedMatrix[u][v] != INF) {
                Node child = createNode(minNode.reducedMatrix, minNode.path, 
                                        minNode.level + 1, u, v, N);
                child.cost += minNode.cost;
                pq.push(child);
            }
        }
    }
}

int main() {
    int N;
    cout << "Enter total number of cities (N): ";
    cin >> N;

    vector<vector<int>> costMatrix(N, vector<int>(N));

    cout << "\nEnter Cost/Distance Matrix (" << N << "x" << N << "):\n";
    cout << "(Note: Enter 0 or -1 for self-loops/no direct road):\n";

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> costMatrix[i][j];
            if (i == j || costMatrix[i][j] <= 0) {
                costMatrix[i][j] = INF;
            }
        }
    }

    solveTSP(N, costMatrix);

    return 0;
}