// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 945: Minimum Increment to Make Array Unique
 * Difficulty: Medium
 * Tags: Greedy, Array, Counting, Sorting
 *
 * Problem Description:
 * You are given an integer array `nums`. In one move, you can pick an index
 * `i`
 * where `0 <= i < nums.length` and increment `nums[i]` by `1`.
 * 
 * Return _the minimum number of moves to make every value in_`nums`
 * _**unique**_.
 * 
 * The test cases are generated so that the answer fits in a 32-bit integer.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [1,2,2]
 *     **Output:** 1
 *     **Explanation:** After 1 move, the array could be [1, 2, 3].
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [3,2,1,2,1,7]
 *     **Output:** 6
 *     **Explanation:** After 6 moves, the array could be [3, 4, 1, 2, 5, 7].
 * It can be shown that it is impossible for the array to have all unique
 * values with 5 or less moves.
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 105`
 *   * `0 <= nums[i] <= 105`
 * 
 * 
 *
 * Link: https://leetcode.com/problems/minimum-increment-to-make-array-unique/
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
    int minIncrementForUnique(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [1,2,2]
    // Test case 2: [3,2,1,2,1,7]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
