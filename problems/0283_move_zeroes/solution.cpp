// 📖 Hints: ./hints.md
// 📖 Reference: ./reference.cpp

/*
 * LeetCode Problem 283: Move Zeroes
 * Difficulty: Easy
 * Tags: Array, Two Pointers
 *
 * Problem Description:
 * Given an integer array `nums`, move all `0`'s to the end of it while
 * maintaining
 * the relative order of the non-zero elements.
 * 
 * **Note** that you must do this in-place without making a copy of the array.
 * 
 * 
 * 
 * **Example 1:**
 * 
 *     
 *     
 *     **Input:** nums = [0,1,0,3,12]
 *     **Output:** [1,3,12,0,0]
 *     
 * 
 * **Example 2:**
 * 
 *     
 *     
 *     **Input:** nums = [0]
 *     **Output:** [0]
 *     
 * 
 * 
 * 
 * **Constraints:**
 * 
 *   * `1 <= nums.length <= 104`
 *   * `-231 <= nums[i] <= 231 - 1`
 * 
 * 
 * 
 * **Follow up:** Could you minimize the total number of operations done?
 * 
 * 
 *
 * Link: https://leetcode.com/problems/move-zeroes/
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
    void moveZeroes(vector<int>& nums) {
        
    }
};

// ==================== Test Harness ====================
// Modify this section to test your solution locally

int main() {
    Solution solution;

    // Example test cases from LeetCode:
    // Test case 1: [0,1,0,3,12]
    // Test case 2: [0]

    // TODO: Add your test cases here
    // Example:
    // vector<int> nums = {1, 2, 3};
    // auto result = solution.yourMethod(nums);
    // cout << "Result: " << result << endl;

    cout << "Tests completed!" << endl;
    return 0;
}
