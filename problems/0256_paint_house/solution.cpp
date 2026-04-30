// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 256: Paint House
 * Difficulty: Medium
 * Tags: Array, Dynamic Programming
 *
 * Problem Description:
 * There is a row of `n` houses, where each house can be painted one of three
 * colors: red, blue, or green. The cost of painting each house with a certain
 * color is different. You have to paint all the houses such that no two
 * adjacent
 * houses have the same color.
 * 
 * The cost of painting each house with a certain color is represented by an
 * `n x
 * 3` cost matrix `costs`.
 * 
 * * For example, `costs[0][0]` is the cost of painting house `0` with the
 * color red; `costs[1][2]` is the cost of painting house 1 with color green,
 * and so on...
 * 
 * Return _the minimum cost to paint all houses_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** costs = [[17,2,17],[16,16,5],[14,3,19]]
 *     **Output:** 10
 * **Explanation:** Paint house 0 into blue, paint house 1 into green, paint
 * house 2 into blue.
 *     Minimum cost: 2 + 5 + 3 = 10.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** costs = [[7,6,2]]
 *     **Output:** 2
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `costs.length == n`
 *   * `costs[i].length == 3`
 *   * `1 <= n <= 100`
 *   * `1 <= costs[i][j] <= 20`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/paint-house/
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
    int minCost(vector<vector<int>>& costs) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[17,2,17],[16,16,5],[14,3,19]]
    // Test case 2: [[7,6,2]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
