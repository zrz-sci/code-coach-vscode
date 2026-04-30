// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 223: Rectangle Area
 * Difficulty: Medium
 * Tags: Geometry, Math
 *
 * Problem Description:
 * Given the coordinates of two **rectilinear** rectangles in a 2D plane,
 * return
 * _the total area covered by the two rectangles_.
 * 
 * The first rectangle is defined by its **bottom-left** corner `(ax1, ay1)`
 * and
 * its **top-right** corner `(ax2, ay2)`.
 * 
 * The second rectangle is defined by its **bottom-left** corner `(bx1, by1)`
 * and
 * its **top-right** corner `(bx2, by2)`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 * **Input:** ax1 = -3, ay1 = 0, ax2 = 3, ay2 = 4, bx1 = 0, by1 = -1, bx2 = 9,
 * by2 = 2
 *     **Output:** 45
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 * **Input:** ax1 = -2, ay1 = -2, ax2 = 2, ay2 = 2, bx1 = -2, by1 = -2, bx2 =
 * 2, by2 = 2
 *     **Output:** 16
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `-104 <= ax1 <= ax2 <= 104`
 *   * `-104 <= ay1 <= ay2 <= 104`
 *   * `-104 <= bx1 <= bx2 <= 104`
 *   * `-104 <= by1 <= by2 <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/rectangle-area/
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
    int computeArea(int ax1, int ay1, int ax2, int ay2, int bx1, int by1, int bx2, int by2) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: -3
    // Test case 2: 0
    // Test case 3: 3
    // Test case 4: 4
    // Test case 5: 0
    // Test case 6: -1
    // Test case 7: 9
    // Test case 8: 2
    // Test case 9: -2
    // Test case 10: -2
    // Test case 11: 2
    // Test case 12: 2
    // Test case 13: -2
    // Test case 14: -2
    // Test case 15: 2
    // Test case 16: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
