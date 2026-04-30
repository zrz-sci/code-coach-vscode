// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 462: Minimum Moves to Equal Array Elements II
 * Difficulty: Medium
 * Tags: Array, Math, Sorting
 *
 * Problem Description:
 * Given an integer array `nums` of size `n`, return _the minimum number of
 * moves
 * required to make all array elements equal_.
 * 
 * In one move, you can increment or decrement an element of the array by `1`.
 * 
 * Test cases are designed so that the answer will fit in a **32-bit** integer.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3]
 *     **Output:** 2
 *     **Explanation:**
 * Only two moves are needed (remember each move increments or decrements one
 * element):
 *     [_1_ ,2,3]  =>  [2,2,_3_]  =>  [2,2,2]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,10,2,9]
 *     **Output:** 16
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == nums.length`
 *   * `1 <= nums.length <= 105`
 *   * `-109 <= nums[i] <= 109`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/minimum-moves-to-equal-array-elements-ii/
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
    int minMoves2(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3]
    // Test case 2: [1,10,2,9]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
