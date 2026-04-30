// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 189: Rotate Array
 * Difficulty: Medium
 * Tags: Array, Math, Two Pointers
 *
 * Problem Description:
 * Given an integer array `nums`, rotate the array to the right by `k` steps,
 * where
 * `k` is non-negative.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3,4,5,6,7], k = 3
 *     **Output:** [5,6,7,1,2,3,4]
 *     **Explanation:**
 *     rotate 1 steps to the right: [7,1,2,3,4,5,6]
 *     rotate 2 steps to the right: [6,7,1,2,3,4,5]
 *     rotate 3 steps to the right: [5,6,7,1,2,3,4]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [-1,-100,3,99], k = 2
 *     **Output:** [3,99,-1,-100]
 *     **Explanation:** 
 *     rotate 1 steps to the right: [99,-1,-100,3]
 *     rotate 2 steps to the right: [3,99,-1,-100]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `-231 <= nums[i] <= 231 - 1`
 *   * `0 <= k <= 105`
 * 
 * 
 * 
 * **Follow up:**
 * 
 * * Try to come up with as many solutions as you can. There are at least
 * **three** different ways to solve this problem.
 *   * Could you do it in-place with `O(1)` extra space?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/rotate-array/
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
    void rotate(vector<int>& nums, int k) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,4,5,6,7]
    // Test case 2: 3
    // Test case 3: [-1,-100,3,99]
    // Test case 4: 2

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
