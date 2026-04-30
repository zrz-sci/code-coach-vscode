// 📖 Hints: ./hints.md
// 📝 Reference: ./reference.cpp

/*
 * LeetCode Problem 3623: Count Number of Trapezoids I
 * Difficulty: Medium
 * Tags: Geometry, Array, Hash Table, Math
 *
 * Problem Description:
 * You are given a 2D integer array `points`, where `points[i] = [xi, yi]`
 * represents the coordinates of the `ith` point on the Cartesian plane.
 * 
 * A **horizontal** **trapezoid** is a convex quadrilateral with **at least one
 * pair** of horizontal sides (i.e. parallel to the x-axis). Two lines are
 * parallel
 * if and only if they have the same slope.
 * 
 * Return the _number of unique_** _horizontal_ _trapezoids_** that can be
 * formed
 * by choosing any four distinct points from `points`.
 * 
 * Since the answer may be very large, return it **modulo** `109 + 7`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 * **Input:** points = [[1,0],[2,0],[3,0],[2,2],[3,2]]
 * 
 * **Output:** 3
 * 
 * **Explanation:**
 * 
 * There are three distinct ways to pick four points that form a horizontal
 * trapezoid:
 * 
 *   * Using points `[1,0]`, `[2,0]`, `[3,2]`, and `[2,2]`.
 *   * Using points `[2,0]`, `[3,0]`, `[3,2]`, and `[2,2]`.
 *   * Using points `[1,0]`, `[3,0]`, `[3,2]`, and `[2,2]`.
 * 
 * **Example 2:**
 * 
 * **Input:** points = [[0,0],[1,0],[0,1],[2,1]]
 * 
 * **Output:** 1
 * 
 * **Explanation:**
 * 
 * There is only one horizontal trapezoid that can be formed.
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `4 <= points.length <= 105`
 *   * `-108 <= xi, yi <= 108`
 *   * All points are pairwise distinct.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/count-number-of-trapezoids-i/
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
    int countTrapezoids(vector<vector<int>>& points) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,0],[2,0],[3,0],[2,2],[3,2]]
    // Test case 2: [[0,0],[1,0],[0,1],[2,1]]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
