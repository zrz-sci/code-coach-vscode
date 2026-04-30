// LeetCode 955: Delete Columns to Make Sorted II
// Difficulty: Medium
// Tags: Greedy, Array, String
// Link: https://leetcode.com/problems/delete-columns-to-make-sorted-ii/
//
// Approach: Greedy column scanning with "settled pairs" tracking.
// Process columns left to right. Keep a column if it doesn't create
// inversions among unsettled pairs. Mark pairs as settled when a kept
// column strictly separates them.
//
// Time:  O(n * m) where n = #strings, m = string length
// Space: O(n) for the settled array

// ============================================================
// Solution 1: Greedy with Settled Pairs (Optimal)
// ============================================================
// Key idea: Two adjacent strings are "settled" once we've kept a column
// where strs[i][col] < strs[i+1][col] (strictly). After that, no
// future column can break their ordering. We only worry about
// unsettled pairs when deciding whether to keep or delete a column.

class Solution {
public:
    int minDeletionSize(vector<string>& strs) {
        int n = strs.size();
        int m = strs[0].size();
        int deletions = 0;

        // settled[i] = true means pair (strs[i], strs[i+1]) is resolved
        // A settled pair is guaranteed to be in correct lex order
        vector<bool> settled(n - 1, false);

        for (int col = 0; col < m; col++) {
            // Phase 1: Check if keeping this column is safe
            // A column is safe iff for every unsettled pair (i, i+1),
            // strs[i][col] <= strs[i+1][col]
            bool canKeep = true;
            for (int i = 0; i < n - 1; i++) {
                if (!settled[i] && strs[i][col] > strs[i + 1][col]) {
                    canKeep = false;
                    break;
                }
            }

            if (!canKeep) {
                // Must delete this column -- it would create an inversion
                deletions++;
                continue;
            }

            // Phase 2: Keep this column and update settled status
            // If strs[i][col] < strs[i+1][col] (strictly less) for an
            // unsettled pair, that pair becomes settled forever
            bool allSettled = true;
            for (int i = 0; i < n - 1; i++) {
                if (!settled[i]) {
                    if (strs[i][col] < strs[i + 1][col]) {
                        settled[i] = true;
                    } else {
                        allSettled = false;
                    }
                }
            }

            // Early termination: if all pairs are settled, no more
            // deletions are needed regardless of remaining columns
            if (allSettled) break;
        }

        return deletions;
    }
};


// ============================================================
// Solution 2: Greedy with String Building
// ============================================================
// Instead of tracking settled pairs, build a "current prefix" for
// each string by appending characters from kept columns. Check
// whether the prefixes maintain lex order after each column addition.
// Simpler to understand but uses more space.

class SolutionStringBuild {
public:
    int minDeletionSize(vector<string>& strs) {
        int n = strs.size();
        int m = strs[0].size();
        int deletions = 0;

        // cur[i] = the string built so far for strs[i] from kept columns
        vector<string> cur(n, "");

        for (int col = 0; col < m; col++) {
            // Tentatively add this column
            vector<string> candidate(n);
            for (int i = 0; i < n; i++) {
                candidate[i] = cur[i] + strs[i][col];
            }

            // Check if candidate strings are in lex order
            bool sorted = true;
            for (int i = 0; i < n - 1; i++) {
                if (candidate[i] > candidate[i + 1]) {
                    sorted = false;
                    break;
                }
            }

            if (sorted) {
                // Keep this column
                cur = move(candidate);
            } else {
                // Delete this column
                deletions++;
            }
        }

        return deletions;
    }
};


// ============================================================
// Solution 3: Greedy with Detailed State Tracking
// ============================================================
// A variant that counts how many unsettled pairs remain and
// exits early when none are left. Also demonstrates how to
// reason about the correctness of the greedy approach.

class SolutionDetailed {
public:
    int minDeletionSize(vector<string>& strs) {
        int n = strs.size();
        int m = strs[0].size();
        int deletions = 0;
        int unsettledCount = n - 1;

        vector<bool> settled(n - 1, false);

        for (int col = 0; col < m && unsettledCount > 0; col++) {
            // Check safety: only unsettled pairs matter
            bool safe = true;
            for (int i = 0; i < n - 1; i++) {
                if (!settled[i] && strs[i][col] > strs[i + 1][col]) {
                    safe = false;
                    break;
                }
            }

            if (!safe) {
                deletions++;
                continue;
            }

            // Update: mark newly settled pairs
            for (int i = 0; i < n - 1; i++) {
                if (!settled[i] && strs[i][col] < strs[i + 1][col]) {
                    settled[i] = true;
                    unsettledCount--;
                }
            }
        }

        return deletions;
    }
};


// ============================================================
// Walkthrough with Example: strs = ["ca", "bb", "ac"]
// ============================================================
//
// n = 3, m = 2
// Pairs to track: (0,1) and (1,2)
// settled = [false, false]
//
// Column 0: characters = ['c', 'b', 'a']
//   Check unsettled pair (0,1): strs[0][0]='c' > strs[1][0]='b' -> UNSAFE!
//   Delete column 0. deletions = 1.
//
// Column 1: characters = ['a', 'b', 'c']
//   Check unsettled pair (0,1): strs[0][1]='a' <= strs[1][1]='b' -> ok
//   Check unsettled pair (1,2): strs[1][1]='b' <= strs[2][1]='c' -> ok
//   Safe to keep!
//   Update settled:
//     Pair (0,1): 'a' < 'b' -> settled[0] = true
//     Pair (1,2): 'b' < 'c' -> settled[1] = true
//   All settled -> done.
//
// Result: deletions = 1. Correct!
//
//
// Walkthrough with Example: strs = ["xc", "yb", "za"]
// ============================================================
//
// n = 3, m = 2
// settled = [false, false]
//
// Column 0: characters = ['x', 'y', 'z']
//   Check pair (0,1): 'x' <= 'y' -> ok
//   Check pair (1,2): 'y' <= 'z' -> ok
//   Safe to keep!
//   Update: 'x' < 'y' -> settled[0] = true
//           'y' < 'z' -> settled[1] = true
//   All settled -> done early.
//
// Result: deletions = 0. Correct!
//
//
// Walkthrough with Example: strs = ["zyx", "wvu", "tsr"]
// ============================================================
//
// n = 3, m = 3
// settled = [false, false]
//
// Column 0: chars = ['z', 'w', 't']
//   Pair (0,1): 'z' > 'w' -> UNSAFE! Delete. deletions = 1.
//
// Column 1: chars = ['y', 'v', 's']
//   Pair (0,1): 'y' > 'v' -> UNSAFE! Delete. deletions = 2.
//
// Column 2: chars = ['x', 'u', 'r']
//   Pair (0,1): 'x' > 'u' -> UNSAFE! Delete. deletions = 3.
//
// Result: deletions = 3. Correct! Must delete all columns.
