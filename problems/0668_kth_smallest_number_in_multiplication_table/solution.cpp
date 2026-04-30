// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 668: Kth Smallest Number in Multiplication Table
 * Difficulty: Hard
 * Tags: Math, Binary Search
 *
 * Problem Description:
 * Nearly everyone has used the [Multiplication
 * Table](https://en.wikipedia.org/wiki/Multiplication_table). The
 * multiplication
 * table of size `m x n` is an integer matrix `mat` where `mat[i][j] == i * j`
 * (**1-indexed**).
 * 
 * Given three integers `m`, `n`, and `k`, return _the_`kth` _smallest element
 * in
 * the_`m x n` _multiplication table_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** m = 3, n = 3, k = 5
 *     **Output:** 3
 *     **Explanation:** The 5th smallest number is 3.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** m = 2, n = 3, k = 6
 *     **Output:** 6
 *     **Explanation:** The 6th smallest number is 6.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= m, n <= 3 * 104`
 *   * `1 <= k <= m * n`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/kth-smallest-number-in-multiplication-table/
 */

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <algorithm>
#include <climits>
#include <cmath>

using namespace std;

class Solution {
public:
    int findKthNumber(int m, int n, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 3
    // Test case 2: 3
    // Test case 3: 5
    // Test case 4: 2
    // Test case 5: 3
    // Test case 6: 6

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
