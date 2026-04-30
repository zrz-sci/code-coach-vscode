// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 667: Beautiful Arrangement II
 * Difficulty: Medium
 * Tags: Array, Math
 *
 * Problem Description:
 * Given two integers `n` and `k`, construct a list `answer` that contains `n`
 * different positive integers ranging from `1` to `n` and obeys the following
 * requirement:
 * 
 * * Suppose this list is `answer = [a1, a2, a3, ... , an]`, then the list
 * `[|a1 - a2|, |a2 - a3|, |a3 - a4|, ... , |an-1 - an|]` has exactly `k`
 * distinct integers.
 * 
 * Return _the list_ `answer`. If there multiple valid answers, return **any of
 * them**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 3, k = 1
 *     **Output:** [1,2,3]
 * Explanation: The [1,2,3] has three different positive integers ranging from
 * 1 to 3, and the [1,1] has exactly 1 distinct integer: 1
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 3, k = 2
 *     **Output:** [1,3,2]
 * Explanation: The [1,3,2] has three different positive integers ranging from
 * 1 to 3, and the [2,1] has exactly 2 distinct integers: 1 and 2.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= k < n <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/beautiful-arrangement-ii/
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
    vector<int> constructArray(int n, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 3
    // Test case 2: 1
    // Test case 3: 3
    // Test case 4: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
