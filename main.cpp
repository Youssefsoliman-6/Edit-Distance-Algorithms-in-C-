#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>  // For INT_MAX

using namespace std;

// Brute Force (Recursive)
int editDistanceRecursive(const string& s1, const string& s2, int m, int n) {
    if (m == 0) return n;
    if (n == 0) return m;

    if (s1[m - 1] == s2[n - 1])
        return editDistanceRecursive(s1, s2, m - 1, n - 1);

    return 1 + min({
        editDistanceRecursive(s1, s2, m, n - 1),     // Insert
        editDistanceRecursive(s1, s2, m - 1, n),     // Delete
        editDistanceRecursive(s1, s2, m - 1, n - 1)  // Replace
    });
}

// Memoization (Top-Down DP)
int editDistanceMemo(const string& s1, const string& s2, int m, int n, vector<vector<int>>& memo) {
    if (m == 0) return n;
    if (n == 0) return m;

    if (memo[m][n] != -1)
        return memo[m][n];

    if (s1[m - 1] == s2[n - 1])
        memo[m][n] = editDistanceMemo(s1, s2, m - 1, n - 1, memo);
    else
        memo[m][n] = 1 + min({
            editDistanceMemo(s1, s2, m, n - 1, memo),     // Insert
            editDistanceMemo(s1, s2, m - 1, n, memo),     // Delete
            editDistanceMemo(s1, s2, m - 1, n - 1, memo)  // Replace
        });

    return memo[m][n];
}

// Bottom-Up Dynamic Programming
int editDistanceDP(const string& s1, const string& s2) {
    size_t m = s1.length();
    size_t n = s2.length();
    vector<vector<int>> dp(m + 1, vector<int>(n + 1));

    for (size_t i = 0; i <= m; ++i) {
        for (size_t j = 0; j <= n; ++j) {
            if (i == 0)
                dp[i][j] = static_cast<int>(j);
            else if (j == 0)
                dp[i][j] = static_cast<int>(i);
            else if (s1[i - 1] == s2[j - 1])
                dp[i][j] = dp[i - 1][j - 1];
            else
                dp[i][j] = 1 + min({
                    dp[i][j - 1],    // Insert
                    dp[i - 1][j],    // Delete
                    dp[i - 1][j - 1] // Replace
                });
        }
    }

    return dp[m][n];
}

// Helper function for Divide & Conquer
vector<int> computeLastRowOfDP(const string& s1, const string& s2) {
    size_t m = s1.size(), n = s2.size();
    vector<int> prev(n + 1), curr(n + 1);

    for (size_t j = 0; j <= n; ++j)
        prev[j] = static_cast<int>(j);

    for (size_t i = 1; i <= m; ++i) {
        curr[0] = static_cast<int>(i);
        for (size_t j = 1; j <= n; ++j) {
            if (s1[i - 1] == s2[j - 1])
                curr[j] = prev[j - 1];
            else
                curr[j] = 1 + min({ prev[j], curr[j - 1], prev[j - 1] });
        }
        prev = curr;
    }

    return prev;
}

// Divide and Conquer (Space-Efficient DP)
int editDistanceDivideConquer(const string& s1, const string& s2) {
    size_t m = s1.size(), n = s2.size();

    if (m == 0) return static_cast<int>(n);
    if (n == 0) return static_cast<int>(m);

    if (m == 1 || n == 1) {
        return editDistanceDP(s1, s2);  // Use standard DP for small inputs
    }

    size_t mid = m / 2;
    string left = s1.substr(0, mid);
    string right = s1.substr(mid);

    vector<int> l1 = computeLastRowOfDP(left, s2);
    string rev_right = string(right.rbegin(), right.rend());
    string rev_s2 = string(s2.rbegin(), s2.rend());
    vector<int> l2 = computeLastRowOfDP(rev_right, rev_s2);

    int min_dist = INT_MAX;
    size_t split = 0;
    for (size_t j = 0; j <= n; ++j) {
        int dist = l1[j] + l2[n - j];
        if (dist < min_dist) {
            min_dist = dist;
            split = j;
        }
    }

    return editDistanceDivideConquer(left, s2.substr(0, split)) +
           editDistanceDivideConquer(right, s2.substr(split));
}

// Main Program
int main() {
    string str1, str2;
    char choice;

    do {
        cout << "Please enter string one:\n";
        getline(cin >> ws, str1);
        cout << "Please enter string two:\n";
        getline(cin, str2);

        cout << "\nEdit Distance (Divide and Conquer): "
             << editDistanceDivideConquer(str1, str2) << endl;

        if (str1.length() <= 10 && str2.length() <= 10) {
            cout << "Edit Distance (Brute Force): "
                 << editDistanceRecursive(str1, str2, str1.length(), str2.length()) << endl;
        } else {
            cout << "Edit Distance (Brute Force): Skipped (too slow for long strings)" << endl;
        }

        cout << "Edit Distance (Dynamic Programming): "
             << editDistanceDP(str1, str2) << endl;

        // Optional: Memoized version
        vector<vector<int>> memo(str1.length() + 1, vector<int>(str2.length() + 1, -1));
        cout << "Edit Distance (Memoized Recursion): "
             << editDistanceMemo(str1, str2, str1.length(), str2.length(), memo) << endl;

        cout << "\nWould you like to try again? (y/n): ";
        cin >> choice;
        cin.ignore();  // Clear newline character from input buffer
        cout << endl;

    } while (choice == 'y' || choice == 'Y');

    return 0;
}
