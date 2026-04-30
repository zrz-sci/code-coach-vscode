// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 836: Rectangle Overlap
 * Difficulty: Easy
 * Tags: Geometry, Math
 *
 * Problem Description:
 * An axis-aligned rectangle is represented as a list `[x1, y1, x2, y2]`, where
 * `(x1, y1)` is the coordinate of its bottom-left corner, and `(x2, y2)` is
 * the
 * coordinate of its top-right corner. Its top and bottom edges are parallel
 * to the
 * X-axis, and its left and right edges are parallel to the Y-axis.
 * 
 * Two rectangles overlap if the area of their intersection is **positive**.
 * To be
 * clear, two rectangles that only touch at the corner or edges do not overlap.
 * 
 * Given two axis-aligned rectangles `rec1` and `rec2`, return `true` _if they
 * overlap, otherwise return_`false`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** rec1 = [0,0,2,2], rec2 = [1,1,3,3]
 *     **Output:** true
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** rec1 = [0,0,1,1], rec2 = [1,0,2,1]
 *     **Output:** false
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** rec1 = [0,0,1,1], rec2 = [2,2,3,3]
 *     **Output:** false
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `rec1.length == 4`
 *   * `rec2.length == 4`
 *   * `-109 <= rec1[i], rec2[i] <= 109`
 *   * `rec1` and `rec2` represent a valid rectangle with a non-zero area.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/rectangle-overlap/
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
    bool isRectangleOverlap(vector<int>& rec1, vector<int>& rec2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [0,0,2,2]
    // Test case 2: [1,1,3,3]
    // Test case 3: [0,0,1,1]
    // Test case 4: [1,0,2,1]
    // Test case 5: [0,0,1,1]
    // Test case 6: [2,2,3,3]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
