// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 448: Find All Numbers Disappeared in an Array
 * Difficulty: Easy
 * Tags: Array, Hash Table
 *
 * Problem Description:
 * Given an array `nums` of `n` integers where `nums[i]` is in the range `[1,
 * n]`,
 * return _an array of all the integers in the range_ `[1, n]` _that do not
 * appear
 * in_ `nums`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [4,3,2,7,8,2,3,1]
 *     **Output:** [5,6]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,1]
 *     **Output:** [2]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == nums.length`
 *   * `1 <= n <= 105`
 *   * `1 <= nums[i] <= n`
 * 
 * 
 * 
 * **Follow up:** Could you do it without extra space and in `O(n)` runtime?
 * You
 * may assume the returned list does not count as extra space.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/find-all-numbers-disappeared-in-an-array/
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
    vector<int> findDisappearedNumbers(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [4,3,2,7,8,2,3,1]
    // Test case 2: [1,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
