#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

// Structure to represent a Course (Graph Vertex)
struct Course {
    int id;
    string name;
    int degree;
};

// Comparator to sort courses by degree in descending order (Welsh-Powell step)
bool compareDegree(const Course& a, const Course& b) {
    return a.degree > b.degree;
}

// Function to perform Timetable Scheduling using Welsh-Powell Graph Coloring
void solveUniversityTimetable(int V, const vector<Course>& courses, const vector<vector<int>>& adj) {
    vector<Course> sortedCourses = courses;

    // Calculate degree (number of conflicts) for each course
    for (int i = 0; i < V; i++) {
        sortedCourses[i].degree = adj[sortedCourses[i].id].size();
    }

    // Sort vertices by degree in descending order
    sort(sortedCourses.begin(), sortedCourses.end(), compareDegree);

    vector<int> slotAssigned(V, -1); // -1 indicates unassigned
    int totalSlotsUsed = 0;

    for (int i = 0; i < V; i++) {
        int u = sortedCourses[i].id;
        if (slotAssigned[u] != -1) continue; // Already scheduled

        totalSlotsUsed++;
        slotAssigned[u] = totalSlotsUsed; // Assign new time slot

        // Find non-adjacent unassigned courses to assign to the same time slot
        for (int j = i + 1; j < V; j++) {
            int v = sortedCourses[j].id;
            if (slotAssigned[v] == -1) {
                // Check if course 'v' conflicts with any course already assigned to 'totalSlotsUsed'
                bool hasConflict = false;
                for (int k = 0; k < V; k++) {
                    if (slotAssigned[k] == totalSlotsUsed) {
                        // Check if edge exists between v and k
                        for (int neighbor : adj[v]) {
                            if (neighbor == k) {
                                hasConflict = true;
                                break;
                            }
                        }
                    }
                    if (hasConflict) break;
                }

                if (!hasConflict) {
                    slotAssigned[v] = totalSlotsUsed;
                }
            }
        }
    }

    // Display Timetable
    cout << "\n=================================================================\n";
    cout << "            UNIVERSITY EXAM TIMETABLE SCHEDULE                   \n";
    cout << "=================================================================\n";
    cout << left << setw(12) << "Course ID" 
         << setw(30) << "Course Name" 
         << setw(20) << "Assigned Time Slot" << "\n";
    cout << string(62, '-') << "\n";

    for (int i = 0; i < V; i++) {
        cout << left << setw(12) << courses[i].id 
             << setw(30) << courses[i].name 
             << "Time Slot " << slotAssigned[i] << "\n";
    }

    cout << string(62, '-') << "\n";
    cout << "Total Minimum Time Slots Required: " << totalSlotsUsed << "\n";
    cout << "=================================================================\n";
}

int main() {
    int V, E;

    cout << "Enter total number of courses (vertices): ";
    cin >> V;

    vector<Course> courses(V);
    for (int i = 0; i < V; i++) {
        courses[i].id = i;
        cout << "Enter name for Course " << i << ": ";
        cin.ignore();
        getline(cin, courses[i].name);
        courses[i].degree = 0;
    }

    cout << "\nEnter total number of student enrollment conflicts (edges): ";
    cin >> E;

    vector<vector<int>> adj(V);
    cout << "Enter conflict details (Format: Course_ID_1 Course_ID_2):\n";
    for (int i = 0; i < E; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u); // Undirected conflict edge
    }

    solveUniversityTimetable(V, courses, adj);

    return 0;
}