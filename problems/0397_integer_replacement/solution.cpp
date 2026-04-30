// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 397: Integer Replacement
 * Difficulty: Medium
 * Tags: Greedy, Bit Manipulation, Memoization, Dynamic Programming
 *
 * Problem Description:
 * Given a positive integer `n`, you can apply one of the following operations:
 * 
 *   1. If `n` is even, replace `n` with `n / 2`.
 *   2. If `n` is odd, replace `n` with either `n + 1` or `n - 1`.
 * 
 * Return _the minimum number of operations needed for_ `n` _to become_ `1`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 8
 *     **Output:** 3
 *     **Explanation:** 8 -> 4 -> 2 -> 1
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 7
 *     **Output:** 4
 *     **Explanation:** 7 -> 8 -> 4 -> 2 -> 1
 *     or 7 -> 6 -> 3 -> 2 -> 1
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** n = 4
 *     **Output:** 2
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 231 - 1`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/integer-replacement/
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
    int integerReplacement(int n) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 8
    // Test case 2: 7
    // Test case 3: 4

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
