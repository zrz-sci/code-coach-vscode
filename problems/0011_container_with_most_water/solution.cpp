// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 11: Container With Most Water
 * Difficulty: Medium
 * Tags: Greedy, Array, Two Pointers
 *
 * Problem Description:
 * You are given an integer array `height` of length `n`. There are `n`
 * vertical
 * lines drawn such that the two endpoints of the `ith` line are `(i, 0)` and
 * `(i,
 * height[i])`.
 * 
 * Find two lines that together with the x-axis form a container, such that the
 * container contains the most water.
 * 
 * Return _the maximum amount of water a container can store_.
 * 
 * **Notice** that you may not slant the container.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** height = [1,8,6,2,5,4,8,3,7]
 *     **Output:** 49
 * **Explanation:** The above vertical lines are represented by array
 * [1,8,6,2,5,4,8,3,7]. In this case, the max area of water (blue section) the
 * container can contain is 49.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** height = [1,1]
 *     **Output:** 1
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == height.length`
 *   * `2 <= n <= 105`
 *   * `0 <= height[i] <= 104`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/container-with-most-water/
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
    int maxArea(vector<int>& height) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,8,6,2,5,4,8,3,7]
    // Test case 2: [1,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
