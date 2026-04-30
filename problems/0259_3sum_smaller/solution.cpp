// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 259: 3Sum Smaller
 * Difficulty: Medium
 * Tags: Array, Two Pointers, Binary Search, Sorting
 *
 * Problem Description:
 * Given an array of `n` integers `nums` and an integer `target`, find the
 * number
 * of index triplets `i`, `j`, `k` with `0 <= i < j < k < n` that satisfy the
 * condition `nums[i] + nums[j] + nums[k] < target`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [-2,0,1,3], target = 2
 *     **Output:** 2
 * **Explanation:** Because there are two triplets which sums are less than 2:
 *     [-2,0,1]
 *     [-2,0,3]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [], target = 0
 *     **Output:** 0
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [0], target = 0
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == nums.length`
 *   * `0 <= n <= 3500`
 *   * `-100 <= nums[i] <= 100`
 *   * `-100 <= target <= 100`
 * * The input is generated such that the answer is less than or equal to 109.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/3sum-smaller/
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
    int threeSumSmaller(vector<int>& nums, int target) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [-2,0,1,3]
    // Test case 2: 2
    // Test case 3: []
    // Test case 4: 0
    // Test case 5: [0]
    // Test case 6: 0

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
