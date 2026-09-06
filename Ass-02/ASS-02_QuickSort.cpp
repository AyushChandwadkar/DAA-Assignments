#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

// Structure to represent a Movie item
struct Movie {
    int id;
    string title;
    double imdbRating;
    int releaseYear;
    long long watchTimeMinutes;
};

// Universal Comparator according to metric choice
bool isBetter(const Movie& a, const Movie& b, int criterion, bool descending) {
    double valA = 0, valB = 0;
    if (criterion == 1) {
        valA = a.imdbRating;
        valB = b.imdbRating;
    } else if (criterion == 2) {
        valA = a.releaseYear;
        valB = b.releaseYear;
    } else if (criterion == 3) {
        valA = a.watchTimeMinutes;
        valB = b.watchTimeMinutes;
    }

    if (descending) {
        return valA > valB; // Higher value prioritized
    } else {
        return valA < valB; // Lower value prioritized
    }
}

// Partition function with Randomized Pivot Selection to prevent O(N^2) worst case
int partition(vector<Movie>& movies, int low, int high, int criterion, bool descending) {
    int randomIndex = low + rand() % (high - low + 1);
    swap(movies[randomIndex], movies[high]);

    Movie pivot = movies[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (isBetter(movies[j], pivot, criterion, descending)) {
            i++;
            swap(movies[i], movies[j]);
        }
    }
    swap(movies[i + 1], movies[high]);
    return i + 1;
}

// QuickSort Algorithm
void quickSort(vector<Movie>& movies, int low, int high, int criterion, bool descending) {
    if (low < high) {
        int pivotIndex = partition(movies, low, high, criterion, descending);
        quickSort(movies, low, pivotIndex - 1, criterion, descending);
        quickSort(movies, pivotIndex + 1, high, criterion, descending);
    }
}

void displayMovies(const vector<Movie>& movies) {
    cout << "\n" << left << setw(8) << "ID" 
         << setw(25) << "Title" 
         << setw(12) << "IMDb Rating" 
         << setw(15) << "Release Year" 
         << setw(18) << "Watch Time (Mins)" << "\n";
    cout << string(78, '-') << "\n";
    for (const auto& movie : movies) {
        cout << left << setw(8) << movie.id 
             << setw(25) << movie.title 
             << setw(12) << fixed << setprecision(1) << movie.imdbRating 
             << setw(15) << movie.releaseYear 
             << setw(18) << movie.watchTimeMinutes << "\n";
    }
}

int main() {
    srand(time(0));

    int n;
    cout << "Enter the number of movies in catalog: ";
    cin >> n;

    vector<Movie> movies(n);
    for (int i = 0; i < n; i++) {
        cout << "\nEnter details for Movie " << i + 1 << ":\n";
        cout << "  Movie ID: ";
        cin >> movies[i].id;
        cout << "  Title: ";
        cin.ignore();
        getline(cin, movies[i].title);
        cout << "  IMDb Rating (0.0 - 10.0): ";
        cin >> movies[i].imdbRating;
        cout << "  Release Year: ";
        cin >> movies[i].releaseYear;
        cout << "  Watch Time Popularity (minutes): ";
        cin >> movies[i].watchTimeMinutes;
    }

    int criterion;
    cout << "\nSelect Sorting Parameter:\n";
    cout << "1. IMDb Rating\n";
    cout << "2. Release Year\n";
    cout << "3. Watch Time Popularity\n";
    cout << "Enter choice (1-3): ";
    cin >> criterion;

    int orderChoice;
    cout << "Select Sort Order (1: High-to-Low / Descending, 2: Low-to-High / Ascending): ";
    cin >> orderChoice;
    bool descending = (orderChoice == 1);

    quickSort(movies, 0, n - 1, criterion, descending);

    cout << "\n--- StreamFlix Sorted Recommendations ---";
    displayMovies(movies);

    return 0;
}