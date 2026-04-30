// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 665: Non-decreasing Array
 * Difficulty: Medium
 * Tags: Array
 *
 * Problem Description:
 * Given an array `nums` with `n` integers, your task is to check if it could
 * become non-decreasing by modifying **at most one element**.
 * 
 * We define an array is non-decreasing if `nums[i] <= nums[i + 1]` holds for
 * every
 * `i` (**0-based**) such that (`0 <= i <= n - 2`).
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [4,2,3]
 *     **Output:** true
 * **Explanation:** You could modify the first 4 to 1 to get a non-decreasing
 * array.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [4,2,1]
 *     **Output:** false
 * **Explanation:** You cannot get a non-decreasing array by modifying at most
 * one element.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == nums.length`
 *   * `1 <= n <= 104`
 *   * `-105 <= nums[i] <= 105`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/non-decreasing-array/
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
    bool checkPossibility(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [4,2,3]
    // Test case 2: [4,2,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
