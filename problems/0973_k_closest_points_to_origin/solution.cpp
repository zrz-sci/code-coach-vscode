// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 973: K Closest Points to Origin
 * Difficulty: Medium
 * Tags: Geometry, Array, Math, Divide and Conquer, Quickselect, Sorting, Heap (Priority Queue)
 *
 * Problem Description:
 * Given an array of `points` where `points[i] = [xi, yi]` represents a point
 * on
 * the **X-Y** plane and an integer `k`, return the `k` closest points to the
 * origin `(0, 0)`.
 * 
 * The distance between two points on the **X-Y** plane is the Euclidean
 * distance
 * (i.e., `√(x1 - x2)2 + (y1 - y2)2`).
 * 
 * You may return the answer in **any order**. The answer is **guaranteed** to
 * be
 * **unique** (except for the order that it is in).
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** points = [[1,3],[-2,2]], k = 1
 *     **Output:** [[-2,2]]
 *     **Explanation:**
 *     The distance between (1, 3) and the origin is sqrt(10).
 *     The distance between (-2, 2) and the origin is sqrt(8).
 *     Since sqrt(8) < sqrt(10), (-2, 2) is closer to the origin.
 * We only want the closest k = 1 points from the origin, so the answer is
 * just [[-2,2]].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** points = [[3,3],[5,-1],[-2,4]], k = 2
 *     **Output:** [[3,3],[-2,4]]
 *     **Explanation:** The answer [[-2,4],[3,3]] would also be accepted.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= k <= points.length <= 104`
 *   * `-104 <= xi, yi <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/k-closest-points-to-origin/
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
    vector<vector<int>> kClosest(vector<vector<int>>& points, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [[1,3],[-2,2]]
    // Test case 2: 1
    // Test case 3: [[3,3],[5,-1],[-2,4]]
    // Test case 4: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
