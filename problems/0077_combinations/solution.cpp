// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 77: Combinations
 * Difficulty: Medium
 * Tags: Backtracking
 *
 * Problem Description:
 * Given two integers `n` and `k`, return _all possible combinations of_ `k`
 * _numbers chosen from the range_ `[1, n]`.
 * 
 * You may return the answer in **any order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** n = 4, k = 2
 *     **Output:** [[1,2],[1,3],[1,4],[2,3],[2,4],[3,4]]
 *     **Explanation:** There are 4 choose 2 = 6 total combinations.
 * Note that combinations are unordered, i.e., [1,2] and [2,1] are considered
 * to be the same combination.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** n = 1, k = 1
 *     **Output:** [[1]]
 *     **Explanation:** There is 1 choose 1 = 1 total combination.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= n <= 20`
 *   * `1 <= k <= n`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/combinations/
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
    vector<vector<int>> combine(int n, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: 4
    // Test case 2: 2
    // Test case 3: 1
    // Test case 4: 1

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
