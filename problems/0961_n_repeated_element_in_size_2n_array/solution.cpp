// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 961: N-Repeated Element in Size 2N Array
 * Difficulty: Easy
 * Tags: Array, Hash Table
 *
 * Problem Description:
 * You are given an integer array `nums` with the following properties:
 * 
 *   * `nums.length == 2 * n`.
 * * `nums` contains `n + 1` **unique** values, `n` of which occur **exactly
 * once** in the array.
 *   * Exactly one element of `nums` is repeated `n` times.
 * 
 * Return _the element that is repeated_`n` _times_.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3,3]
 *     **Output:** 3
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [2,1,2,5,3,2]
 *     **Output:** 2
 *     
 * 
 * **Example 3:**
 * 
 *     
 *     
 *     **Input:** nums = [5,1,5,2,5,3,5,4]
 *     **Output:** 5
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `2 <= n <= 5000`
 *   * `nums.length == 2 * n`
 *   * `0 <= nums[i] <= 104`
 * * `nums` contains `n + 1` **unique** elements and one of them is repeated
 * exactly `n` times.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/n-repeated-element-in-size-2n-array/
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
    int repeatedNTimes(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3,3]
    // Test case 2: [2,1,2,5,3,2]
    // Test case 3: [5,1,5,2,5,3,5,4]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
