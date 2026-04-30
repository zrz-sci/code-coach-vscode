// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 34: Find First and Last Position of Element in Sorted Array
 * Difficulty: Medium
 * Tags: Array, Binary Search
 *
 * Problem Description:
 * Given an array of integers `nums` sorted in non-decreasing order, find the
 * starting and ending position of a given `target` value.
 * 
 * If `target` is not found in the array, return `[-1, -1]`.
 * 
 * You must write an algorithm with `O(log n)` runtime complexity.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [5,7,7,8,8,10], target = 8
 *     **Output:** [3,4]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [5,7,7,8,8,10], target = 6
 *     **Output:** [-1,-1]
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [], target = 0
 *     **Output:** [-1,-1]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `0 <= nums.length <= 105`
 *   * `-109 <= nums[i] <= 109`
 *   * `nums` is a non-decreasing array.
 *   * `-109 <= target <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-first-and-last-position-of-element-in-sorted-array/
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
    vector<int> searchRange(vector<int>& nums, int target) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [5,7,7,8,8,10]
    // Test case 2: 8
    // Test case 3: [5,7,7,8,8,10]
    // Test case 4: 6
    // Test case 5: []
    // Test case 6: 0

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
