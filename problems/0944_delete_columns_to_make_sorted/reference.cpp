/*
 * LeetCode 944 - Delete Columns to Make Sorted
 *
 * Approach: Column-by-Column Scan with Early Termination
 *
 * Key Idea:
 *   For each column index j, check whether adjacent rows are in non-decreasing
 *   order. If any pair strs[i][j] > strs[i+1][j], the column is unsorted.
 *   Count the total number of unsorted columns.
 *
 * Time Complexity:  O(m * L) where m = number of strings, L = string length
 * Space Complexity: O(1) auxiliary
 *
 * Reference: ./solution.cpp
 */

// ============================================================
// Solution 1: Direct Column Scan (Optimal)
// ============================================================
class Solution {
public:
    int minDeletionSize(vector<string>& strs) {
        int m = strs.size();       // number of rows (strings)
        int L = strs[0].size();    // number of columns (string length)
        int deletions = 0;

        for (int j = 0; j < L; ++j) {
            // Check if column j is sorted (non-decreasing from top to bottom)
            for (int i = 0; i < m - 1; ++i) {
                if (strs[i][j] > strs[i + 1][j]) {
                    // Column j is NOT sorted; count it and move on
                    ++deletions;
                    break; // no need to check more rows for this column
                }
            }
        }

        return deletions;
    }
};

// ============================================================
// Solution 2: Using std::is_sorted on Extracted Column
// ============================================================
//
// A more "STL-idiomatic" approach: extract each column into a
// temporary string, then use std::is_sorted to check it.
// Same time complexity but uses O(m) extra space per column.
class Solution2 {
public:
    int minDeletionSize(vector<string>& strs) {
        int m = strs.size();
        int L = strs[0].size();
        int deletions = 0;

        for (int j = 0; j < L; ++j) {
            // Extract column j into a string
            string col(m, ' ');
            for (int i = 0; i < m; ++i) {
                col[i] = strs[i][j];
            }

            // Check if the extracted column is sorted
            if (!is_sorted(col.begin(), col.end())) {
                ++deletions;
            }
        }

        return deletions;
    }
};

// ============================================================
// Solution 3: Lambda-Based Approach (C++17 style)
// ============================================================
//
// Uses a functional style with count_if and adjacent_find
// to express the logic more declaratively.
class Solution3 {
public:
    int minDeletionSize(vector<string>& strs) {
        int m = strs.size();
        int L = strs[0].size();
        int deletions = 0;

        for (int j = 0; j < L; ++j) {
            // Use a lambda to check if any adjacent pair is out of order
            bool unsorted = false;
            for (int i = 0; i + 1 < m; ++i) {
                if (strs[i][j] > strs[i + 1][j]) {
                    unsorted = true;
                    break;
                }
            }
            deletions += unsorted;
        }

        return deletions;
    }
};

// ============================================================
// Detailed Walkthrough with Example
// ============================================================
//
// Example: strs = ["cba", "daf", "ghi"]
//
// Grid:
//   c b a    (row 0)
//   d a f    (row 1)
//   g h i    (row 2)
//
// Column 0: 'c', 'd', 'g'
//   'c' <= 'd' OK
//   'd' <= 'g' OK
//   Column 0 is SORTED -> do not delete
//
// Column 1: 'b', 'a', 'h'
//   'b' > 'a' NOT OK -> Column 1 is UNSORTED -> delete!
//   (early break, no need to check 'a' vs 'h')
//
// Column 2: 'a', 'f', 'i'
//   'a' <= 'f' OK
//   'f' <= 'i' OK
//   Column 2 is SORTED -> do not delete
//
// Result: 1 column deleted (column 1)
//
// ============================================================
// Example 2: strs = ["zyx", "wvu", "tsr"]
//
// Grid:
//   z y x    (row 0)
//   w v u    (row 1)
//   t s r    (row 2)
//
// Column 0: 'z' > 'w' -> UNSORTED (delete)
// Column 1: 'y' > 'v' -> UNSORTED (delete)
// Column 2: 'x' > 'u' -> UNSORTED (delete)
//
// Result: 3 columns deleted (all of them)
//
// ============================================================
// Edge Cases
// ============================================================
//
// 1. Single row: strs = ["abc"]
//    No adjacent pairs to compare. All columns are sorted.
//    Result: 0
//
// 2. Single character strings: strs = ["z", "a"]
//    Column 0: 'z' > 'a' -> unsorted
//    Result: 1
//
// 3. All identical strings: strs = ["aaa", "aaa", "aaa"]
//    Every column has all equal characters -> all sorted
//    Result: 0
//
// 4. Already sorted columns: strs = ["abc", "bcd", "cde"]
//    Column 0: a,b,c sorted. Column 1: b,c,d sorted. Column 2: c,d,e sorted.
//    Result: 0
//
// ============================================================
// Comparison with LC 955 and LC 960
// ============================================================
//
// LC 944 (this problem): Check each column independently. O(m * L).
//
// LC 955 (Delete Columns II): Must ensure that ROWS are lexicographically
//   sorted after deletion. Columns are interdependent. Greedy approach
//   with "locked" row pairs. More complex.
//
// LC 960 (Delete Columns III): Must ensure each remaining row is sorted
//   within itself (not between rows). Equivalent to LIS on columns.
//   Uses O(L^2) DP or O(L log L) with patience sorting.
