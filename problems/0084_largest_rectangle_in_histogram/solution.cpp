// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 84: Largest Rectangle in Histogram
 * Difficulty: Hard
 * Tags: Stack, Array, Monotonic Stack
 *
 * Problem Description:
 * Given an array of integers `heights` representing the histogram's bar height
 * where the width of each bar is `1`, return _the area of the largest
 * rectangle in
 * the histogram_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** heights = [2,1,5,6,2,3]
 *     **Output:** 10
 *     **Explanation:** The above is a histogram where width of each bar is 1.
 * The largest rectangle is shown in the red area, which has an area = 10
 * units.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** heights = [2,4]
 *     **Output:** 4
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= heights.length <= 105`
 *   * `0 <= heights[i] <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/largest-rectangle-in-histogram/
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
    int largestRectangleArea(vector<int>& heights) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [2,1,5,6,2,3]
    // Test case 2: [2,4]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
