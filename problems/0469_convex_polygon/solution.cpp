// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 469: Convex Polygon
 * Difficulty: Medium
 * Tags: Geometry, Array, Math
 *
 * Problem Description:
 * You are given an array of points on the **X-Y** plane `points` where
 * `points[i]
 * = [xi, yi]`. The points form a polygon when joined sequentially.
 * 
 * Return `true` if this polygon is
 * [convex](http://en.wikipedia.org/wiki/Convex_polygon) and `false` otherwise.
 * 
 * You may assume the polygon formed by given points is always a [simple
 * polygon](http://en.wikipedia.org/wiki/Simple_polygon). In other words, we
 * ensure
 * that exactly two edges intersect at each vertex and that edges otherwise
 * don't
 * intersect each other.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** points = [[0,0],[0,5],[5,5],[5,0]]
 *     **Output:** true
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** points = [[0,0],[0,10],[10,10],[10,0],[5,5]]
 *     **Output:** false
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `3 <= points.length <= 104`
 *   * `points[i].length == 2`
 *   * `-104 <= xi, yi <= 104`
 *   * All the given points are **unique**.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/convex-polygon/
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
    bool isConvex(vector<vector<int>>& points) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[0,0],[0,5],[5,5],[5,0]]
    // Test case 2: [[0,0],[0,10],[10,10],[10,0],[5,5]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
