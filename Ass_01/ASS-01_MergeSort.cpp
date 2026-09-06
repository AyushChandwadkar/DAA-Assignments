#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// Structure to represent a customer order
struct Order {
    int orderId;
    string customerName;
    long long timestamp; // Epoch timestamp in seconds
    double orderAmount;
};

// Merges two sorted subarrays into orders[left...right]
// Pre-allocated temp vector avoids dynamic memory allocation overhead during recursion
void merge(vector<Order>& orders, vector<Order>& temp, int left, int mid, int right) {
    int i = left;      // Index for left subarray
    int j = mid + 1;   // Index for right subarray
    int k = left;      // Index for temp array

    while (i <= mid && j <= right) {
        if (orders[i].timestamp <= orders[j].timestamp) {
            temp[k++] = orders[i++];
        } else {
            temp[k++] = orders[j++];
        }
    }

    while (i <= mid) {
        temp[k++] = orders[i++];
    }

    while (j <= right) {
        temp[k++] = orders[j++];
    }

    for (i = left; i <= right; i++) {
        orders[i] = temp[i];
    }
}

// Main Merge Sort function
void mergeSort(vector<Order>& orders, vector<Order>& temp, int left, int right) {
    if (left >= right) return;

    int mid = left + (right - left) / 2;
    mergeSort(orders, temp, left, mid);
    mergeSort(orders, temp, mid + 1, right);
    merge(orders, temp, left, mid, right);
}

void printOrders(const vector<Order>& orders) {
    cout << "\n" << left << setw(10) << "Order ID" 
         << setw(20) << "Customer Name" 
         << setw(15) << "Timestamp" 
         << setw(12) << "Amount ($)" << "\n";
    cout << string(57, '-') << "\n";
    for (const auto& order : orders) {
        cout << left << setw(10) << order.orderId 
             << setw(20) << order.customerName 
             << setw(15) << order.timestamp 
             << setw(12) << fixed << setprecision(2) << order.orderAmount << "\n";
    }
}

int main() {
    int n;
    cout << "Enter the number of customer orders: ";
    cin >> n;

    vector<Order> orders(n);
    for (int i = 0; i < n; i++) {
        cout << "\nEnter details for Order " << i + 1 << ":\n";
        cout << "  Order ID: ";
        cin >> orders[i].orderId;
        cout << "  Customer Name: ";
        cin.ignore();
        getline(cin, orders[i].customerName);
        cout << "  Timestamp (epoch seconds): ";
        cin >> orders[i].timestamp;
        cout << "  Order Amount: ";
        cin >> orders[i].orderAmount;
    }

    // Allocate single temporary array to optimize performance for up to 1M items
    vector<Order> temp(n);

    cout << "\n--- Orders Before Sorting ---";
    printOrders(orders);

    mergeSort(orders, temp, 0, n - 1);

    cout << "\n--- Orders Sorted by Timestamp (Ascending) ---";
    printOrders(orders);

    return 0;
}
