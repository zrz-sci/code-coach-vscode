// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 967: Numbers With Same Consecutive Differences
 * Difficulty: Medium
 * Tags: Breadth-First Search, Backtracking
 *
 * Problem Description:
 * Given two integers n and k, return _an array of all the integers of
 * length_`n`
 * _where the difference between every two consecutive digits is_`k`. You may
 * return the answer in **any order**.
 * 
 * Note that the integers should not have leading zeros. Integers as `02` and
 * `043`
 * are not allowed.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 3, k = 7
 *     **Output:** [181,292,707,818,929]
 * **Explanation:** Note that 070 is not a valid number, because it has
 * leading zeroes.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 2, k = 1
 *     **Output:** [10,12,21,23,32,34,43,45,54,56,65,67,76,78,87,89,98]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= n <= 9`
 *   * `0 <= k <= 9`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/numbers-with-same-consecutive-differences/
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
    vector<int> numsSameConsecDiff(int n, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 3
    // Test case 2: 7
    // Test case 3: 2
    // Test case 4: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
