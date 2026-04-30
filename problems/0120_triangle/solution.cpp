// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 120: Triangle
 * Difficulty: Medium
 * Tags: Array, Dynamic Programming
 *
 * Problem Description:
 * Given a `triangle` array, return _the minimum path sum from top to bottom_.
 * 
 * For each step, you may move to an adjacent number of the row below. More
 * formally, if you are on index `i` on the current row, you may move to either
 * index `i` or index `i + 1` on the next row.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** triangle = [[2],[3,4],[6,5,7],[4,1,8,3]]
 *     **Output:** 11
 *     **Explanation:** The triangle looks like:
 *        _2_
 *       _3_ 4
 *      6 _5_ 7
 *     4 _1_ 8 3
 * The minimum path sum from top to bottom is 2 + 3 + 5 + 1 = 11 (underlined
 * above).
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** triangle = [[-10]]
 *     **Output:** -10
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= triangle.length <= 200`
 *   * `triangle[0].length == 1`
 *   * `triangle[i].length == triangle[i - 1].length + 1`
 *   * `-104 <= triangle[i][j] <= 104`
 * 
 * 
 * 
 * **Follow up:** Could you do this using only `O(n)` extra space, where `n`
 * is the
 * total number of rows in the triangle?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/triangle/
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
    int minimumTotal(vector<vector<int>>& triangle) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[2],[3,4],[6,5,7],[4,1,8,3]]
    // Test case 2: [[-10]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
