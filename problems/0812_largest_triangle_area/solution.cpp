// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 812: Largest Triangle Area
 * Difficulty: Easy
 * Tags: Geometry, Array, Math
 *
 * Problem Description:
 * Given an array of points on the **X-Y** plane `points` where `points[i] =
 * [xi,
 * yi]`, return _the area of the largest triangle that can be formed by any
 * three
 * different points_. Answers within `10-5` of the actual answer will be
 * accepted.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** points = [[0,0],[0,1],[1,0],[0,2],[2,0]]
 *     **Output:** 2.00000
 * **Explanation:** The five points are shown in the above figure. The red
 * triangle is the largest.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** points = [[1,0],[0,0],[0,1]]
 *     **Output:** 0.50000
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `3 <= points.length <= 50`
 *   * `-50 <= xi, yi <= 50`
 *   * All the given points are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/largest-triangle-area/
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
    double largestTriangleArea(vector<vector<int>>& points) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[0,0],[0,1],[1,0],[0,2],[2,0]]
    // Test case 2: [[1,0],[0,0],[0,1]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
