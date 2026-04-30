// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp
/*
 * LeetCode Problem 453: Minimum Moves to Equal Array Elements
 * Difficulty: Medium
 * Tags: Array, Math
 *
 * Problem Description:
 * Given an integer array `nums` of size `n`, return _the minimum number of
 * moves
 * required to make all array elements equal_.
 * 
 * In one move, you can increment `n - 1` elements of the array by `1`.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,3]
 *     **Output:** 3
 * **Explanation:** Only three moves are needed (remember each move increments
 * two elements):
 *     [1,2,3]  =>  [2,3,3]  =>  [3,4,3]  =>  [4,4,4]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [1,1,1]
 *     **Output:** 0
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `n == nums.length`
 *   * `1 <= nums.length <= 105`
 *   * `-109 <= nums[i] <= 109`
 *   * The answer is guaranteed to fit in a **32-bit** integer.
 * 
 * 
 *
 * Link: https://leetcode.com/problems/minimum-moves-to-equal-array-elements/
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
    int minMoves(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,3]
    // Test case 2: [1,1,1]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
