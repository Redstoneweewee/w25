#include <iostream>
#include <vector>

using namespace std;

vector<vector<size_t>> countTileSteps(size_t m, size_t n) {
    // Initialize a DP table to store the number of ways to reach each tile
    vector<vector<size_t>> dp(m, vector<size_t>(n, 0));
    
    // Initialize a step count table to store the number of times each tile is stepped on
    vector<vector<size_t>> stepCount(m, vector<size_t>(n, 0));
    
    // Base case: There's only one way to reach the starting tile (0, 0)
    dp[0][0] = 1;
    
    // Fill the DP table
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (i > 0) {
                dp[i][j] += dp[i-1][j]; // Move down
            }
            if (j > 0) {
                dp[i][j] += dp[i][j-1]; // Move right
            }
        }
    }
    
    // Calculate the number of times each tile is stepped on
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            // Number of ways to reach (i, j)
            size_t waysToReach = dp[i][j];
            // Number of ways to go from (i, j) to (m-1, n-1)
            size_t waysToEnd = dp[m-1-i][n-1-j];
            // Total steps on (i, j) is waysToReach * waysToEnd
            stepCount[i][j] = waysToReach * waysToEnd;
        }
    }
    
    return stepCount;
}

int main() {
    size_t m, n;
    cout << "Enter the number of rows (m): ";
    cin >> m;
    cout << "Enter the number of columns (n): ";
    cin >> n;
    
    vector<vector<size_t>> steps = countTileSteps(m, n);
    
    cout << "Number of times each tile is stepped on:" << endl;
    for (size_t i = 0; i < m; ++i) {
        for (size_t j = 0; j < n; ++j) {
            cout << steps[i][j] << " ";
        }
        cout << endl;
    }
    
    return 0;
}