// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 413: Arithmetic Slices
 * Difficulty: Medium
 * Tags: Array, Dynamic Programming, Sliding Window
 *
 * Problem Description:
 * An integer array is called arithmetic if it consists of **at least three
 * elements** and if the difference between any two consecutive elements is the
 * same.
 * 
 * * For example, `[1,3,5,7,9]`, `[7,7,7,7]`, and `[3,-1,-5,-9]` are
 * arithmetic sequences.
 * 
 * Given an integer array `nums`, return _the number of
 * arithmetic**subarrays** of_
 * `nums`.
 * 
 * A **subarray** is a contiguous subsequence of the array.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3,4]
 *     **Output:** 3
 * **Explanation:** We have 3 arithmetic slices in nums: [1, 2, 3], [2, 3, 4]
 * and [1,2,3,4] itself.
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1]
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 5000`
 *   * `-1000 <= nums[i] <= 1000`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/arithmetic-slices/
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
    int numberOfArithmeticSlices(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,4]
    // Test case 2: [1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
