// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 149: Max Points on a Line
 * Difficulty: Hard
 * Tags: Geometry, Array, Hash Table, Math
 *
 * Problem Description:
 * Given an array of `points` where `points[i] = [xi, yi]` represents a point
 * on
 * the **X-Y** plane, return _the maximum number of points that lie on the same
 * straight line_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** points = [[1,1],[2,2],[3,3]]
 *     **Output:** 3
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** points = [[1,1],[3,2],[5,3],[4,1],[2,3],[1,4]]
 *     **Output:** 4
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= points.length <= 300`
 *   * `points[i].length == 2`
 *   * `-104 <= xi, yi <= 104`
 *   * All the `points` are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/max-points-on-a-line/
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
    int maxPoints(vector<vector<int>>& points) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,1],[2,2],[3,3]]
    // Test case 2: [[1,1],[3,2],[5,3],[4,1],[2,3],[1,4]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
