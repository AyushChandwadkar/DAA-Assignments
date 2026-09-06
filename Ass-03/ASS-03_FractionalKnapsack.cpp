#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

// Structure for relief supplies
struct Item {
    string name;
    double weight;       // in kg
    double utility;      // value score
    bool isDivisible;    // true if item can be fractionally portioned
    double valuePerKg;   // calculated ratio: utility / weight
};

// Comparator to sort items by utility density (value per kg) descending
bool compareDensity(const Item& a, const Item& b) {
    return a.valuePerKg > b.valuePerKg;
}

void solveFractionalKnapsack(vector<Item>& items, double totalCapacity) {
    // Calculate efficiency ratio
    for (auto& item : items) {
        item.valuePerKg = item.utility / item.weight;
    }

    // Sort items based on value density
    sort(items.begin(), items.end(), compareDensity);

    double currentWeight = 0.0;
    double totalUtility = 0.0;

    cout << "\n" << string(68, '=') << "\n";
    cout << "               EMERGENCY BOAT SHIPMENT MANIFEST              \n";
    cout << string(68, '=') << "\n";
    cout << left << setw(18) << "Item Name" 
         << setw(14) << "Weight (kg)" 
         << setw(14) << "Loaded (kg)" 
         << setw(12) << "Portion" 
         << setw(12) << "Utility" << "\n";
    cout << string(68, '-') << "\n";

    for (const auto& item : items) {
        if (currentWeight >= totalCapacity) break;

        double remainingCap = totalCapacity - currentWeight;

        if (item.weight <= remainingCap) {
            // Take whole item
            currentWeight += item.weight;
            totalUtility += item.utility;

            cout << left << setw(18) << item.name 
                 << setw(14) << fixed << setprecision(1) << item.weight 
                 << setw(14) << item.weight 
                 << setw(12) << "100%" 
                 << setw(12) << setprecision(2) << item.utility << "\n";
        } else {
            if (item.isDivisible) {
                // Take fraction of divisible item
                double fraction = remainingCap / item.weight;
                double fractionUtility = item.utility * fraction;

                currentWeight += remainingCap;
                totalUtility += fractionUtility;

                cout << left << setw(18) << item.name 
                     << setw(14) << fixed << setprecision(1) << item.weight 
                     << setw(14) << remainingCap 
                     << setw(11) << setprecision(1) << (fraction * 100.0) << "%" 
                     << setw(12) << setprecision(2) << fractionUtility << "\n";
            } else {
                // Non-divisible item doesn't fit completely; skip to search for smaller items
                cout << left << setw(18) << item.name 
                     << setw(14) << fixed << setprecision(1) << item.weight 
                     << setw(14) << "0.0 (Skipped)" 
                     << setw(12) << "0%" 
                     << setw(12) << "0.00" << "\n";
            }
        }
    }

    cout << string(68, '-') << "\n";
    cout << "Total Capacity Allowed : " << fixed << setprecision(2) << totalCapacity << " kg\n";
    cout << "Total Weight Transported: " << currentWeight << " kg\n";
    cout << "Total Utility Value     : " << totalUtility << "\n";
    cout << string(68, '=') << "\n";
}

int main() {
    int n;
    double W;

    cout << "Enter total capacity of rescue boat (W in kg): ";
    cin >> W;

    cout << "Enter number of relief supply item types: ";
    cin >> n;

    vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        int divChoice;
        cout << "\nEnter details for Item " << i + 1 << ":\n";
        cout << "  Item Name: ";
        cin.ignore();
        getline(cin, items[i].name);
        cout << "  Weight (kg): ";
        cin >> items[i].weight;
        cout << "  Utility Value: ";
        cin >> items[i].utility;
        cout << "  Is Divisible? (1 for Yes [e.g., Grain/Water], 0 for No [e.g., MedKit]): ";
        cin >> divChoice;
        items[i].isDivisible = (divChoice == 1);
    }

    solveFractionalKnapsack(items, W);

    return 0;
}