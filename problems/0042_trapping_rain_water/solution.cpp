// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 42: Trapping Rain Water
 * Difficulty: Hard
 * Tags: Stack, Array, Two Pointers, Dynamic Programming, Monotonic Stack
 *
 * Problem Description:
 * Given `n` non-negative integers representing an elevation map where the
 * width of
 * each bar is `1`, compute how much water it can trap after raining.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** height = [0,1,0,2,1,0,1,3,2,1,2,1]
 *     **Output:** 6
 * **Explanation:** The above elevation map (black section) is represented by
 * array [0,1,0,2,1,0,1,3,2,1,2,1]. In this case, 6 units of rain water (blue
 * section) are being trapped.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** height = [4,2,0,3,2,5]
 *     **Output:** 9
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == height.length`
 *   * `1 <= n <= 2 * 104`
 *   * `0 <= height[i] <= 105`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/trapping-rain-water/
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
    int trap(vector<int>& height) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [0,1,0,2,1,0,1,3,2,1,2,1]
    // Test case 2: [4,2,0,3,2,5]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
