#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

// Structure to represent a relief supply item
struct Item {
    string name;
    int weight;           // Weight in kg
    int baseUtility;      // Base utility score
    int priority;         // 1: Critical/Perishable (e.g. Medicine/Food), 2: Standard (e.g. Tents)
    int effectiveUtility; // Adjusted utility based on priority multiplier
};

// Function to solve 0/1 Knapsack using Dynamic Programming
void solveDisasterReliefKnapsack(int W, int N, const vector<Item>& items) {
    // DP table where dp[i][w] stores maximum utility for first i items with weight limit w
    vector<vector<int>> dp(N + 1, vector<int>(W + 1, 0));

    // Build DP table bottom-up
    for (int i = 1; i <= N; i++) {
        for (int w = 0; w <= W; w++) {
            if (items[i - 1].weight <= w) {
                dp[i][w] = max(dp[i - 1][w], 
                               items[i - 1].effectiveUtility + dp[i - 1][w - items[i - 1].weight]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    // Trace back to find which items were selected
    vector<Item> selectedItems;
    int currentWeight = W;
    int totalBaseUtility = 0;
    int totalEffectiveUtility = dp[N][W];

    for (int i = N; i > 0 && currentWeight > 0; i--) {
        if (dp[i][currentWeight] != dp[i - 1][currentWeight]) {
            selectedItems.push_back(items[i - 1]);
            totalBaseUtility += items[i - 1].baseUtility;
            currentWeight -= items[i - 1].weight;
        }
    }

    // Display Manifest Output
    cout << "\n=================================================================\n";
    cout << "             OPTIMAL DISASTER RELIEF TRUCK MANIFEST              \n";
    cout << "=================================================================\n";
    cout << left << setw(22) << "Item Name" 
         << setw(12) << "Weight(kg)" 
         << setw(12) << "Priority" 
         << setw(12) << "Base Util" 
         << setw(12) << "Eff. Util" << "\n";
    cout << string(68, '-') << "\n";

    int totalWeightLoaded = 0;
    for (const auto& item : selectedItems) {
        totalWeightLoaded += item.weight;
        string priorityStr = (item.priority == 1) ? "Critical" : "Standard";
        cout << left << setw(22) << item.name 
             << setw(12) << item.weight 
             << setw(12) << priorityStr 
             << setw(12) << item.baseUtility 
             << setw(12) << item.effectiveUtility << "\n";
    }

    cout << string(68, '-') << "\n";
    cout << "Truck Capacity Limit   : " << W << " kg\n";
    cout << "Total Weight Transported: " << totalWeightLoaded << " kg\n";
    cout << "Remaining Unused Capacity: " << (W - totalWeightLoaded) << " kg\n";
    cout << "Total Base Utility      : " << totalBaseUtility << "\n";
    cout << "Total Effective Utility : " << totalEffectiveUtility << "\n";
    cout << "=================================================================\n";
}

int main() {
    int W, N;

    cout << "Enter total truck weight capacity (W in kg): ";
    cin >> W;

    cout << "Enter total number of relief supply item types (N): ";
    cin >> N;

    vector<Item> items(N);
    for (int i = 0; i < N; i++) {
        cout << "\nEnter details for Item " << i + 1 << ":\n";
        cout << "  Item Name: ";
        cin.ignore();
        getline(cin, items[i].name);
        cout << "  Weight (kg): ";
        cin >> items[i].weight;
        cout << "  Base Utility Score: ";
        cin >> items[i].baseUtility;
        cout << "  Priority (1: Critical/Perishable [Medicine/Food], 2: Standard [Tents/Blankets]): ";
        cin >> items[i].priority;

        // Critical items receive a 2x priority multiplier to enforce priority loading
        int multiplier = (items[i].priority == 1) ? 2 : 1;
        items[i].effectiveUtility = items[i].baseUtility * multiplier;
    }

    solveDisasterReliefKnapsack(W, N, items);

    return 0;
}
