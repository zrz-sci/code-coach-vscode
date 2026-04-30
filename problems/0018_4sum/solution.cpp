// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 18: 4Sum
 * Difficulty: Medium
 * Tags: Array, Two Pointers, Sorting
 *
 * Problem Description:
 * Given an array `nums` of `n` integers, return _an array of all the**unique**
 * quadruplets_ `[nums[a], nums[b], nums[c], nums[d]]` such that:
 * 
 *   * `0 <= a, b, c, d < n`
 *   * `a`, `b`, `c`, and `d` are **distinct**.
 *   * `nums[a] + nums[b] + nums[c] + nums[d] == target`
 * 
 * You may return the answer in **any order**.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,0,-1,0,-2,2], target = 0
 *     **Output:** [[-2,-1,1,2],[-2,0,0,2],[-1,0,0,1]]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [2,2,2,2,2], target = 8
 *     **Output:** [[2,2,2,2]]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 200`
 *   * `-109 <= nums[i] <= 109`
 *   * `-109 <= target <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/4sum/
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
    vector<vector<int>> fourSum(vector<int>& nums, int target) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,0,-1,0,-2,2]
    // Test case 2: 0
    // Test case 3: [2,2,2,2,2]
    // Test case 4: 8

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
