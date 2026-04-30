// LeetCode 960: Delete Columns to Make Sorted III
// ./solution.cpp

// ============================================================
// Approach 1: DP (LIS Variant) - Standard
// ============================================================
// Reframe as: find the Longest Subsequence of columns such that
// every row is non-decreasing at those column positions.
// dp[j] = longest valid column subsequence ending at column j.
//
// Time:  O(m^2 * n) where m = column count, n = row count
// Space: O(m)

class Solution {
public:
    int minDeletionSize(vector<string>& strs) {
        int n = strs.size();
        int m = strs[0].size();

        // dp[j] = length of longest valid subsequence of columns ending at column j
        vector<int> dp(m, 1);

        for (int j = 1; j < m; j++) {
            for (int i = 0; i < j; i++) {
                // Check if column i can precede column j in the kept set
                bool canFollow = true;
                for (int k = 0; k < n; k++) {
                    if (strs[k][i] > strs[k][j]) {
                        canFollow = false;
                        break;
                    }
                }
                if (canFollow) {
                    dp[j] = max(dp[j], dp[i] + 1);
                }
            }
        }

        int maxKept = *max_element(dp.begin(), dp.end());
        return m - maxKept;
    }
};


// ============================================================
// Approach 2: DP with Precomputed Compatibility Matrix
// ============================================================
// Separate the compatibility check from the DP logic for clarity.
// Precompute canFollow[i][j] for all pairs (i, j) where i < j.
//
// Time:  O(m^2 * n)
// Space: O(m^2) for the matrix + O(m) for dp

class Solution2 {
public:
    int minDeletionSize(vector<string>& strs) {
        int n = strs.size();
        int m = strs[0].size();

        // Precompute: can column i precede column j?
        // canFollow[i][j] = true iff strs[k][i] <= strs[k][j] for all k
        vector<vector<bool>> canFollow(m, vector<bool>(m, false));
        for (int i = 0; i < m; i++) {
            for (int j = i + 1; j < m; j++) {
                bool valid = true;
                for (int k = 0; k < n; k++) {
                    if (strs[k][i] > strs[k][j]) {
                        valid = false;
                        break;
                    }
                }
                canFollow[i][j] = valid;
            }
        }

        // LIS DP using precomputed matrix
        vector<int> dp(m, 1);
        for (int j = 1; j < m; j++) {
            for (int i = 0; i < j; i++) {
                if (canFollow[i][j]) {
                    dp[j] = max(dp[j], dp[i] + 1);
                }
            }
        }

        int maxKept = *max_element(dp.begin(), dp.end());
        return m - maxKept;
    }
};


// ============================================================
// Approach 3: DP with Early Termination Optimization
// ============================================================
// Same as Approach 1 but with an optimization: if dp[j] already
// equals j+1 (maximum possible), we can skip further checks
// for column j. Also, track global max to enable early exit.
//
// Time:  O(m^2 * n) worst case, but often faster in practice
// Space: O(m)

class Solution3 {
public:
    int minDeletionSize(vector<string>& strs) {
        int n = strs.size();
        int m = strs[0].size();

        vector<int> dp(m, 1);
        int globalMax = 1;

        for (int j = 1; j < m; j++) {
            for (int i = j - 1; i >= 0; i--) {
                // If extending from dp[i] won't beat current dp[j], skip
                if (dp[i] + 1 <= dp[j]) continue;

                // Check compatibility: can column i precede column j?
                bool compatible = true;
                for (int k = 0; k < n; k++) {
                    if (strs[k][i] > strs[k][j]) {
                        compatible = false;
                        break;
                    }
                }

                if (compatible) {
                    dp[j] = dp[i] + 1;
                    // If dp[j] is maximum possible (all columns 0..j kept), stop
                    if (dp[j] == j + 1) break;
                }
            }
            globalMax = max(globalMax, dp[j]);

            // If we can keep all columns so far, the remaining columns
            // can only improve or maintain this, so no early global exit
            // is possible here without checking future columns.
        }

        return m - globalMax;
    }
};


// ============================================================
// Approach 4: Column Bitmask DP (for small m)
// ============================================================
// When m <= 20, we could theoretically enumerate all 2^m subsets
// of columns, but this is exponential. Shown here for conceptual
// completeness -- not practical for m=100.
//
// For each subset S of columns (in order), check if every row
// is non-decreasing at those positions. Track the largest |S|.
//
// Time:  O(2^m * m * n) -- impractical for large m
// Space: O(2^m)
//
// NOTE: This approach is included for educational purposes only.
// The O(m^2 * n) DP approach above is the intended solution.

class Solution4 {
public:
    int minDeletionSize(vector<string>& strs) {
        int n = strs.size();
        int m = strs[0].size();

        // Only feasible for small m (demonstration with m <= 15)
        if (m > 15) {
            // Fall back to standard DP for large m
            Solution s;
            return s.minDeletionSize(strs);
        }

        int maxKept = 1;

        for (int mask = 1; mask < (1 << m); mask++) {
            int bits = __builtin_popcount(mask);
            if (bits <= maxKept) continue; // can't improve

            // Extract column indices in order
            vector<int> cols;
            for (int b = 0; b < m; b++) {
                if (mask & (1 << b)) cols.push_back(b);
            }

            // Check if every row is non-decreasing at these columns
            bool valid = true;
            for (int k = 0; k < n && valid; k++) {
                for (int idx = 1; idx < (int)cols.size(); idx++) {
                    if (strs[k][cols[idx - 1]] > strs[k][cols[idx]]) {
                        valid = false;
                        break;
                    }
                }
            }

            if (valid) {
                maxKept = bits;
            }
        }

        return m - maxKept;
    }
};
